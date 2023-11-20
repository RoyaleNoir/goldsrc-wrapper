#include "cbase.h"
#include "goldsrc_usermessages_shared.h"
#include "networkstringtabledefs.h"

#ifdef CLIENT_DLL
#include "cdll_int.h"
#else
#include "eiface.h"
#include "irecipientfilter.h"
#include "globalvars_base.h"
#include "goldsrc_servergameents.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#ifdef CLIENT_DLL
extern IVEngineClient *engine;
#else
extern IVEngineServer *engine;
extern CGlobalVars *gpGlobals;
#endif

extern INetworkStringTableContainer *networkstringtable;
INetworkStringTable *g_pUserMsgNames = NULL;

#define ANGLE_BITS 8

static CGoldSRCUserMessages g_UserMessages;
CGoldSRCUserMessages *UserMessages()
{
	return &g_UserMessages;
}


CGoldSRCUserMessages::CGoldSRCUserMessages()
{
#ifndef CLIENT_DLL
	m_RegisteredMessages.SetLessFunc( DefLessFunc( int ) );
	m_buf = NULL;
#endif
}


CGoldSRCUserMessages::~CGoldSRCUserMessages()
{
	Shutdown();
}


void CGoldSRCUserMessages::Init()
{
}


void CGoldSRCUserMessages::Shutdown()
{
#if CLIENT_DLL
	m_Hooks.Purge();
#else
	m_RegisteredMessages.RemoveAll();
#endif
}

#ifdef CLIENT_DLL
bool CGoldSRCUserMessages::InstallStringTableCallback( const char *szTableName )
{
	if ( !Q_strcasecmp( szTableName, "UserMsgName" ) )
	{
		g_pUserMsgNames = networkstringtable->FindTable( szTableName );
		return true;
	}
	return false;
}

byte s_tempBytes[1024] = {};

bool CGoldSRCUserMessages::Dispatch( int type, bf_read &buf )
{
	if ( !g_pUserMsgNames )
		return false;

	if ( type > g_pUserMsgNames->GetNumStrings() )
	{
		Warning( "UserMsg %d/%d\n", type, g_pUserMsgNames->GetNumStrings() );
		return false;
	}

	const char *szName = g_pUserMsgNames->GetString( type );

	pfnUserMsgHook hook = m_Hooks.Get( szName, NULL );
	if ( !hook )
		return false;

	int size = buf.GetNumBytesLeft();

	if ( size > 1024 )
		return false;
	//byte *bytes = new byte[size * 8];

	buf.ReadBytes( s_tempBytes, size );
	hook( szName, size, s_tempBytes );

	//delete[] bytes;

	return true;
}


int CGoldSRCUserMessages::HookUserMsg( const char *szName, pfnUserMsgHook hook )
{
	m_Hooks.Insert( szName, hook );
	return 0;
}


#else

void CGoldSRCUserMessages::CreateStringTable()
{
	g_pUserMsgNames = networkstringtable->CreateStringTable( "UserMsgName", 64 );
}

int CGoldSRCUserMessages::Register( const char *pszName, int size )
{
	if ( !g_pUserMsgNames )
		return -1;

	int stringID = g_pUserMsgNames->AddString( true, pszName );
	MsgDesc d = { pszName, size };	// TODO: Should the name be copied?

	Msg( "MessageRegister %s %d (%d bytes)\n", pszName, stringID, size );

	m_RegisteredMessages.Insert( stringID, d );

	return stringID + FIRST_MOD_USERMSG;
}

bool CGoldSRCUserMessages::GetMessageInfo( int type, char *szName, int nameLength, int &size )
{
	unsigned short index = m_RegisteredMessages.Find( type );
	if ( !m_RegisteredMessages.IsValidIndex( index ) )
		return false;

	Q_strncpy( szName, m_RegisteredMessages.Element( index ).name, nameLength );
	size = m_RegisteredMessages.Element( index ).size;

	return true;
}


enum MsgDestination
{
	MSG_BROADCAST = 0,
	MSG_ONE,
	MSG_ALL,
	MSG_INIT,
	MSG_PVS,
	MSG_PAS,
	MSG_PVS_R,
	MSG_PAS_R,
	MSG_ONE_UNRELIABLE,
	MSG_SPEC
};

class CMsgRecipientFilter : public IRecipientFilter
{
public:
	CMsgRecipientFilter()
	{

	}

	void Init( int dest, const float *pOrigin, GoldSRC::edict_t *pEnt )
	{
		Reset();

		// Set reliable
		if ( dest == MSG_ONE || dest == MSG_INIT || dest == MSG_ALL || dest == MSG_PVS_R || dest == MSG_PAS_R )
			m_bReliable = true;

		// Broadcast (TODO: PVS/PAS)
		if ( dest == MSG_ALL || dest == MSG_INIT || dest == MSG_PVS || dest == MSG_PVS_R || dest == MSG_PAS || dest == MSG_PAS_R )
		{
			for ( int i = 1; i <= gpGlobals->maxClients; i++ )
			{
				CBaseEntity *pPlayer = g_pGoldSRCServerGameEnts->GetEntityByIndex( i );
				if ( pPlayer && pPlayer->IsPlayer() )
					m_Recipients.AddToTail( i );
			}
		}

		// Single user
		if ( pEnt && ( dest == MSG_ONE || dest == MSG_ONE_UNRELIABLE ) )
		{
			int index = g_pGoldSRCEdict->ToIndex( pEnt );
			m_Recipients.AddToTail( index );
		}
	}

	void Reset()
	{
		m_bReliable = false;
		m_Recipients.RemoveAll();
	}

	virtual bool IsReliable( void ) const
	{
		return m_bReliable;
	}

	virtual bool IsInitMessage( void ) const
	{
		return false;
	}

	virtual int GetRecipientCount( void ) const
	{
		return m_Recipients.Count();
	}

	virtual int GetRecipientIndex(int slot) const
	{
		return m_Recipients.Element( slot );
	}

private:
	CUtlVector<int> m_Recipients;
	bool m_bReliable;
};

static CMsgRecipientFilter s_Filter;

void CGoldSRCUserMessages::MessageBegin( int dest, int type, const float *pOrigin, GoldSRC::edict_t *pEnt )
{
	if ( type == -1 )
		return;

	int realType = type - FIRST_MOD_USERMSG;

	if ( type < FIRST_MOD_USERMSG )
	{
		// TODO
		return;
	}

	Msg( "MessageBegin %d\n", realType );

	s_Filter.Init( dest, pOrigin, pEnt );
	m_buf = engine->UserMessageBegin( &s_Filter, realType );
}


void CGoldSRCUserMessages::MessageEnd()
{
	if ( !m_buf )
		return;

	engine->MessageEnd();
	s_Filter.Reset();
	m_buf = NULL;
}


void CGoldSRCUserMessages::WriteByte( int iValue )
{
	if ( !m_buf )
		return;

	m_buf->WriteByte( iValue );
}


void CGoldSRCUserMessages::WriteChar( int iValue )
{
	if ( !m_buf )
		return;
	m_buf->WriteChar( iValue );
}


void CGoldSRCUserMessages::WriteShort( int iValue )
{
	if ( !m_buf )
		return;
	m_buf->WriteShort( iValue );
}


void CGoldSRCUserMessages::WriteLong( int iValue )
{
	if ( !m_buf )
		return;
	m_buf->WriteLong( iValue );
}


void CGoldSRCUserMessages::WriteAngle( float flValue )
{
	if ( !m_buf )
		return;
	m_buf->WriteBitAngle( flValue, ANGLE_BITS );	// TODO: How many bits?
}


void CGoldSRCUserMessages::WriteCoord( float flValue )
{
	if ( !m_buf )
		return;
	m_buf->WriteShort( flValue * 8 );
}


void CGoldSRCUserMessages::WriteString( const char *sz )
{
	if ( !m_buf )
		return;
	m_buf->WriteString( sz );
}


void CGoldSRCUserMessages::WriteEntity( int iValue )
{
	if ( !m_buf )
		return;
	m_buf->WriteShort( iValue );
}
#endif
