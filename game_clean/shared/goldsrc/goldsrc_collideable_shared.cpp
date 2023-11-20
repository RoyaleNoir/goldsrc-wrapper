#include "cbase.h"
#include "goldsrc_collideable_shared.h"
#include "tier0/tslist.h"

#if !defined( CLIENT_DLL )
#include "ispatialpartition.h"
#endif

#if defined( CLIENT_DLL )
#include "goldsrc_c_baseentity.h"
#else
#include "goldsrc_servergameents.h"
#include "goldsrc_baseentity.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


#if !defined( CLIENT_DLL )
extern ISpatialPartition* partition;
#endif


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
#if !defined( CLIENT_DLL )
	if ( m_PartitionHandle != PARTITION_INVALID_HANDLE )
	{
		partition->DestroyHandle( m_PartitionHandle );
		m_PartitionHandle = PARTITION_INVALID_HANDLE;
	}
#endif
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

#if !defined( CLIENT_DLL )
	m_PartitionHandle = PARTITION_INVALID_HANDLE;
#endif
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

	MarkDirty();
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

	MarkDirty();
}

void CCollideable::SetSolidVars( int solid )
{
	unsigned char nSolidType = SourceSolidType( solid );
	if ( m_nSolidType != nSolidType )
		m_nSolidType = nSolidType;
	
	unsigned short usSolidFlags = SourceSolidFlags( solid );
	if ( m_usSolidFlags != usSolidFlags )
		m_usSolidFlags = usSolidFlags;

	MarkDirty();

#if !defined( CLIENT_DLL )
	UpdatePartitionMask();
#endif
}


void CCollideable::UpdatePartition()
{
#if !defined( CLIENT_DLL )
	if ( m_PartitionHandle == PARTITION_INVALID_HANDLE )
	{
		m_PartitionHandle = partition->CreateHandle( GetEntityHandle() );
		UpdatePartitionMask();
	}

	if ( m_PartitionHandle == PARTITION_INVALID_HANDLE )
		return;

	bool isSolid = m_nSolidType != SOLID_NONE && !( m_usSolidFlags & FSOLID_NOT_SOLID );
	bool isTrigger = ( m_usSolidFlags & FSOLID_TRIGGER ) != 0;

	if ( isSolid || isTrigger )
		partition->ElementMoved( m_PartitionHandle, m_absmin, m_absmax );
#endif
}


void CCollideable::UpdatePartitionMask()
{
#if !defined( CLIENT_DLL )
	if ( m_PartitionHandle == PARTITION_INVALID_HANDLE )
		return;

	partition->Remove( m_PartitionHandle );

	if ( m_pEntity->GetSourceEdict()->m_EdictIndex == 0 )
		return;

	bool isSolid = m_nSolidType != SOLID_NONE && !( m_usSolidFlags & FSOLID_NOT_SOLID );
	bool isTrigger = ( m_usSolidFlags & FSOLID_TRIGGER ) != 0;

	if ( !isSolid && !isTrigger )
		return;

	partition->Insert( PARTITION_ENGINE_NON_STATIC_EDICTS, m_PartitionHandle );

	SpatialPartitionListMask_t mask = 0;

	if ( isSolid )
		mask |= PARTITION_ENGINE_SOLID_EDICTS;
	if ( isTrigger )
		mask |= PARTITION_ENGINE_TRIGGER_EDICTS;

	partition->Insert( mask, m_PartitionHandle );
#endif
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
#if defined( CLIENT_DLL )
	return NULL;
#else
	return m_pEntity->GetModelIndex();
#endif
}


const model_t *CCollideable::GetCollisionModel()
{
#if defined( CLIENT_DLL )
	return NULL;
#else
	return m_pEntity->GetSourceModel();
#endif
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
	if ( m_usSolidFlags & FSOLID_TRIGGER )
		return false;

	return true;
}


const matrix3x4_t *CCollideable::GetRootParentToWorldTransform() const
{
	return NULL;
}


#if !defined( CLIENT_DLL )
class CPartitionQueryCallback : public IPartitionQueryCallback
{
public:
	CPartitionQueryCallback();
	~CPartitionQueryCallback();

	void AddEntity( CBaseEntity *pEntity );
	void RemoveAll();

	virtual void OnPreQuery_V1() {}
	virtual void OnPreQuery( SpatialPartitionListMask_t listMask );
	virtual void OnPostQuery( SpatialPartitionListMask_t listMask );

private:
	CTSListWithFreeList<CBaseHandle> m_DirtyEntities;
};

static CPartitionQueryCallback s_PartitionQueryCallback;

// TODO:  lock/unlock?

void PartitionQueryCallBackInit()
{
	partition->InstallQueryCallback( &s_PartitionQueryCallback );
}

void PartitionQueryCallBackShutdown()
{
	partition->RemoveQueryCallback( &s_PartitionQueryCallback );
}

void PartitionQueryCallBackLevelShutdownPostEntity()
{
	s_PartitionQueryCallback.RemoveAll();
}


CPartitionQueryCallback::CPartitionQueryCallback()
{
	m_DirtyEntities.Purge();
}


CPartitionQueryCallback::~CPartitionQueryCallback()
{
	m_DirtyEntities.Purge();
}


void CPartitionQueryCallback::AddEntity( CBaseEntity *pEntity )
{
	m_DirtyEntities.PushItem( pEntity->GetRefEHandle() );
}


void CPartitionQueryCallback::RemoveAll()
{
	m_DirtyEntities.RemoveAll();
}


void CPartitionQueryCallback::OnPreQuery( SpatialPartitionListMask_t listMask )
{
	if ( m_DirtyEntities.Count() )
	{
		CTSListWithFreeList<CBaseHandle>::Node_t *pCurrent, *pNext;
		while ( ( pCurrent = m_DirtyEntities.Detach() ) != NULL )
		{
			while ( pCurrent )
			{
				CBaseHandle handle = pCurrent->elem;
				pNext = (CTSListWithFreeList<CBaseHandle>::Node_t *)pCurrent->Next;
				m_DirtyEntities.FreeNode( pCurrent );
				pCurrent = pNext;

				CBaseEntity *pEntity = g_pGoldSRCServerGameEnts->GetEntityByHandle( handle );

				if ( pEntity )
				{
					pEntity->GetCCollideable()->UpdatePartition();
				}
			}
		}
	}
}


void CPartitionQueryCallback::OnPostQuery( SpatialPartitionListMask_t listMask )
{
}

#endif


void CCollideable::MarkDirty()
{
#if !defined( CLIENT_DLL )
	// Ignore world
	if ( m_pEntity->GetSourceEdict()->m_EdictIndex == 0 )
		return;

	s_PartitionQueryCallback.AddEntity( m_pEntity );
#endif
}
