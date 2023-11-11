#include "cbase.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"
#include "goldsrc_servergameents.h"
#include "engine/IEngineTrace.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineServer *engine;
extern IVModelInfo* modelinfo;
extern IEngineTrace *enginetrace;


IMPLEMENT_SERVERCLASS_ST_NOBASE( CBaseEntity, DT_BaseEntity )
	SendPropModelIndex( SENDINFO( m_nModelIndex ) ),
	SendPropVector( SENDINFO( m_origin ), -1, SPROP_NOSCALE | SPROP_CHANGES_OFTEN, 0.0f, HIGH_DEFAULT ),
	SendPropQAngles( SENDINFO( m_angles ), -1, SPROP_CHANGES_OFTEN ),
	SendPropVector( SENDINFO( m_view_ofs ), -1, SPROP_NOSCALE | SPROP_CHANGES_OFTEN, 0.0f, HIGH_DEFAULT ),
	SendPropDataTable( SENDINFO_DT( m_Collideable ), &REFERENCE_SEND_TABLE( DT_Collideable ) ),
END_SEND_TABLE()


CBaseEntity::CBaseEntity()
{
	m_Collideable.Init( this );
	m_Networkable.Init( this );
	m_RefEHandle.Term();

	m_pGoldSrcEdict = NULL;
	m_szDebugClassName = NULL;

	m_nModelIndex = 0;
}


CBaseEntity::~CBaseEntity()
{
	if ( GoldSrcEdict() )
		DetachGoldSRCEdict();

	g_pGoldSRCServerGameEnts->RemoveEntityByHandle( GetRefEHandle() );
}


//-----------------------------------------------------------------------------
// Purpose: Gets the entvars
//-----------------------------------------------------------------------------
GoldSRC::entvars_t *CBaseEntity::EntVars()
{
	if ( !GoldSrcEdict() )
		return NULL;

	return &GoldSrcEdict()->v;
}


//-----------------------------------------------------------------------------
// Purpose: Called after the entity class has been created in the GoldSRC dll
//-----------------------------------------------------------------------------
void CBaseEntity::PostGoldSrcCreate()
{
	if ( !GoldSrcEdict() )
		return;

	m_szDebugClassName = GoldSRC::SzFromIndex( EntVars()->classname );
}


//-----------------------------------------------------------------------------
// Purpose: Called after Spawn() has been called in the GoldSRC dll
//-----------------------------------------------------------------------------
void CBaseEntity::PostGoldSrcSpawn()
{
	if ( !GoldSrcEdict() )
		return;

	m_szDebugClassName = GoldSRC::SzFromIndex( EntVars()->classname );

	UpdateFromEntVars();
}


void CBaseEntity::SetClassName( GoldSRC::string_t iClassName )
{
	m_iClassName = iClassName;
}


GoldSRC::string_t CBaseEntity::GetClassName()
{
	return m_iClassName;
}


//-----------------------------------------------------------------------------
// Purpose: Creates and attaches both the Source and GoldSrc edicts
// Input  : *pOverrideEdict - The Source edict to force the entity to use
//-----------------------------------------------------------------------------
void CBaseEntity::AttachEdict( edict_t *pOverrideEdict )
{
	m_Networkable.AttachEdict( pOverrideEdict );

	g_pGoldSRCServerGameEnts->AddEntity( this, m_Networkable.GetEdict()->m_EdictIndex );

	// Get and attach the goldsrc edict
	edict_t *pSourceEdict = m_Networkable.GetEdict();
	m_pGoldSrcEdict = g_pGoldSRCEdict->GetGoldSRCEdict( pSourceEdict, true );
}


//-----------------------------------------------------------------------------
// Purpose: Detaches the GoldSrc edict
//-----------------------------------------------------------------------------
void CBaseEntity::DetachGoldSRCEdict()
{
	g_pGoldSRCEdict->DestroyGoldSRCEdict( GoldSrcEdict() );
	m_pGoldSrcEdict = NULL;
}


//-----------------------------------------------------------------------------
// Purpose: Gets the Source edict
//-----------------------------------------------------------------------------
edict_t *CBaseEntity::GetSourceEdict() const
{
	return m_Networkable.GetEdict();
}


//-----------------------------------------------------------------------------
// Purpose: Gets the GoldSRC edict
//-----------------------------------------------------------------------------
GoldSRC::edict_t *CBaseEntity::GoldSrcEdict() const
{
	return m_pGoldSrcEdict;
}


const Vector &CBaseEntity::GetAbsOrigin() const
{
	return m_origin;
}


const QAngle &CBaseEntity::GetAbsAngles() const
{
	return m_angles;
}


//-----------------------------------------------------------------------------
// Purpose: Updates the local variables from the pev
//-----------------------------------------------------------------------------
void CBaseEntity::UpdateFromEntVars()
{
	if ( !GoldSrcEdict() )
		return;

	// Update model
	if ( m_nModelIndex != EntVars()->modelindex )
	{
		m_nModelIndex = EntVars()->modelindex;
	}

	// Copy origin
	Vector vecNewVec;
	VectorCopy( EntVars()->origin, vecNewVec.Base() );
	if ( vecNewVec != m_origin )
		m_origin = vecNewVec;

	// Copy angles
	QAngle angNewAngles;
	VectorCopy( EntVars()->angles, angNewAngles.Base() );
	if ( angNewAngles != m_angles )
		m_angles = angNewAngles;

	// Copy angles
	VectorCopy( EntVars()->view_ofs, vecNewVec.Base() );
	if ( vecNewVec != m_view_ofs )
		m_view_ofs = vecNewVec;

	// Update bounding box
	m_Collideable.SetMinMax( EntVars()->mins, EntVars()->maxs );
	m_Collideable.SetAbsMinMax( EntVars()->absmin, EntVars()->absmax );

	// TODO: Should the rest of the pev data just be sent via a datatable
}


//-----------------------------------------------------------------------------
// Purpose: Spawns the entity in the world
//-----------------------------------------------------------------------------
void CBaseEntity::Spawn()
{
	if ( GoldSrcEdict() )
	{
		g_pGoldSRCEntityInterface->Spawn( GoldSrcEdict() );
		PostGoldSrcSpawn();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Update
//-----------------------------------------------------------------------------
void CBaseEntity::Think()
{
	if ( GoldSrcEdict() )
	{
		UpdateFromEntVars();
		g_pGoldSRCEntityInterface->Think( GoldSrcEdict() );
		UpdateFromEntVars();
	}
}


//-----------------------------------------------------------------------------
// Purpose: Update
//-----------------------------------------------------------------------------
void CBaseEntity::Use( CBaseEntity *pOtherEnt )
{
	if ( !GoldSrcEdict() )
		return;

	UpdateFromEntVars();

	if ( !pOtherEnt || !pOtherEnt->GoldSrcEdict() )
		return;

	g_pGoldSRCEntityInterface->Use( GoldSrcEdict(), pOtherEnt->GoldSrcEdict() );
	UpdateFromEntVars();
	pOtherEnt->UpdateFromEntVars();
}


//-----------------------------------------------------------------------------
// Purpose: Sets the origin, and updates the pev
//-----------------------------------------------------------------------------
void CBaseEntity::SetOrigin( const float *flOrigin )
{
	if ( !flOrigin )
		return;

	Vector vecNewOrigin;
	VectorCopy( flOrigin, vecNewOrigin.Base() );
	if ( vecNewOrigin != m_origin )
		m_origin = vecNewOrigin;

	if ( GoldSrcEdict() )
	{
		VectorCopy( flOrigin, EntVars()->origin );
		g_pGoldSRCEdict->LinkEdict( GoldSrcEdict(), false );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Sets the origin, and updates the pev
//-----------------------------------------------------------------------------
void CBaseEntity::SetSize( const float *min, const float *max )
{
	if ( !GoldSrcEdict() )
		return;

	UpdateFromEntVars();

	// TODO: Handle rotation

	VectorCopy( min, EntVars()->mins );
	VectorCopy( max, EntVars()->maxs );
	VectorSubtract( max, min, EntVars()->size );

	VectorAdd( min, EntVars()->origin, EntVars()->absmin );
	VectorAdd( min, EntVars()->origin, EntVars()->absmin );

	m_Collideable.SetMinMax( EntVars()->mins, EntVars()->maxs );
	m_Collideable.SetAbsMinMax( EntVars()->absmin, EntVars()->absmax );
	m_Collideable.SetSolidVars( EntVars()->solid );

	g_pGoldSRCEdict->LinkEdict( GoldSrcEdict(), false);
}


//-----------------------------------------------------------------------------
// Purpose: Sets the model and entity bounds
//-----------------------------------------------------------------------------
void CBaseEntity::SetModel( const char *szModelName )
{
	if ( !szModelName )
		return;

	if ( !GoldSrcEdict() )
		return;

	int index = modelinfo->GetModelIndex( szModelName );

	// Found an index
	if ( index != -1 )
	{
		const model_t *pModel = modelinfo->GetModel( index );

		// Model actually loaded
		if ( pModel )
		{
			EntVars()->model = szModelName - GoldSRC::SzFromIndex( 0 );
			EntVars()->modelindex = index;

			Vector vMins, vMaxs;
			modelinfo->GetModelBounds( pModel, vMins, vMaxs );

			SetSize( vMins.Base(), vMaxs.Base() );
			UpdateFromEntVars();
			return;
		}
	}

	// Failed, just set defaults
	EntVars()->model = NULL;
	EntVars()->modelindex = -1;
	SetSize( vec3_origin.Base(), vec3_origin.Base() );
	UpdateFromEntVars();
}


//-----------------------------------------------------------------------------
// Purpose: Drops the entity to the floor
//-----------------------------------------------------------------------------
int CBaseEntity::DropToFloor()
{
	UpdateFromEntVars();

	Vector vecStart = GetAbsOrigin();
	Vector vecEnd = vecStart - Vector( 0, 0, 256 );

	CTraceFilterHitAll filter;
	trace_t tr;

	enginetrace->SweepCollideable(
		GetCollideable(),
		vecStart,
		vecEnd,
		vec3_angle,
		MASK_SOLID,
		&filter,
		&tr
	);

	if ( tr.fraction == 1 || tr.allsolid )
		return 0;

	SetOrigin( tr.endpos.Base() );

	if ( GoldSrcEdict() )
	{
		EntVars()->flags |= GoldSRC::G_FL_ONGROUND;

		if ( tr.m_pEnt )
		{
			EntVars()->groundentity = tr.m_pEnt->GoldSrcEdict();
		}
	}

	UpdateFromEntVars();

	return 1;
}


void CBaseEntity::SetRefEHandle( const CBaseHandle &handle )
{
	m_RefEHandle = handle;

	if ( m_Networkable.GetEdict() )
	{
		m_Networkable.GetEdict()->m_NetworkSerialNumber = handle.GetSerialNumber();
	}
}


const CBaseHandle &CBaseEntity::GetRefEHandle() const
{
	return m_RefEHandle;
}


ICollideable *CBaseEntity::GetCollideable()
{
	return &m_Collideable;
}


IServerNetworkable *CBaseEntity::GetNetworkable()
{
	return &m_Networkable;
}


CBaseEntity *CBaseEntity::GetBaseEntity()
{
	return this;
}


int CBaseEntity::GetModelIndex( void ) const
{
	return -1;
}


string_t CBaseEntity::GetModelName( void ) const
{
	return NULL_STRING;
}


void CBaseEntity::SetModelIndex( int index )
{
}


#include "tier0/memdbgoff.h"

void *CBaseEntity::operator new( size_t stAllocateBlock )
{
	return engine->PvAllocEntPrivateData( stAllocateBlock );
}


void *CBaseEntity::operator new( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine )
{
	return engine->PvAllocEntPrivateData( stAllocateBlock );
}


void CBaseEntity::operator delete( void *pMem )
{
	engine->FreeEntPrivateData( pMem );
}


void CBaseEntity::operator delete( void *pMem, int nBlockUse, const char *pFileName, int inLine )
{
	operator delete( pMem );
}


void CBaseEntity::NetworkStateChanged()
{
	if ( m_Networkable.GetEdict() )
		m_Networkable.GetEdict()->StateChanged();
}


void CBaseEntity::NetworkStateChanged( void *pVar )
{
	if ( m_Networkable.GetEdict() )
		m_Networkable.GetEdict()->StateChanged( (char *)pVar - (char *)this );
}


#include "tier0/memdbgon.h"


//=============================================================================


CServerNetworkable::CServerNetworkable()
{
	Init( NULL );
}


CServerNetworkable::~CServerNetworkable()
{
	DetachEdict();
}


void CServerNetworkable::Init( CBaseEntity *pEntity )
{
	m_pEntity = pEntity;
}


void CServerNetworkable::AttachEdict( edict_t *pEdict )
{
	if ( !pEdict )
	{
		pEdict = engine->CreateEdict();
	}

	m_pEdict = pEdict;
	m_pEdict->SetEdict( GetBaseEntity(), true );
	m_pEdict->m_pNetworkable = this;
}


void CServerNetworkable::DetachEdict()
{
	if ( m_pEdict )
	{
		m_pEdict->SetEdict( NULL, false );
		engine->RemoveEdict( m_pEdict );
		m_pEdict = NULL;
	}
}


IHandleEntity *CServerNetworkable::GetEntityHandle()
{
	return m_pEntity;
}


ServerClass *CServerNetworkable::GetServerClass()
{
	if ( !m_pEntity )
		return NULL;
	return m_pEntity->GetServerClass();
}


edict_t *CServerNetworkable::GetEdict() const
{
	return m_pEdict;
}


const char *CServerNetworkable::GetClassName() const
{
	if ( !m_pEntity )
		return GoldSRC::SzFromIndex( NULL );

	return GoldSRC::SzFromIndex( m_pEntity->GetClassName() );
}


void CServerNetworkable::Release()
{
	if ( m_pEntity )
	{
		m_pEntity->DetachGoldSRCEdict();
	}

	delete m_pEntity;
}


int CServerNetworkable::AreaNum() const
{
	return 0;
}


CBaseNetworkable *CServerNetworkable::GetBaseNetworkable()
{
	return NULL;	// What is this?
}


CBaseEntity *CServerNetworkable::GetBaseEntity()
{
	return m_pEntity;
}


PVSInfo_t *CServerNetworkable::GetPVSInfo()
{
	return nullptr;
}
