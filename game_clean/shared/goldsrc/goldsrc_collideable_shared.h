#ifndef GOLDSRC_COLLIDEABLE_SHARED_H
#define GOLDSRC_COLLIDEABLE_SHARED_H

#ifdef _WIN32
#pragma once
#endif

#include "engine/ICollideable.h"

#if defined( CLIENT_DLL )
#define CBaseEntity C_BaseEntity
#endif

class CBaseEntity;

#ifdef CLIENT_DLL
EXTERN_RECV_TABLE( DT_Collideable );
#else
EXTERN_SEND_TABLE( DT_Collideable );
#endif

//-----------------------------------------------------------------------------
// Entity collision
//-----------------------------------------------------------------------------
class CCollideable : public ICollideable
{
public:
	DECLARE_CLASS_NOBASE( CCollideable );
	DECLARE_EMBEDDED_NETWORKVAR();

	CCollideable();
	~CCollideable();

	void Init( CBaseEntity *pEntity );

	void SetMinMax( const float *pMins, const float *pMaxs );
	void SetAbsMinMax( const float *pAbsMins, const float *pAbsMaxs );
	void SetSolidVars( int solid );

	// ICollideable
public:
	virtual IHandleEntity *GetEntityHandle();

	virtual const Vector& OBBMinsPreScaled() const;
	virtual const Vector& OBBMaxsPreScaled() const;
	virtual const Vector& OBBMins() const;
	virtual const Vector& OBBMaxs() const;

	virtual void WorldSpaceTriggerBounds( Vector *pVecWorldMins, Vector *pVecWorldMaxs ) const;

	virtual bool TestCollision( const Ray_t &ray, unsigned int fContentsMask, trace_t &tr );
	virtual bool TestHitboxes( const Ray_t &ray, unsigned int fContentsMask, trace_t &tr );

	virtual int GetCollisionModelIndex();
	virtual const model_t *GetCollisionModel();

	virtual const Vector &GetCollisionOrigin() const;
	virtual const QAngle &GetCollisionAngles() const;
	virtual const matrix3x4_t &CollisionToWorldTransform() const;

	virtual SolidType_t GetSolid() const;
	virtual int GetSolidFlags() const;

	virtual IClientUnknown *GetIClientUnknown();

	virtual int GetCollisionGroup() const;

	virtual void WorldSpaceSurroundingBounds( Vector *pVecMins, Vector *pVecMaxs );

	virtual bool ShouldTouchTrigger( int triggerSolidFlags ) const;

	virtual const matrix3x4_t *GetRootParentToWorldTransform() const;

private:
	CBaseEntity *m_pEntity;

	CNetworkVector( m_absmin );
	CNetworkVector( m_absmax );
	CNetworkVector( m_mins );
	CNetworkVector( m_maxs );

	CNetworkVar( unsigned char, m_nSolidType );
	CNetworkVar( unsigned short, m_usSolidFlags );
};

namespace GoldSRC
{
	enum SolidType
	{
		SOLID_NOT = 0,
		SOLID_TRIGGER,
		SOLID_BBOX,
		SOLID_SLIDEBOX,
		SOLID_BSP
	};
}


//-----------------------------------------------------------------------------
// Purpose: Converts the GoldSRC solid entvar to a SolidType_t
// Input  : solid - solid field of entvars_t
//-----------------------------------------------------------------------------
inline SolidType_t SourceSolidType( int solid )
{
	switch ( solid )
	{
	case GoldSRC::SOLID_NOT:
		return SOLID_NONE;
		break;
	case GoldSRC::SOLID_TRIGGER:
	case GoldSRC::SOLID_BBOX:
	case GoldSRC::SOLID_SLIDEBOX:
		return SOLID_BBOX;
		break;
	case GoldSRC::SOLID_BSP:
		return SOLID_BSP;
	default:
		return SOLID_NONE;
	}
}


//-----------------------------------------------------------------------------
// Purpose: Generates appropriate solid flags for a GoldSRC solid entvar
// Input  : solid - solid field of entvars_t
//-----------------------------------------------------------------------------
inline int SourceSolidFlags( int solid )
{
	switch ( solid )
	{
	case GoldSRC::SOLID_TRIGGER:
		return FSOLID_NOT_SOLID;
	case GoldSRC::SOLID_SLIDEBOX:
		return FSOLID_NOT_STANDABLE;
	default:
		return 0;
	}
}

#endif // GOLDSRC_COLLIDEABLE_SHARED_H