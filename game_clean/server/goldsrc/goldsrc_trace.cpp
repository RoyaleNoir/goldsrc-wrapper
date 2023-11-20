#include "cbase.h"
#include "goldsrc_trace.h"
#include "goldsrc_playermove_server.h"
#include "engine/IEngineTrace.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IEngineTrace *enginetrace;


// Implements SV_ClipToLinks logic
class CTraceFilterClipToLinks : public CTraceFilter
{
public:
	CTraceFilterClipToLinks( CBaseEntity *pIgnoreEntity )
	{
		m_pIgnoreEntity = pIgnoreEntity;
	}

	virtual bool ShouldHitEntity( IHandleEntity *pServerEntity, int contentsMask )
	{
		if ( !UTIL_StandardTraceFilter( pServerEntity, contentsMask ) )
			return false;

		CBaseEntity *pTouched = (CBaseEntity*)pServerEntity;

		if ( pTouched == m_pIgnoreEntity )
			return false;

		return true; 
	}

private:
	CBaseEntity *m_pIgnoreEntity;
};


GoldSRC::TraceResult UTIL_TraceEntityHull( const Vector& start, const Vector& end, 
	const Vector &mins, const Vector &maxs, CBaseEntity *pIgnoreEnt, int traceType )
{
	CTraceFilterClipToLinks filter( pIgnoreEnt );
	trace_t tr;

	Ray_t ray;
	ray.Init( start, end, mins, maxs );

	int mask = traceType == 1 ? MASK_NPCSOLID_BRUSHONLY : MASK_NPCSOLID;

	enginetrace->TraceRay( ray, mask, &filter, &tr );

	GoldSRC::TraceResult gtr;
	gtr.fAllSolid = tr.allsolid;
	gtr.fStartSolid = tr.startsolid;
	gtr.fInOpen = (tr.contents == CONTENTS_EMPTY);
	gtr.fInWater = (tr.contents & CONTENTS_WATER) != 0;
	gtr.flFraction = tr.fraction;
	VectorCopy( tr.endpos.Base(), gtr.vecEndPos);
	gtr.flPlaneDist = tr.plane.dist;
	VectorCopy( tr.plane.normal.Base(), gtr.vecPlaneNormal);
	gtr.pHit = tr.m_pEnt ? tr.m_pEnt->GoldSrcEdict() : NULL;
	gtr.iHitgroup = 0; // TODO: What is this?

	return gtr;
}


void UTIL_TraceHull( const Vector &start, const Vector &end, int traceType, 
	int hullNumber, CBaseEntity *pIgnoreEnt, GoldSRC::TraceResult *ptr )
{
	Vector mins;
	Vector maxs;
	g_pGoldSRCPlayerMove->GetHull( hullNumber, mins, maxs );

	CTraceFilterClipToLinks filter( pIgnoreEnt );
	trace_t tr;

	Ray_t ray;
	ray.Init( start, end, mins, maxs );

	int mask = traceType == 1 ? MASK_NPCSOLID_BRUSHONLY : MASK_NPCSOLID;

	enginetrace->TraceRay( ray, mask, &filter, &tr );

	ptr->fAllSolid = tr.allsolid;
	ptr->fStartSolid = tr.startsolid;
	ptr->fInOpen = (tr.contents == CONTENTS_EMPTY);
	ptr->fInWater = (tr.contents & CONTENTS_WATER) != 0;
	ptr->flFraction = tr.fraction;
	VectorCopy( tr.endpos.Base(), ptr->vecEndPos);
	ptr->flPlaneDist = tr.plane.dist;
	VectorCopy( tr.plane.normal.Base(), ptr->vecPlaneNormal);
	ptr->pHit = tr.m_pEnt ? tr.m_pEnt->GoldSrcEdict() : NULL;
	ptr->iHitgroup = 0; // TODO: What is this?
}


void UTIL_TraceLine( const Vector &start, const Vector &end, int traceType, 
	CBaseEntity *pIgnoreEnt, GoldSRC::TraceResult *ptr )
{
	CTraceFilterClipToLinks filter( pIgnoreEnt );
	trace_t tr;

	Ray_t ray;
	ray.Init( start, end );

	int mask = (traceType & 3) == 1 ? MASK_NPCSOLID_BRUSHONLY : MASK_NPCSOLID;

	enginetrace->TraceRay( ray, mask, &filter, &tr );

	ptr->fAllSolid = tr.allsolid;
	ptr->fStartSolid = tr.startsolid;
	ptr->fInOpen = (tr.contents == CONTENTS_EMPTY);
	ptr->fInWater = (tr.contents & CONTENTS_WATER) != 0;
	ptr->flFraction = tr.fraction;
	VectorCopy( tr.endpos.Base(), ptr->vecEndPos);
	ptr->flPlaneDist = tr.plane.dist;
	VectorCopy( tr.plane.normal.Base(), ptr->vecPlaneNormal);
	ptr->pHit = tr.m_pEnt ? tr.m_pEnt->GoldSrcEdict() : NULL;
	ptr->iHitgroup = 0; // TODO: What is this?
}


bool UTIL_StandardTraceFilter( IHandleEntity *pHandleEntity, int fContentsMask )
{
	CBaseEntity *pEntity = (CBaseEntity*)pHandleEntity;
	if ( !pEntity )
		return false;

	SolidType_t solid = pEntity->GetCollideable()->GetSolid();

	if ( solid != SOLID_BSP )
	{
		if ( !( fContentsMask & CONTENTS_MONSTER ) )
			return false;
	}

	// TODO: CONTENTS_WINDOW

	// TODO: CONTENTS_MOVEABLE

	return true;
}
