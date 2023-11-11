#include "cbase.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"
#include "goldsrc_baseentity.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineServer *engine;


CGoldSRCEdict g_GoldSRCEdict;
CGoldSRCEdict *g_pGoldSRCEdict = &g_GoldSRCEdict;


CGoldSRCEdict::CGoldSRCEdict()
{
}


CGoldSRCEdict::~CGoldSRCEdict()
{
}


void CGoldSRCEdict::Init()
{
	for ( int i = 0; i < MAX_EDICTS; i++ ) {
		m_realedicts[i] = NULL;
		InitEdict( &m_edicts[i] );
		m_edicts[i].free = TRUE;
	}
}


void CGoldSRCEdict::EnsureAllFreed()
{
	for ( int i = 0; i < MAX_EDICTS; i++ ) {
		if ( m_edicts[i].free )
			continue;

		Warning( "GoldSrc edict %d leaked!\n", i );
		Assert( 0 );
		RemoveEntityByGoldSRCEdict( &m_edicts[i] );
	}
}


CBaseEntity *CGoldSRCEdict::CreateEntity( int iForceEdict )
{
	edict_t *pOverrideEdict = NULL;

	// Doing this here
	if ( iForceEdict != -1 )
	{
		// Attach to the existing edict if available
		pOverrideEdict = engine->PEntityOfEntIndex( iForceEdict );

		// Otherwise create it
		if ( !pOverrideEdict )
		{
			pOverrideEdict = engine->CreateEdict( iForceEdict );
			if ( !pOverrideEdict )
				return NULL;
		}
	}

	CBaseEntity *pNewEntity = new CBaseEntity;
	pNewEntity->AttachEdict( pOverrideEdict );

	return pNewEntity;
}


CBaseEntity *CGoldSRCEdict::CreateEntityByName( GoldSRC::string_t classname, int iForceEdict )
{
	GoldSRC::pfnCreateEntityFunc_t func = GetCreateEntityFunc( GoldSRC::SzFromIndex( classname ) );

	if ( !func )
	{
		//Msg( "Entity creation function not found for %s\n", GoldSRC::SzFromIndex( classname ) );
		return NULL;
	}

	CBaseEntity *pEntity = CreateEntity( iForceEdict );
	if ( !pEntity )
		return NULL;

	GoldSRC::edict_t *pEdict = pEntity->GoldSrcEdict();
	pEdict->v.classname = classname;
	func( &pEdict->v );

	pEntity->PostGoldSrcCreate();

	return pEntity;
}


GoldSRC::edict_t *CGoldSRCEdict::GetGoldSRCEdict( edict_t *pRealEdict, bool bCreate )
{
	int index = pRealEdict->m_EdictIndex;
	GoldSRC::edict_t *pEdict = &m_edicts[index];

	if ( !pEdict->free )
	{
		// TODO: Check if it's the correct one!!!
		return pEdict;
	}

	if ( bCreate )
	{
		InitEdict( pEdict );
		m_realedicts[index] = pRealEdict;

		return pEdict;
	}

	return NULL;
}


void CGoldSRCEdict::DestroyGoldSRCEdict( GoldSRC::edict_t *pEdict )
{
	FreeEdict( pEdict );
}


GoldSRC::edict_t *CGoldSRCEdict::CreateGoldSRCEntity( int iForceEdict )
{
	return CreateEntity( iForceEdict )->GoldSrcEdict();
}


GoldSRC::edict_t *CGoldSRCEdict::CreateGoldSRCEntityByName( GoldSRC::string_t classname, int iForceEdict )
{
	return CreateEntityByName( classname, iForceEdict )->GoldSrcEdict();
}


void CGoldSRCEdict::RemoveEntityByGoldSRCEdict( GoldSRC::edict_t *pEdict )
{
	if ( !pEdict || pEdict->free )
		return;

	int entindex = ToIndex( pEdict );

	if ( m_realedicts[entindex] != NULL )
	{
		IServerUnknown *pUnknown = m_realedicts[entindex]->GetUnknown();

		if ( !pUnknown )
			return;

		IServerNetworkable *pNetworkable = m_realedicts[entindex]->GetNetworkable();
		
		if ( pNetworkable )
			pNetworkable->Release();

		m_realedicts[entindex] = NULL;
	}
}


void *CGoldSRCEdict::AllocPrivateData( GoldSRC::edict_t *pEdict, int32 size )
{
	if ( pEdict->pvPrivateData )
	{
		FreePrivateData( pEdict );
	}

	pEdict->pvPrivateData = malloc( size );

	if ( pEdict->pvPrivateData )
	{
		Q_memset( pEdict->pvPrivateData, 0, size );
	}

	return pEdict->pvPrivateData;
}


void CGoldSRCEdict::FreePrivateData( GoldSRC::edict_t *pEdict )
{
	if ( pEdict->pvPrivateData )
	{
		free( pEdict->pvPrivateData );
		pEdict->pvPrivateData = NULL;
	}
}


void *CGoldSRCEdict::GetPrivateData( GoldSRC::edict_t *pEdict )
{
	return pEdict->pvPrivateData;
}


int CGoldSRCEdict::ToOffset( const GoldSRC::edict_t *pEdict )
{
	return ( (int)pEdict - (int)&m_edicts  );
}


GoldSRC::edict_t *CGoldSRCEdict::FromOffset( int offset )
{
	return (GoldSRC::edict_t *)( (int)&m_edicts + offset );
}


int CGoldSRCEdict::ToIndex( const GoldSRC::edict_t *pEdict )
{
	return ToOffset( pEdict ) / sizeof( GoldSRC::edict_t );
}


GoldSRC::edict_t *CGoldSRCEdict::FromIndex( int index )
{
	return &m_edicts[index];
}


GoldSRC::edict_t *CGoldSRCEdict::FindByClassName( GoldSRC::edict_t *pStart, const char *szClassName )
{
	int i = 0;

	if ( pStart )
		i = ToIndex( pStart );

	for ( ; i < MAX_EDICTS; i++ )
	{
		if ( m_edicts[i].free )
			continue;

		if ( !Q_strcmp( GoldSRC::SzFromIndex( m_edicts[i].v.classname ), szClassName ) )
		{
			return &m_edicts[i];
		}
	}

	return NULL;
}

CBaseEntity *CGoldSRCEdict::GetSourceEntity( GoldSRC::edict_t *pGoldSRCEdict )
{
	if ( !pGoldSRCEdict || pGoldSRCEdict->free )
		return NULL;

	int entindex = ToIndex( pGoldSRCEdict );

	if ( m_realedicts[entindex] != NULL )
	{
		IServerUnknown *pUnknown = m_realedicts[entindex]->GetUnknown();

		if ( !pUnknown )
			return NULL;

		return pUnknown->GetBaseEntity();
	}

	return NULL;
}


void CGoldSRCEdict::LinkEdict( GoldSRC::edict_t *pEdict, bool touch_triggers )
{
	// TODO
}


void CGoldSRCEdict::UnlinkEdict( GoldSRC::edict_t *pEdict )
{
	// TODO
}


void CGoldSRCEdict::InitEdict( GoldSRC::edict_t *pEdict )
{
	pEdict->free = FALSE;
	pEdict->pvPrivateData = NULL;

	// Clear out the pev
	Q_memset( &pEdict->v, 0, sizeof( GoldSRC::entvars_t ) );
	pEdict->v.pContainingEntity = pEdict;
	
	// Init some safe defaults
	pEdict->v.modelindex = -1;
}


void CGoldSRCEdict::FreeEdict( GoldSRC::edict_t *pEdict )
{
	pEdict->free = TRUE;
	pEdict->serialnumber++;
	FreePrivateData( pEdict );
}
