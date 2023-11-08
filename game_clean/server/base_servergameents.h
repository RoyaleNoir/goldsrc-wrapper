#ifndef BASE_SERVERGAMEENTS_H
#define BASE_SERVERGAMEENTS_H

#ifdef _WIN32
#pragma once
#endif

#include "eiface.h"

class CBaseServerGameEnts : public IServerGameEnts
{
public:
	virtual void	SetDebugEdictBase( edict_t *base );
	virtual void	MarkEntitiesAsTouching( edict_t *e1, edict_t *e2 );
	virtual void	FreeContainingEntity( edict_t *pEdict );
	virtual edict_t*	BaseEntityToEdict( CBaseEntity *pEnt );
	virtual CBaseEntity	*EdictToBaseEntity( edict_t *pEdict );
	virtual void	CheckTransmit( CCheckTransmitInfo *pInfo, const unsigned short *pEdictIndices, int nEdicts );
};

extern CBaseServerGameEnts *g_pServerGameEnts;

#endif // BASE_SERVERGAMEENTS_H
