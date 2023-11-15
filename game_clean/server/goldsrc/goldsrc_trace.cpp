#include "cbase.h"
#include "goldsrc_trace.h"
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
		CBaseEntity *pTouched = (CBaseEntity*)pServerEntity;

		if ( pTouched == m_pIgnoreEntity )
			return false;

		return true; 
	}

private:
	CBaseEntity *m_pIgnoreEntity;
};


GoldSRC::TraceResult UTIL_TraceEntityHull( const Vector& start, const Vector& end, 
	const Vector &mins, const Vector &maxs, CBaseEntity *pIgnoreEnt )
{
	CTraceFilterClipToLinks filter( pIgnoreEnt );
	trace_t tr;

	Ray_t ray;
	ray.Init( start, end, mins, maxs );

	enginetrace->TraceRay( ray, MASK_NPCSOLID, &filter, &tr );

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
