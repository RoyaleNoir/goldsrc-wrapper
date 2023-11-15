#include "cbase.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"
#include "goldsrc_servergameents.h"
#include "goldsrc_globalvars.h"
#include "engine/IEngineTrace.h"
#include "goldsrc_trace.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineServer *engine;
extern IVModelInfo* modelinfo;
extern IEngineTrace *enginetrace;
extern CGlobalVars *gpGlobals;


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


//-----------------------------------------------------------------------------
// Purpose: Runs a single tick for this entity.
//-----------------------------------------------------------------------------
void CBaseEntity::RunTick()
{
	// Equivalent to SV_RunEntity() or CBaseEntity::PhysicsSimulate()

	EntVars()->ltime;

	switch( EntVars()->movetype )
	{
	case GoldSRC::MOVETYPE_PUSH:
	// SV_Physics_Pusher()
	{
		float oldltime = EntVars()->ltime;
		float movetime = gpGlobals->frametime;
		float thinktime = EntVars()->nextthink;

		// Don't overshoot
		if ( thinktime < EntVars()->ltime + gpGlobals->frametime )
		{
			movetime = MAX( thinktime - EntVars()->ltime, 0 );
		}

		if ( movetime )
		{
			if ( TryPush( movetime ) )
				EntVars()->ltime += movetime;
		}

		// Only think if the thinktime is within the movement this frame
		if ( thinktime > oldltime && thinktime <= EntVars()->ltime )
			Think( true );

		break;
	}
	case GoldSRC::MOVETYPE_NONE:
		// SV_Physics_None()
		{
			Think();
			break;
		}
	case GoldSRC::MOVETYPE_STEP:
		// SV_Physics_Step()
		{
			if ( !( EntVars()->flags & ( GoldSRC::G_FL_ONGROUND | GoldSRC::G_FL_FLY | GoldSRC::G_FL_SWIM ) ) )
			{
				// TODO: Falling
			}

			Think();
			//CheckWaterTransition()
			break;
		}
	default:
		DevMsg( "Unimplemented/unknown movetype %d\n", EntVars()->movetype );
		break;
	}

	UpdateFromEntVars();
}


bool CBaseEntity::TryPush( float movetime )
{
	// SV_Push()

	// Try angular movement first
	Vector angularmove;
	VectorScale( EntVars()->avelocity, movetime, angularmove.Base() );
	VectorAdd( EntVars()->angles, angularmove.Base(), EntVars()->angles );

	// Then try linear movement
	Vector linearmove;
	VectorScale( EntVars()->velocity, movetime, linearmove.Base() );
	VectorAdd( EntVars()->origin, linearmove.Base(), EntVars()->origin );

	return true;
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

	// TODO: Other playerstate stuff
	//VectorCopy( EntVars()->v_angle, angNewAngles.Base() );
	//m_PlayerState.v_angle = angNewAngles;
	//m_PlayerState.fixangle = EntVars()->fixangle;
	
	if ( EntVars()->fixangle )
	{
		VectorCopy( EntVars()->angles, EntVars()->v_angle );
		VectorCopy( EntVars()->angles, m_PlayerState.v_angle.Base() );
		m_PlayerState.fixangle = EntVars()->fixangle;
		EntVars()->fixangle = 0;
	}

	// TODO: Reset fixangle later on

	// Copy angles
	VectorCopy( EntVars()->view_ofs, vecNewVec.Base() );
	if ( vecNewVec != m_view_ofs )
		m_view_ofs = vecNewVec;

	// Update bounding box
	m_Collideable.SetMinMax( EntVars()->mins, EntVars()->maxs );
	m_Collideable.SetAbsMinMax( EntVars()->absmin, EntVars()->absmax );

	// TODO: Should the rest of the pev data just be sent via a datatable
}


CPlayerState *CBaseEntity::GetPlayerState()
{
	return &m_PlayerState;
}


void CBaseEntity::SetIsPlayer( bool bPlayer )
{
	m_bPlayer = bPlayer;
}


bool CBaseEntity::IsPlayer()
{
	return m_bPlayer;
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
// Purpose: Runs the think, and returns whether the entity has been removed.
//-----------------------------------------------------------------------------
bool CBaseEntity::Think( float useRealTime )
{
	// Equivalent to SV_RunThink()

	if ( GoldSrcEdict() )
	{
		if ( useRealTime )
		{
			GlobalVars()->realtime = gpGlobals->curtime;
		}
		else
		{
			int thinktime = EntVars()->nextthink;

			if ( thinktime <= 0 )
				return true;

			if ( thinktime > ( gpGlobals->curtime + gpGlobals->frametime ) )
				return true;

			if ( thinktime < gpGlobals->curtime)
				thinktime = gpGlobals->curtime;

			GlobalVars()->realtime = thinktime;
		}

		EntVars()->nextthink = 0;
		g_pGoldSRCEntityInterface->Think( GoldSrcEdict() );
		UpdateFromEntVars();

		// TODO: Check dead
	}

	return true;
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

	GoldSRC::TraceResult tr = UTIL_TraceEntityHull( 
		vecStart, 
		vecEnd, 
		m_Collideable.OBBMins(), 
		m_Collideable.OBBMaxs(), 
		this
	);

	if ( tr.flFraction == 1 || tr.fAllSolid )
		return 0;

	VectorCopy( tr.vecEndPos, EntVars()->origin );
	// Link()

	if ( GoldSrcEdict() )
	{
		EntVars()->flags |= GoldSRC::G_FL_ONGROUND;

		if ( tr.pHit )
		{
			EntVars()->groundentity = tr.pHit;
		}
	}

	UpdateFromEntVars();

	return 1;
}

#define STEPSIZE 18

int CBaseEntity::MoveStep( const Vector &move, bool relink )
{
	// SV_movestep

	// Save off bounds
	Vector vecMins;
	Vector vecMaxs;
	VectorCopy( EntVars()->mins, vecMins.Base() );
	VectorCopy( EntVars()->maxs, vecMaxs.Base() );

	Vector oldorg;
	Vector neworg;

	VectorCopy( EntVars()->origin, oldorg.Base() );
	VectorAdd( EntVars()->origin, move.Base(), neworg.Base() );

	// TODO: Flying/Swimming case

	// Try stepping up at most STEPSIZE units
	Vector traceEnd = neworg - Vector(0, 0, STEPSIZE);
	neworg.z += STEPSIZE;
	GoldSRC::TraceResult tr = UTIL_TraceEntityHull( neworg, traceEnd, vecMins, vecMaxs, this );

	if ( tr.fAllSolid )
		return false;

	// Try from the same height if it was too high
	if ( tr.fStartSolid )
	{
		neworg.z += STEPSIZE;
		tr = UTIL_TraceEntityHull( neworg, traceEnd, vecMins, vecMaxs, this );
		if ( tr.fAllSolid || tr.fStartSolid )
			return 0;
	}

	if ( tr.flFraction == 1 )
	{
		if ( EntVars()->flags & GoldSRC::G_FL_PARTIALGROUND )
		{
			VectorAdd( EntVars()->origin, move.Base(), EntVars()->origin);
			// if ( relink )
			//	Link( true );
			EntVars()->flags &= ~GoldSRC::G_FL_ONGROUND;
			UpdateFromEntVars();
			return 1;
		}

		return 0;
	}

	VectorCopy( tr.vecEndPos, EntVars()->origin );

	// TODO: SV_CheckBottom()

	EntVars()->flags &= ~GoldSRC::G_FL_PARTIALGROUND;
	EntVars()->groundentity = tr.pHit;

	// if ( relink )
	//	Link( true );

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
