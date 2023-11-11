#include "cbase.h"
#include "goldsrc_collideable_shared.h"

#if defined( CLIENT_DLL )
#include "goldsrc_c_baseentity.h"
#else
#include "goldsrc_baseentity.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

#if defined ( CLIENT_DLL )
BEGIN_RECV_TABLE_NOBASE( CCollideable, DT_Collideable )
	RecvPropVector( RECVINFO( m_absmin ) ),
	RecvPropVector( RECVINFO( m_absmax ) ),
	RecvPropVector( RECVINFO( m_mins ) ),
	RecvPropVector( RECVINFO( m_maxs ) ),
	RecvPropInt( RECVINFO( m_nSolidType ) ),
	RecvPropInt( RECVINFO( m_usSolidFlags ) ),
END_RECV_TABLE()
#else
BEGIN_SEND_TABLE_NOBASE( CCollideable, DT_Collideable )
	SendPropVector( SENDINFO( m_absmin ), 0, SPROP_NOSCALE ),
	SendPropVector( SENDINFO( m_absmax ), 0, SPROP_NOSCALE ),
	SendPropVector( SENDINFO( m_mins ), 0, SPROP_NOSCALE ),
	SendPropVector( SENDINFO( m_maxs ), 0, SPROP_NOSCALE ),
	SendPropInt( SENDINFO( m_nSolidType ) ),
	SendPropInt( SENDINFO( m_usSolidFlags ) ),
END_SEND_TABLE()
#endif


CCollideable::CCollideable()
{
	Init( NULL );
}


CCollideable::~CCollideable()
{
}


void CCollideable::Init( CBaseEntity *pEntity )
{
	m_pEntity = pEntity;

	m_absmin.GetForModify().Init();
	m_absmax.GetForModify().Init();
	m_mins.GetForModify().Init();
	m_maxs.GetForModify().Init();

	m_nSolidType = SOLID_NONE;
	m_usSolidFlags = 0;
}


void CCollideable::SetMinMax( const float *pMins, const float *pMaxs )
{
	Vector v;
	
	VectorCopy( pMins, v.Base() );
	if ( v != m_mins )
		m_mins = v;

	VectorCopy( pMaxs, v.Base() );
	if ( v != m_maxs )
		m_maxs = v;
}


void CCollideable::SetAbsMinMax( const float *pAbsMins, const float *pAbsMaxs )
{
	Vector v;

	VectorCopy( pAbsMins, v.Base() );
	if ( v != m_absmin )
		m_absmin = v;

	VectorCopy( pAbsMaxs, v.Base() );
	if ( v != m_absmax )
		m_absmax = v;
}

void CCollideable::SetSolidVars( int solid )
{
	unsigned char nSolidType = SourceSolidType( solid );
	if ( m_nSolidType != nSolidType )
		m_nSolidType = nSolidType;
	
	unsigned short usSolidFlags = SourceSolidFlags( solid );
	if ( m_usSolidFlags != usSolidFlags )
		m_usSolidFlags = usSolidFlags;
}


IHandleEntity *CCollideable::GetEntityHandle()
{
	return m_pEntity;
}


const Vector &CCollideable::OBBMinsPreScaled() const
{
	return m_mins;
}


const Vector &CCollideable::OBBMaxsPreScaled() const
{
	return m_maxs;
}


const Vector &CCollideable::OBBMins() const
{
	return m_mins;
}


const Vector &CCollideable::OBBMaxs() const
{
	return m_maxs;
}


void CCollideable::WorldSpaceTriggerBounds( Vector *pVecWorldMins, Vector *pVecWorldMaxs ) const
{
}


bool CCollideable::TestCollision( const Ray_t &ray, unsigned int fContentsMask, trace_t &tr )
{
	return false;
}


bool CCollideable::TestHitboxes( const Ray_t &ray, unsigned int fContentsMask, trace_t &tr )
{
	return false;
}


int CCollideable::GetCollisionModelIndex()
{
	return -1;
}


const model_t *CCollideable::GetCollisionModel()
{
	return NULL;
}


const Vector &CCollideable::GetCollisionOrigin() const
{
	return m_pEntity->GetAbsOrigin();
}


const QAngle &CCollideable::GetCollisionAngles() const
{
	return m_pEntity->GetAbsAngles();
}


const matrix3x4_t &CCollideable::CollisionToWorldTransform() const
{
	static matrix3x4_t s_matTemp[4];
	static int s_nIndex = 0;

	matrix3x4_t &matResult = s_matTemp[s_nIndex];
	s_nIndex = ( s_nIndex + 1 ) & 3;

	SetIdentityMatrix( matResult );
	MatrixSetColumn( GetCollisionOrigin(), 3, matResult );

	return matResult;
}


SolidType_t CCollideable::GetSolid() const
{
	return (SolidType_t)m_nSolidType.Get();
}


int CCollideable::GetSolidFlags() const
{
	return m_usSolidFlags;
}


IClientUnknown *CCollideable::GetIClientUnknown()
{
#if defined( CLIENT_DLL )
	return m_pEntity;
#else
	return NULL;
#endif
}


int CCollideable::GetCollisionGroup() const
{
	return 0;
}


void CCollideable::WorldSpaceSurroundingBounds( Vector *pVecMins, Vector *pVecMaxs )
{
	VectorCopy( m_absmin.Get().Base(), pVecMins->Base() );
	VectorCopy( m_absmax.Get().Base(), pVecMaxs->Base() );
}


bool CCollideable::ShouldTouchTrigger( int triggerSolidFlags ) const
{
	return false;
}


const matrix3x4_t *CCollideable::GetRootParentToWorldTransform() const
{
	return NULL;
}
