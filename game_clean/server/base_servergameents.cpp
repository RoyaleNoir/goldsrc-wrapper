#include "cbase.h"
#include "base_servergameents.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


void CBaseServerGameEnts::SetDebugEdictBase( edict_t *base )
{
	LOG_STUB();
}


void CBaseServerGameEnts::MarkEntitiesAsTouching( edict_t *e1, edict_t *e2 )
{
	LOG_STUB();
}


void CBaseServerGameEnts::FreeContainingEntity( edict_t *pEdict )
{
	LOG_STUB();
}


edict_t *CBaseServerGameEnts::BaseEntityToEdict( CBaseEntity *pEnt )
{
	LOG_STUB();

	return nullptr;
}


CBaseEntity *CBaseServerGameEnts::EdictToBaseEntity( edict_t *pEdict )
{
	LOG_STUB();

	return nullptr;
}


void CBaseServerGameEnts::CheckTransmit( CCheckTransmitInfo *pInfo, const unsigned short *pEdictIndices, int nEdicts )
{
	LOG_STUB();
}
