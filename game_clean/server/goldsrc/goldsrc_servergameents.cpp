#include "cbase.h"
#include "goldsrc_servergameents.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_edict.h"
#include "goldsrc_globalvars.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineServer *engine;


CGoldSRCServerGameEnts g_ServerGameEnts;
CBaseServerGameEnts *g_pServerGameEnts = &g_ServerGameEnts;
CGoldSRCServerGameEnts *g_pGoldSRCServerGameEnts = &g_ServerGameEnts;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCServerGameEnts, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS, g_ServerGameEnts );


CGoldSRCServerGameEnts::CGoldSRCServerGameEnts()
{
	for ( int i = 0; i < NUM_ENT_ENTRIES; i++ )
	{
		m_entries[i].m_pEntity = NULL;
		m_entries[i].m_SerialNumber = rand() & 0x7fff;
	}
}


CGoldSRCServerGameEnts::~CGoldSRCServerGameEnts()
{
}


void CGoldSRCServerGameEnts::ReleaseAll()
{
	for ( int i = 0; i < NUM_ENT_ENTRIES; i++ )
	{
		CBaseEntity *pEnt = m_entries[i].m_pEntity;
		if ( !pEnt )
			continue;
		
		// This should call RemoveEntityByHandle(), and delete the GoldSrc edict
		pEnt->GetNetworkable()->Release();
	}
}


CBaseHandle CGoldSRCServerGameEnts::AddEntity( CBaseEntity *pEnt, int index )
{
	if ( index == INVALID_EHANDLE_INDEX )
		return CBaseHandle( INVALID_EHANDLE_INDEX );

	GoldSRCEntityListEntry_t *pEntry = &m_entries[index];
	pEntry->m_pEntity = pEnt;

	CBaseHandle hEnt( index, pEntry->m_SerialNumber );
	pEnt->SetRefEHandle( hEnt );

	return hEnt;
}


void CGoldSRCServerGameEnts::RemoveEntityByIndex( int index )
{
	if ( index == INVALID_EHANDLE_INDEX )
		return;

	GoldSRCEntityListEntry_t *pEntry = &m_entries[index];

	if ( pEntry->m_pEntity )
	{
		pEntry->m_pEntity->SetRefEHandle( INVALID_EHANDLE_INDEX );
		pEntry->m_pEntity = NULL;
		pEntry->m_SerialNumber = ( pEntry->m_SerialNumber + 1) & 0x7fff;
	}
}


void CGoldSRCServerGameEnts::RemoveEntityByHandle( CBaseHandle hEnt )
{
	RemoveEntityByIndex( hEnt.GetEntryIndex() );
}


CBaseEntity *CGoldSRCServerGameEnts::GetEntityByIndex( int index )
{
	if ( index < 0 || index > NUM_ENT_ENTRIES )
		return NULL;

	return m_entries[index].m_pEntity;
}


//-----------------------------------------------------------------------------
// Purpose: Runs a tick for every entity.
//-----------------------------------------------------------------------------
void CGoldSRCServerGameEnts::TickEntities()
{
	// TODO: Faster way to iterate
	for ( int i = 0; i < NUM_ENT_ENTRIES; i++ )
	{
		CBaseEntity *pEntity = m_entries[i].m_pEntity;
		if ( !pEntity )
			continue;

		if ( pEntity->IsPlayer() )
			continue;

		//if ( GlobalVars()->force_retouch )
		//	pEntity->Link( true );

		pEntity->RunTick();
		// SV_RunNewmis()
	}

	if ( GlobalVars()->force_retouch )
		GlobalVars()->force_retouch--;
}


void CGoldSRCServerGameEnts::FreeContainingEntity( edict_t *pEdict )
{
	/*
	GoldSRC::edict_t *pGEdict = g_pGoldSRCEdict->GetGoldSRCEdict( pEdict );

	if ( pGEdict )
	{
		g_pGoldSRCEdict->RemoveEntity( pGEdict );
	}
	*/

	if ( !pEdict )
		return;

	IServerUnknown *pUnknown = pEdict->GetUnknown();

	if ( !pUnknown )
		return;

	CBaseEntity *pEntity = pUnknown->GetBaseEntity();

	if ( pEntity )
		delete pEntity;
}


edict_t *CGoldSRCServerGameEnts::BaseEntityToEdict( CBaseEntity *pEnt )
{
	return pEnt ? pEnt->GetSourceEdict() : NULL;
}


CBaseEntity *CGoldSRCServerGameEnts::EdictToBaseEntity( edict_t *pEdict )
{
	if ( !pEdict )
		return NULL;

	IServerUnknown *pUnknown = pEdict->GetUnknown();

	return pUnknown ? pUnknown->GetBaseEntity() : NULL;
}

void CGoldSRCServerGameEnts::CheckTransmit( CCheckTransmitInfo *pInfo, const unsigned short *pEdictIndices, int nEdicts )
{
	for ( int i = 0; i < nEdicts; i++ )
	{
		// Transmit everything for now
		pInfo->m_pTransmitEdict->Set( i );
	}
}
