#include "cbase.h"
#include "goldsrc_servergameclients.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"
#include "goldsrc_playermove_shared.h"
#include "goldsrc_playermove_server.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CGoldSRCServerGameClients g_ServerGameClients;
extern CBaseServerGameClients *g_pServerGameClients = &g_ServerGameClients;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCServerGameClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS, g_ServerGameClients );


bool CGoldSRCServerGameClients::ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	/*
	GoldSRC::edict_t *pGoldSRCEdict = g_pGoldSRCEdict->GetGoldSRCEdict( pEntity, true );

	if ( !pGoldSRCEdict )
		return false;

	char szRejectReason[128];
	GoldSRC::qboolean bConnect =  g_pGoldSRCEntityInterface->ClientConnect( pGoldSRCEdict, pszName, pszAddress, szRejectReason );
	Q_strncpy( reject, szRejectReason, maxrejectlen );

	return bConnect != FALSE;
	*/

	return true;
}


void CGoldSRCServerGameClients::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	CBaseEntity *pBaseEntity = g_pGoldSRCEdict->CreateEntity( pEntity->m_EdictIndex );
	if ( !pBaseEntity )
		return;

	GoldSRC::edict_t *pGoldSRCEdict = pBaseEntity->GoldSrcEdict();
	if ( !pGoldSRCEdict )
		return;

	g_pGoldSRCEntityInterface->ClientPutInServer( pGoldSRCEdict );
	
	// Need to do this manually
	pBaseEntity->SetIsPlayer( true );
	pBaseEntity->PostGoldSrcCreate();
	pBaseEntity->PostGoldSrcSpawn();
}

#define READ_DELTA_I( x, t, b, s ) if ( buf->ReadOneBit() )\
{\
	pTo->##x = buf->Read##t( b );\
}\
else\
{\
	pTo->##x = pFrom->##x;\
}

#define READ_DELTA_F( x, t, b, s ) if ( buf->ReadOneBit() )\
{\
	pTo->##x = buf->Read##t( b ) / s;\
}\
else\
{\
	pTo->##x = pFrom->##x;\
}

#define NUM_USERCMD_LISTS 16	// Hopefully this is enough

static CUserCmdList s_NullList = { {}, 1 };

float CGoldSRCServerGameClients::ProcessUsercmds( edict_t *player, bf_read *buf, int numcmds, int totalcmds, int dropped_packets, bool ignore, bool paused )
{
	if ( totalcmds < 0 || totalcmds >= (NUM_USERCMD_LISTS - 1) )
	{
		Warning( "CGoldSRCServerGameClients::ProcessUsercmds: too many cmd lists!\n" );
		buf->SetOverflowFlag();
		return 0.0f;
	}

	CUserCmdList cmdlists[NUM_USERCMD_LISTS] = {};

	GoldSRC::usercmd_t *pFrom = &s_NullList.m_Cmds[0];
	GoldSRC::usercmd_t *pTo = NULL;

	// Read in all the lists
	for ( int i = totalcmds - 1; i >= 0; i-- )
	{
		CUserCmdList *pList = &cmdlists[i];

		int nCmdsInList = buf->ReadByte();
		pList->m_nNumCmds = nCmdsInList;

		for ( int j = 0; j < nCmdsInList; j++ )
		{
			pTo = &pList->m_Cmds[j];

			READ_DELTA_I	( lerp_msec,			UBitLong,	9,	1.0f );
			READ_DELTA_I	( msec,					UBitLong,	8,	1.0f );
			READ_DELTA_I	( viewangles[1],		BitAngle,	16,	1.0f );
			READ_DELTA_I	( viewangles[0],		BitAngle,	16,	1.0f );
			READ_DELTA_I	( buttons,				UBitLong,	8,	1.0f );
			READ_DELTA_F	( forwardmove,			SBitLong,	12,	1.0f );
			READ_DELTA_I	( lightlevel,			UBitLong,	8,	1.0f );
			READ_DELTA_F	( sidemove,				SBitLong,	12,	1.0f );
			READ_DELTA_F	( upmove,				SBitLong,	12,	1.0f );
			READ_DELTA_I	( impulse,				UBitLong,	8,	1.0f );
			READ_DELTA_I	( viewangles[2],		BitAngle,	16,	1.0f );
			READ_DELTA_I	( impact_index,			UBitLong,	6,	1.0f );
			READ_DELTA_F	( impact_position[0],	SBitLong,	16,	8.0f );
			READ_DELTA_F	( impact_position[1],	SBitLong,	16,	8.0f );
			READ_DELTA_F	( impact_position[2],	SBitLong,	16,	8.0f );

			if ( buf->IsOverflowed() )
			{
				Warning( "CGoldSRCServerGameClients::ProcessUsercmds: buffer overflowed!\n" );
				return 0.0f;
			}

			pFrom = pTo;
		}
	}

	if ( ignore || paused )
		return 0.0f;

	IServerUnknown *pUnknown = player->GetUnknown();
	if ( !pUnknown )
		return 0.0f;

	CBaseEntity *pEntity = pUnknown->GetBaseEntity();
	if ( !pEntity )
		return 0.0f;

	g_pGoldSRCPlayerMove->ProcessUsercmds( pEntity, cmdlists, numcmds, totalcmds, dropped_packets, paused );

	return DEFAULT_TICK_INTERVAL;
}


CPlayerState *CGoldSRCServerGameClients::GetPlayerState( edict_t *player )
{
	IServerUnknown *pUnknown = player->GetUnknown();
	if ( !pUnknown )
		return NULL;

	CBaseEntity *pEntity = pUnknown->GetBaseEntity();
	if ( !pEntity )
		return NULL;

	return pEntity->GetPlayerState();
}


void CGoldSRCServerGameClients::ClientEarPosition( edict_t *pEntity, Vector *pEarOrigin )
{
	IServerUnknown *pUnknown = pEntity->GetUnknown();
	if ( !pUnknown )
	{
		*pEarOrigin = vec3_origin;
		return;
	}

	CBaseEntity *pPlayer = pUnknown->GetBaseEntity();
	if ( !pPlayer )
	{
		*pEarOrigin = vec3_origin;
		return;
	}

	VectorAdd( pPlayer->EntVars()->origin, pPlayer->EntVars()->view_ofs, pEarOrigin->Base() );
}
