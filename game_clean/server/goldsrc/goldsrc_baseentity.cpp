#include "cbase.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"
#include "goldsrc_servergameents.h"
#include "goldsrc_globalvars.h"
#include "engine/IEngineTrace.h"
#include "goldsrc_trace.h"
#include "model_types.h"
#include "datacache/imdlcache.h"
#include "tier3/tier3.h"
#include "studio.h"

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
	SendPropVector( SENDINFO( m_velocity ), -1, SPROP_NOSCALE | SPROP_CHANGES_OFTEN, 0.0f, HIGH_DEFAULT ),
	SendPropDataTable( SENDINFO_DT( m_Collideable ), &REFERENCE_SEND_TABLE( DT_Collideable ) ),

	SendPropFloat( SENDINFO( m_renderamt ) ),
	SendPropInt( SENDINFO( m_rendermode ) ),
END_SEND_TABLE()


CBaseEntity::CBaseEntity()
{
	m_Collideable.Init( this );
	m_Networkable.Init( this );
	m_RefEHandle.Term();

	m_pGoldSrcEdict = NULL;
	m_szDebugClassName = NULL;

	m_nModelIndex = 0;
	m_pGoldSRCModel = NULL;

	m_renderamt = 255.0f;
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
				AddGravity( 1.0f );
				CheckVelocity();

				Relink( true );
			}

			Think();
			//CheckWaterTransition()
			break;
		}
	case GoldSRC::MOVETYPE_TOSS:
	case GoldSRC::MOVETYPE_BOUNCE:
	case GoldSRC::MOVETYPE_BOUNCEMISSILE:
	case GoldSRC::MOVETYPE_FLY:
	case GoldSRC::MOVETYPE_FLYMISSILE:
		// SV_Physics_Toss()
		{
			if ( !Think() )
				break;

			if ( EntVars()->velocity[2] > 0 )
				EntVars()->flags &= ~GoldSRC::G_FL_ONGROUND;

			if ( EntVars()->flags & ( GoldSRC::G_FL_ONGROUND ) )
				break;

			CheckVelocity();

			if ( EntVars()->movetype != GoldSRC::MOVETYPE_FLY && EntVars()->movetype != GoldSRC::MOVETYPE_FLYMISSILE )
			{
				AddGravity( 1.0f );
			}

			// Angular move
			VectorMA( EntVars()->angles, gpGlobals->frametime, EntVars()->avelocity, EntVars()->angles );

			// Linear move
			Vector move;
			VectorScale( EntVars()->velocity, gpGlobals->frametime, move.Base() );
			GoldSRC::TraceResult tr = TryMove( move );

			if ( tr.flFraction == 1 )
				break;

			bool bounce = EntVars()->movetype == GoldSRC::MOVETYPE_BOUNCE || EntVars()->movetype == GoldSRC::MOVETYPE_BOUNCEMISSILE;

			ClipVelocity( EntVars()->velocity, tr.vecPlaneNormal, EntVars()->velocity, bounce ? 1.5f : 1.0f );

			if ( tr.vecPlaneNormal[2] > 0.7 )
			{
				if ( EntVars()->velocity[2] < 60 || !bounce )
				{
					EntVars()->flags |= GoldSRC::G_FL_ONGROUND;
					EntVars()->groundentity = tr.pHit;
					VectorCopy( vec3_origin.Base(), EntVars()->velocity );
					VectorCopy( vec3_origin.Base(), EntVars()->avelocity );
				}
			}

			// TODO: SV_CheckWaterTransition
		}
	default:
		//DevMsg( "Unimplemented/unknown movetype %d\n", EntVars()->movetype );
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


GoldSRC::TraceResult CBaseEntity::TryMove( Vector &move )
{
	// SV_PushEntity

	Vector vecStart;
	VectorCopy( EntVars()->origin, vecStart.Base() );
	Vector vecEnd = vecStart + move;

	Vector vecMins;
	Vector vecMaxs;
	VectorCopy( EntVars()->mins, vecMins.Base() );
	VectorCopy( EntVars()->maxs, vecMaxs.Base() );

	int mask = 0;
	if ( EntVars()->movetype == GoldSRC::MOVETYPE_FLYMISSILE || EntVars()->movetype == GoldSRC::MOVETYPE_BOUNCEMISSILE )
	{
		mask = 2;
	}
	else if ( EntVars()->solid == GoldSRC::SOLID_TRIGGER || EntVars()->solid == GoldSRC::SOLID_NOT )
	{
		mask = 1;
	}

	GoldSRC::TraceResult tr = UTIL_TraceEntityHull( vecStart, vecEnd, vecMins, vecMaxs, this, mask );

	VectorCopy( tr.vecEndPos, EntVars()->origin );
	Relink( true );

	if ( tr.pHit )
	{
		CBaseEntity *pOther = g_pGoldSRCEdict->GetSourceEntity( tr.pHit );
		Impact( pOther );
	}

	return tr;
}


void CBaseEntity::AddGravity( float scale )
{
	ConVarRef sv_gravity( "sv_gravity" );

	EntVars()->velocity[2] -= scale * sv_gravity.GetFloat() * gpGlobals->frametime;
}


void CBaseEntity::CheckVelocity()
{
	ConVarRef sv_maxvelocity( "sv_maxvelocity" );

	for ( int i = 0; i < 3; i++ )
	{
		float v = EntVars()->velocity[i];

		if ( isnan( v ) )
			v = 0;

		if ( fabs( v ) > sv_maxvelocity.GetFloat() )
		{
			v = Sign( v ) * sv_maxvelocity.GetFloat();
		}

		EntVars()->velocity[i] = v;
	}
}


void CBaseEntity::ClipVelocity( GoldSRC::vec3_t inVel, GoldSRC::vec3_t normal, GoldSRC::vec3_t outVel, float overbounce )
{
	// Slide
	float normalFac = DotProduct( inVel, normal ) * overbounce;
	VectorMA( inVel, -normalFac, normal, outVel);

	// Epsilon
	for ( int i = 0; i < 3; i++ )
	{
		if ( fabs( outVel[i] ) < 0.1 )
		{
			outVel[i] = 0;
		}
	}
}


void CBaseEntity::Impact( CBaseEntity *pOther )
{
	if ( !pOther )
		return;

	if ( EntVars()->solid != GoldSRC::SOLID_NOT )
	{
		Touch( pOther );
	}

	if ( pOther->EntVars()->solid != GoldSRC::SOLID_NOT )
	{
		pOther->Touch( this );
	}
}


void CBaseEntity::Relink( bool touchTriggers )
{
	//Msg( "Relink( %d ) %s\n", touchTriggers, m_szDebugClassName );
	// TODO: Area

	// Don't add world
	if ( GetSourceEdict()->m_EdictIndex == 0 )
		return;

	VectorAdd( EntVars()->origin, EntVars()->mins, EntVars()->absmin );
	VectorAdd( EntVars()->origin, EntVars()->maxs, EntVars()->absmax );

	// TODO: PVS

	// TODO: Links

	UpdateFromEntVars();

	if ( touchTriggers )
	{
		// Don't do anything with non-solid ents
		if ( EntVars()->solid == GoldSRC::SOLID_NOT )
			return;

		if ( EntVars()->solid == GoldSRC::SOLID_TRIGGER )
			engine->TriggerMoved( GetSourceEdict(), true );
		else
			engine->SolidMoved( GetSourceEdict(), GetCollideable(), NULL, true );
	}
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
		SetModelIndex( EntVars()->modelindex );
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

	VectorCopy( EntVars()->velocity, vecNewVec.Base() );
	if ( vecNewVec != m_velocity )
		m_velocity = vecNewVec;

	// Update bounding box
	m_Collideable.SetMinMax( EntVars()->mins, EntVars()->maxs );
	m_Collideable.SetAbsMinMax( EntVars()->absmin, EntVars()->absmax );
	m_Collideable.SetSolidVars( EntVars()->solid );

	// TODO: Should the rest of the pev data just be sent via a datatable

	// Render
	if ( EntVars()->renderamt != m_renderamt )
		m_renderamt = EntVars()->renderamt;
	if ( EntVars()->rendermode != m_rendermode )
		m_rendermode = EntVars()->rendermode;
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
			float thinktime = EntVars()->nextthink;

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
		if ( !GoldSrcEdict() )
			return false;
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
// Purpose: Touched
//-----------------------------------------------------------------------------
void CBaseEntity::Touch( CBaseEntity *pOtherEnt )
{
	if ( !GoldSrcEdict() )
		return;
	if ( !pOtherEnt || !pOtherEnt->GoldSrcEdict() )
		return;

	GlobalVars()->realtime = gpGlobals->curtime;
	g_pGoldSRCEntityInterface->Touch( GoldSrcEdict(), pOtherEnt->GoldSrcEdict() );
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
		Relink( false );
	}
}


//-----------------------------------------------------------------------------
// Purpose: Sets the origin, and updates the pev
//-----------------------------------------------------------------------------
void CBaseEntity::SetSize( const float *min, const float *max )
{
	if ( !GoldSrcEdict() )
		return;

	// TODO: Handle rotation

	VectorCopy( min, EntVars()->mins );
	VectorCopy( max, EntVars()->maxs );
	VectorSubtract( max, min, EntVars()->size );

	VectorAdd( min, EntVars()->origin, EntVars()->absmin );
	VectorAdd( min, EntVars()->origin, EntVars()->absmin );

	UpdateFromEntVars();

	Relink( false );
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
	if ( index != 0 )
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
	Relink( false );

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
			
			if ( relink )
				Relink( true );

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

	if ( relink )
		Relink( true );

	UpdateFromEntVars();
	return 1;
}


bool CBaseEntity::StepDirection( float yaw, float dist )
{
	// Move towards the direction
	EntVars()->ideal_yaw = yaw;
	ChangeYaw();

	yaw = DEG2RAD( yaw );
	Vector move( cos( yaw ) * dist, sin( yaw ) * dist, 0 );

	GoldSRC::vec3_t oldorigin;
	VectorCopy( EntVars()->origin, oldorigin );

	if ( MoveStep( move, false ) )
	{
		float delta = EntVars()->angles[YAW] - EntVars()->ideal_yaw;
		if ( delta > 45 && delta < 315 )
		{
			VectorCopy( oldorigin, EntVars()->origin );
		}

		Relink( true );
		UpdateFromEntVars();
		return true;
	}

	Relink( true );

	UpdateFromEntVars();
	return false;
}


void CBaseEntity::NewChaseDir( const float *pflGoal, float dist )
{
	// SV_NewChaseDir()

	float olddir = anglemod( (int)( EntVars()->ideal_yaw / 45 ) * 45 );
	float turnaround = anglemod( olddir - 180 );

	float deltax = pflGoal[0] - EntVars()->origin[0];
	float deltay = pflGoal[1] - EntVars()->origin[1];

	float d[3];

	if (deltax > 10)
		d[1] = 0;
	else if (deltax < -10)
		d[1] = 180;
	else
		d[1] = -1;

	if ( deltay < -10 )
		d[2] = 270;
	else if ( deltay > 10 )
		d[2] = 90;
	else
		d[2] = -1;

	float turn = 0;

	// More than 10 away in both axes, try direct route
	if ( d[1] != -1 && d[2] != -1 )
	{
		if ( d[1] == 0 )
			turn = d[2] == 90 ? 45 : 315;	// > 10 units in x
		else
			turn = d[2] == 90 ? 135 : 215;	// < -10 units in x

		// Try turning if it won't be a 180 degree turn
		if ( turn != turnaround && StepDirection( turn, dist ) )
			return;
	}

	if ( ( ( rand() & 3 ) & 1 ) || abs( deltay > deltax ) )
	{
		turn = d[1];
		d[1] = d[2];
		d[2] = turn;
	}

	if ( d[1] != -1 && d[1] != turnaround && StepDirection( d[1], dist) )
		return;

	if ( d[2] != -1 && d[2] != turnaround && StepDirection( d[2], dist) )
		return;

	if ( StepDirection( olddir, dist ) )
		return;

	// try in increments of 45 degrees in a random direction
	if ( rand() & 1 )
	{
		for( turn = 0; turn <= 315; turn += 45 )
		{
			if ( turn != turnaround && StepDirection( turn, dist ) )
				return;
		}
	}
	else
	{
		for( turn = 315; turn >= 0; turn -= 45 )
		{
			if ( turn != turnaround && StepDirection( turn, dist ) )
				return;
		}
	}

	// Lastly just try turning 180
	if ( StepDirection( turnaround, dist ) )
		return;

	EntVars()->ideal_yaw = olddir;

	// TODO: SV_CheckBottom()
}


void CBaseEntity::MoveToGoal( const float *pflGoal, float dist, int iMovetype )
{
	// SV_MoveToGoal

	if ( !( EntVars()->flags & ( GoldSRC::G_FL_ONGROUND | GoldSRC::G_FL_FLY | GoldSRC::G_FL_SWIM ) ) )
	{
		return;
	}

	// Can only handle MOVE_NORMAL right now
	if ( iMovetype != 0 /*MOVE_NORMAL*/)
	{
		LOG_STUB();
		return;
	}

	// SV_CloseEnough()

	// Why is there a rand here?
	if ( ( rand() & 3 ) == 1 || !StepDirection( EntVars()->ideal_yaw, dist ) )
	{
		NewChaseDir( pflGoal, dist );
	}
}


void CBaseEntity::ChangeYaw()
{
	float current = anglemod( EntVars()->angles[1] );
	float ideal = EntVars()->ideal_yaw;
	float speed = EntVars()->yaw_speed;

	if ( current == ideal )
		return;

	float move = ideal - current;

	// Find the nearest direction
	if ( ideal > current && move >= 180 )
		move -= 360;
	else if ( ideal < current && move <= -180 )
		move += 360;

	// Clamp to speed
	move = clamp( move, -speed, speed );

	EntVars()->angles[1] = anglemod( current + move );
	UpdateFromEntVars();
}


void *CBaseEntity::GetModelPtr()
{
	return m_pGoldSRCModel;
}


void CBaseEntity::GetBonePosition( int iBone, float *rgflOrigin, float *rgflAngles )
{
	// TODO: Animation
	if ( iBone != 0 )
		LOG_STUB();

	VectorCopy( EntVars()->origin, rgflOrigin );
	VectorCopy( EntVars()->angles, rgflAngles );
}


const model_t *CBaseEntity::GetSourceModel()
{
	// TODO: Cache
	return modelinfo->GetModel( m_nModelIndex );
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
	return m_nModelIndex;
}


string_t CBaseEntity::GetModelName( void ) const
{
	return NULL_STRING;
}


void CBaseEntity::SetModelIndex( int index )
{
	if ( index != m_nModelIndex && m_pGoldSRCModel )
	{
		UnwrapStudioModel( m_pGoldSRCModel );
		m_pGoldSRCModel = NULL;
	}

	m_nModelIndex = index;

	if ( m_nModelIndex == 0 )
		return;

	const model_t *pModel = modelinfo->GetModel( m_nModelIndex );

	if ( modelinfo->GetModelType( pModel ) != mod_studio )
		return;

	MDLHandle_t hStudioHdr = modelinfo->GetCacheHandle( pModel );
	if ( hStudioHdr == MDLHANDLE_INVALID )
		return;

	const studiohdr_t *pStudioHdr = mdlcache->LockStudioHdr( hStudioHdr );

	if ( pStudioHdr )
	{
		CStudioHdr studioHdr( pStudioHdr, mdlcache );
		m_pGoldSRCModel = WrapStudioModel( &studioHdr );
	}

	mdlcache->UnlockStudioHdr( hStudioHdr );
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
