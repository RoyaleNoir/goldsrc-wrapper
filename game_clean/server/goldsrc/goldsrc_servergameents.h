#ifndef GOLDSRC_SERVERGAMEENTS_H
#define GOLDSRC_SERVERGAMEENTS_H

#ifdef _WIN32
#pragma once
#endif

#include "base_servergameents.h"
#include "goldsrc_baseentity.h"


//-----------------------------------------------------------------------------
// An entry in the entity list
//-----------------------------------------------------------------------------
struct GoldSRCEntityListEntry_t
{
	CBaseEntity *m_pEntity;
	int m_SerialNumber;
};


class CGoldSRCServerGameEnts : public CBaseServerGameEnts
{
public:
	CGoldSRCServerGameEnts();
	~CGoldSRCServerGameEnts();

	void ReleaseAll();

	CBaseHandle AddEntity( CBaseEntity *pEnt, int index );
	void RemoveEntityByIndex( int index );
	void RemoveEntityByHandle( CBaseHandle hEnt );

	CBaseEntity *GetEntityByIndex( int index );

	void TickEntities();

public:
	virtual void		FreeContainingEntity( edict_t *pEdict );
	virtual edict_t		*BaseEntityToEdict( CBaseEntity *pEnt );
	virtual CBaseEntity	*EdictToBaseEntity( edict_t *pEdict );
	virtual void		CheckTransmit( CCheckTransmitInfo *pInfo, const unsigned short *pEdictIndices, int nEdicts );

private:
	GoldSRCEntityListEntry_t m_entries[NUM_ENT_ENTRIES];
};

extern CGoldSRCServerGameEnts *g_pGoldSRCServerGameEnts;

#endif // GOLDSRC_SERVERGAMEENTS_H
