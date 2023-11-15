#include "cbase.h"
#include "goldsrc_playermove_server.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_eiface.h"
#include "goldsrc_globalvars.h"
#include "goldsrc_servergameents.h"
#include "filesystem.h"
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern CGlobalVars *gpGlobals;
extern IEngineSound *enginesound;
extern IEngineTrace *enginetrace;
extern IFileSystem *filesystem;


static CGoldSRCPlayerMove g_GoldSRCPlayerMove;
CGoldSRCPlayerMove *g_pGoldSRCPlayerMove = &g_GoldSRCPlayerMove;


// TODO: Cvars!
static GoldSRC::movevars_t g_movevars = {
	800,		//gravity;
	100,		//stopspeed;
	320,		//maxspeed;
	500,		//spectatormaxspeed;
	10,			//accelerate;
	10,			//airaccelerate;
	10,			//wateraccelerate;
	4,			//friction;
	2,			//edgefriction;
	1,			//waterfriction;
	1,			//entgravity;
	1,			//bounce;
	18,			//stepsize;
	2000,		//maxvelocity;
	4096,		//zmax;
	0,			//waveHeight;
	1,			//footsteps;
	"desert",	//skyName[32];
	2,			//rollangle;
	200,		//rollspeed;
	0,			//skycolor_r;
	0,			//skycolor_g;
	0,			//skycolor_b;
	0,			//skyvec_x;
	0,			//skyvec_y;
	0,			//skyvec_z;
};

#undef LOG_STUB
#define LOG_STUB()


CGoldSRCPlayerMove::CGoldSRCPlayerMove()
{
	m_playerMove.movevars = &g_movevars;

	// Set function pointers
	m_playerMove.pfnPM_Info_ValueForKey = &CGoldSRCPlayerMove::PM_Info_ValueForKey;
	m_playerMove.pfnPM_Particle = &CGoldSRCPlayerMove::PM_Particle;
	m_playerMove.pfnPM_TestPlayerPosition = &CGoldSRCPlayerMove::PM_TestPlayerPosition;
	m_playerMove.pfnCon_NPrintf = &CGoldSRCPlayerMove::Con_NPrintf;
	m_playerMove.pfnCon_DPrintf = &CGoldSRCPlayerMove::Con_DPrintf;
	m_playerMove.pfnCon_Printf = &CGoldSRCPlayerMove::Con_Printf;
	m_playerMove.pfnSys_FloatTime = &CGoldSRCPlayerMove::PM_Sys_FloatTime;
	m_playerMove.pfnPM_StuckTouch = &CGoldSRCPlayerMove::PM_StuckTouch;
	m_playerMove.pfnPM_PointContents = &CGoldSRCPlayerMove::PM_PointContents;
	m_playerMove.pfnPM_TruePointContents = &CGoldSRCPlayerMove::PM_TruePointContents;
	m_playerMove.pfnPM_HullPointContents = &CGoldSRCPlayerMove::PM_HullPointContents;
	m_playerMove.pfnPM_PlayerTrace = &CGoldSRCPlayerMove::PM_PlayerTrace;
	m_playerMove.pfnPM_TraceLine = &CGoldSRCPlayerMove::PM_TraceLine;
	m_playerMove.pfnRandomLong = &CGoldSRCPlayerMove::RandomLong;
	m_playerMove.pfnRandomFloat = &CGoldSRCPlayerMove::RandomFloat;
	m_playerMove.pfnPM_GetModelType = &CGoldSRCPlayerMove::PM_GetModelType;
	m_playerMove.pfnPM_GetModelBounds = &CGoldSRCPlayerMove::PM_GetModelBounds;
	m_playerMove.pfnPM_HullForBsp = &CGoldSRCPlayerMove::PM_HullForBsp;
	m_playerMove.pfnPM_TraceModel = &CGoldSRCPlayerMove::PM_TraceModel;
	m_playerMove.pfnCOM_FileSize = &CGoldSRCPlayerMove::COM_FileSize;
	m_playerMove.pfnCOM_LoadFile = &CGoldSRCPlayerMove::COM_LoadFile;
	m_playerMove.pfnCOM_FreeFile = &CGoldSRCPlayerMove::COM_FreeFile;
	m_playerMove.pfnmemfgets = &CGoldSRCPlayerMove::memfgets;
	m_playerMove.pfnPM_PlaySound = &CGoldSRCPlayerMove::PM_PlaySound;
	m_playerMove.pfnPM_TraceTexture = &CGoldSRCPlayerMove::PM_TraceTexture;
	m_playerMove.pfnPM_PlaybackEventFull = &CGoldSRCPlayerMove::PM_PlaybackEventFull;
	m_playerMove.pfnPM_PlayerTraceEx = &CGoldSRCPlayerMove::PM_PlayerTraceEx;
	m_playerMove.pfnPM_TestPlayerPositionEx = &CGoldSRCPlayerMove::PM_TestPlayerPositionEx;
	m_playerMove.pfnPM_TraceLineEx = &CGoldSRCPlayerMove::PM_TraceLineEx;
}


CGoldSRCPlayerMove::~CGoldSRCPlayerMove()
{
}


void CGoldSRCPlayerMove::Init()
{
	// Set initial values
	m_playerMove.server = true;
	m_playerMove.runfuncs = false;

	//m_playerMove.movevars = NULL;

	// Standing hull
	m_playerMove.player_mins[0][0] = -16;
	m_playerMove.player_mins[0][1] = -16;
	m_playerMove.player_mins[0][2] = -36;
	m_playerMove.player_maxs[0][0] =  16;
	m_playerMove.player_maxs[0][1] =  16;
	m_playerMove.player_maxs[0][2] =  36;

	// Crouching hull
	m_playerMove.player_mins[1][0] = -16;
	m_playerMove.player_mins[1][1] = -16;
	m_playerMove.player_mins[1][2] = -18;
	m_playerMove.player_maxs[1][0] =  16;
	m_playerMove.player_maxs[1][1] =  16;
	m_playerMove.player_maxs[1][2] =  18;

	// Copy hulls
	for ( int i = 0; i < 4; i++ )
	{
		// This doesn't work
		g_pGoldSRCEntityInterface->pfnGetHullBounds( i, m_playerMove.player_mins[i], m_playerMove.player_maxs[i] );
	}

	g_pGoldSRCEntityInterface->pfnPM_Init( &m_playerMove );
}


void CGoldSRCPlayerMove::ProcessUsercmds( CBaseEntity *pEntity, CUserCmdList *cmdlist, int numcmds, int totalcmds, int dropped_packets, bool paused )
{
	// TODO: Run any dropped commands

	// Run new commands from oldest to newest
	for ( int i = numcmds - 1; i >= 0; i-- )
	{
		CUserCmdList *pList = cmdlist;

		for ( int j = 0; j < pList->m_nNumCmds; j++ )
		{
			RunUserCmd( pEntity, &pList->m_Cmds[j] );
		}
	}
}

void CGoldSRCPlayerMove::RunUserCmd( CBaseEntity *pEntity, GoldSRC::usercmd_t *pCmd )
{
	// Save off command
	m_cmd = *pCmd;

	// Split the command if it's more than 50ms
	if ( m_cmd.msec > 50 )
	{
		int oldmsec = pCmd->msec;
		m_cmd.msec = oldmsec / 2;
		RunUserCmd( pEntity, &m_cmd );
		m_cmd.msec = oldmsec / 2;
		RunUserCmd( pEntity, &m_cmd );
		return;
	}

	// TODO: Random seed
	g_pGoldSRCEntityInterface->pfnCmdStart( pEntity->GoldSrcEdict(), pCmd, 0 );

	// angles
	// based on QW code, even though it seems kinda odd.
	if ( !pEntity->EntVars()->fixangle )
	{
		VectorCopy( pCmd->viewangles, pEntity->EntVars()->v_angle );
		VectorCopy( pCmd->viewangles, pEntity->EntVars()->angles );
		pEntity->EntVars()->angles[PITCH] /= -3;
	}
	pEntity->EntVars()->angles[ROLL] = pEntity->EntVars()->v_angle[ROLL];

	pEntity->EntVars()->button = pCmd->buttons;

	// Run the player think
	g_pGoldSRCEntityInterface->pfnPlayerPreThink( pEntity->GoldSrcEdict() );
	pEntity->Think();

	// Run PM
	SetupPlayerMove( pEntity, pCmd );
	g_pGoldSRCEntityInterface->pfnPM_Move( &m_playerMove, true );
	ApplyPlayerMove( pEntity );

	//Msg( "%f %f\n", m_playerMove.frametime, gpGlobals->frametime );

	// TODO: Link and Touch
	pEntity->UpdateFromEntVars();

	g_pGoldSRCEntityInterface->pfnPlayerPostThink( pEntity->GoldSrcEdict() );
	g_pGoldSRCEntityInterface->pfnCmdEnd( pEntity->GoldSrcEdict() );
}


void CGoldSRCPlayerMove::SetupPlayerMove( CBaseEntity *pEntity, GoldSRC::usercmd_t *pCmd )
{
	m_playerMove.player_index = pEntity->GetSourceEdict()->m_EdictIndex - 1;
	// m_playerMove.server			(HANDLED IN DLL)
	// 
	// m_playerMove.multiplayer
	// m_playerMove.realtime
	// m_playerMove.frametime		(HANDLED IN DLL)
	// 
	// m_playerMove.forward			(HANDLED IN DLL)
	// m_playerMove.right			(HANDLED IN DLL)
	// m_playerMove.up				(HANDLED IN DLL)

	VectorCopy( pEntity->EntVars()->origin, m_playerMove.origin );
	VectorCopy( pEntity->EntVars()->v_angle, m_playerMove.angles );
	VectorCopy( pEntity->EntVars()->v_angle, m_playerMove.oldangles );
	VectorCopy( pEntity->EntVars()->velocity, m_playerMove.velocity );
	VectorCopy( pEntity->EntVars()->movedir, m_playerMove.movedir );
	VectorCopy( pEntity->EntVars()->basevelocity, m_playerMove.basevelocity );

	VectorCopy( pEntity->EntVars()->view_ofs, m_playerMove.view_ofs );
	m_playerMove.flDuckTime = pEntity->EntVars()->flDuckTime;
	m_playerMove.bInDuck = pEntity->EntVars()->bInDuck;

	//flTimeStepSound
	//iStepLeft

	//flFallVelocity
	//punchangle

	//flSwimTime

	//flNextPrimaryAttack

	//effects

	m_playerMove.flags = pEntity->EntVars()->flags;
	m_playerMove.usehull = pEntity->EntVars()->flags & ( 1 << 14 ) ? 1 : 0;
	m_playerMove.gravity = pEntity->EntVars()->gravity;
	m_playerMove.friction = pEntity->EntVars()->friction;
	m_playerMove.oldbuttons = pEntity->EntVars()->oldbuttons;
	//waterjumptime
	//dead
	//deaflag
	//spectator
	m_playerMove.movetype = pEntity->EntVars()->movetype;

	m_playerMove.onground = -1;	// Hack
	//waterlevel
	//watertype
	//oldwaterlevel	(HANDLED IN DLL)

	//sztexturename
	//chtexturetype

	m_playerMove.maxspeed = g_movevars.maxspeed;
	m_playerMove.clientmaxspeed = pEntity->EntVars()->maxspeed;

	m_playerMove.iuser1 = pEntity->EntVars()->iuser1;
	m_playerMove.iuser2 = pEntity->EntVars()->iuser2;
	m_playerMove.iuser3 = pEntity->EntVars()->iuser3;
	m_playerMove.iuser4 = pEntity->EntVars()->iuser4;
	m_playerMove.fuser1 = pEntity->EntVars()->fuser1;
	m_playerMove.fuser2 = pEntity->EntVars()->fuser2;
	m_playerMove.fuser3 = pEntity->EntVars()->fuser3;
	m_playerMove.fuser4 = pEntity->EntVars()->fuser4;
	VectorCopy( pEntity->EntVars()->vuser1, m_playerMove.vuser1 );
	VectorCopy( pEntity->EntVars()->vuser2, m_playerMove.vuser2 );
	VectorCopy( pEntity->EntVars()->vuser3, m_playerMove.vuser3 );
	VectorCopy( pEntity->EntVars()->vuser4, m_playerMove.vuser4 );

	m_playerMove.cmd = *pCmd;
	//numtouch	(HANDLED IN DLL)

	m_playerMove.runfuncs = true;
}


void CGoldSRCPlayerMove::ApplyPlayerMove( CBaseEntity *pEntity )
{
	VectorCopy( m_playerMove.origin, pEntity->EntVars()->origin );
	if ( !pEntity->EntVars()->fixangle )
	{
		VectorCopy( m_playerMove.angles, pEntity->EntVars()->angles );
		pEntity->EntVars()->angles[PITCH] = -pEntity->EntVars()->v_angle[PITCH] / 3;
		pEntity->EntVars()->angles[YAW] = pEntity->EntVars()->v_angle[YAW];
	}
	pEntity->EntVars()->angles[ROLL] = pEntity->EntVars()->v_angle[ROLL];
	//oldangles
	VectorCopy( m_playerMove.velocity, pEntity->EntVars()->velocity );
	VectorCopy( m_playerMove.movedir, pEntity->EntVars()->movedir );
	VectorCopy( m_playerMove.basevelocity, pEntity->EntVars()->basevelocity );

	VectorCopy( m_playerMove.view_ofs, pEntity->EntVars()->view_ofs );
	pEntity->EntVars()->flDuckTime = m_playerMove.flDuckTime;
	pEntity->EntVars()->bInDuck = m_playerMove.bInDuck;

	//flTimeStepSound
	//iStepLeft

	//flFallVelocity
	//punchangle

	//flSwimTime

	//flNextPrimaryAttack

	//effects

	pEntity->EntVars()->flags = m_playerMove.flags;
	//usehull
	pEntity->EntVars()->gravity = m_playerMove.gravity;
	pEntity->EntVars()->friction = m_playerMove.friction;
	pEntity->EntVars()->oldbuttons = m_playerMove.cmd.buttons;
	//waterjumptime
	//dead
	//deadflag
	//spectator
	//movetype

	//onground
	//waterlevel
	//watertype
	//oldwaterlevel

	//sztexturename
	//chtexturetype

	//maxspeed
	//clientmaxspeed

	pEntity->EntVars()->iuser1 = m_playerMove.iuser1;
	pEntity->EntVars()->iuser2 = m_playerMove.iuser2;
	pEntity->EntVars()->iuser3 = m_playerMove.iuser3;
	pEntity->EntVars()->iuser4 = m_playerMove.iuser4;
	pEntity->EntVars()->fuser1 = m_playerMove.fuser1;
	pEntity->EntVars()->fuser2 = m_playerMove.fuser2;
	pEntity->EntVars()->fuser3 = m_playerMove.fuser3;
	pEntity->EntVars()->fuser4 = m_playerMove.fuser4;
	VectorCopy( m_playerMove.vuser1, pEntity->EntVars()->vuser1 );
	VectorCopy( m_playerMove.vuser2, pEntity->EntVars()->vuser2 );
	VectorCopy( m_playerMove.vuser3, pEntity->EntVars()->vuser3 );
	VectorCopy( m_playerMove.vuser4, pEntity->EntVars()->vuser4 );

	m_playerMove.runfuncs = false;
}


const char *CGoldSRCPlayerMove::PM_Info_ValueForKey( const char *s, const char *key )
{
	LOG_STUB();
	return "0";
}


void CGoldSRCPlayerMove::PM_Particle( float* origin, int color, float life, int zpos, int zvel )
{
	LOG_STUB();
}


int CGoldSRCPlayerMove::PM_TestPlayerPosition( float *pos, GoldSRC::pmtrace_t *ptrace )
{
	LOG_STUB();
	// Pretend there's no entity for now
	return -1;
}


void CGoldSRCPlayerMove::Con_NPrintf( int idx, char *fmt, ... )
{
	LOG_STUB();
}


void CGoldSRCPlayerMove::Con_DPrintf( char *fmt, ... )
{
	LOG_STUB();
}


void CGoldSRCPlayerMove::Con_Printf( char *fmt, ... )
{
	LOG_STUB();
}


double CGoldSRCPlayerMove::PM_Sys_FloatTime( void )
{
	return Sys_FloatTime();
}


void CGoldSRCPlayerMove::PM_StuckTouch( int hitent, GoldSRC::pmtrace_t *ptraceresult )
{
	LOG_STUB();
}


int CGoldSRCPlayerMove::PM_PointContents( float *p, int *truecontents )
{
	LOG_STUB();
	return 0;
}


int CGoldSRCPlayerMove::PM_TruePointContents( float * )
{
	LOG_STUB();
	return 0;
}


int CGoldSRCPlayerMove::PM_HullPointContents( GoldSRC::hull_t *hull, int num, float *p )
{
	LOG_STUB();
	return 0;
}


GoldSRC::pmtrace_t CGoldSRCPlayerMove::PM_PlayerTrace( float *start, float *end, int traceFlags, int ignore_pe )
{
	GoldSRC::playermove_t *pMove = &g_pGoldSRCPlayerMove->m_playerMove;

	Vector vecStart = *(Vector*)start;
	Vector vecEnd = *(Vector*)end;
	Vector vecMins = *(Vector*)pMove->player_mins[pMove->usehull];
	Vector vecMaxs = *(Vector*)pMove->player_maxs[pMove->usehull];

	// Temp as hell
	CTraceFilterWorldOnly filter;
	trace_t tr;

	Ray_t ray;
	ray.Init( vecStart, vecEnd, vecMins, vecMaxs );
	enginetrace->TraceRay( ray, MASK_PLAYERSOLID, &filter, &tr );

	GoldSRC::pmtrace_t ptr;
	ptr.allsolid = tr.allsolid;
	ptr.startsolid = tr.startsolid;
	ptr.inopen = (tr.contents == CONTENTS_EMPTY);
	ptr.inwater = (tr.contents & CONTENTS_WATER) != 0;
	ptr.fraction = tr.fraction;
	VectorCopy( tr.endpos.Base(), ptr.endpos);

	VectorCopy( tr.plane.normal.Base(), ptr.plane.normal);
	ptr.plane.dist = tr.plane.dist;

	ptr.ent = tr.m_pEnt ? 0 : -1;	// TEMP
	//ptr.deltavelocity	-- Handled by DLL
	//ptr.hitgroup -- Unused

	return ptr;
}


GoldSRC::pmtrace_t *CGoldSRCPlayerMove::PM_TraceLine( float *start, float *end, int flags, int usehulll, int ignore_pe )
{
	LOG_STUB();
	return nullptr;
}


GoldSRC::int32 CGoldSRCPlayerMove::RandomLong( GoldSRC::int32 lLow, GoldSRC::int32 lHigh )
{
	return g_pGoldSRCEngineFuncs->pfnRandomLong( lLow, lHigh );
}


float CGoldSRCPlayerMove::RandomFloat( float lLow, float lHigh )
{
	return g_pGoldSRCEngineFuncs->pfnRandomFloat( lLow, lHigh );
}


int CGoldSRCPlayerMove::PM_GetModelType( GoldSRC::model_t *mod )
{
	LOG_STUB();
	return int32();
}


void CGoldSRCPlayerMove::PM_GetModelBounds( GoldSRC::model_t *mod, float *mins, float *maxs )
{
	LOG_STUB();
}


void *CGoldSRCPlayerMove::PM_HullForBsp( GoldSRC::physent_t *pe, float *offset )
{
	LOG_STUB();
	return nullptr;
}


float CGoldSRCPlayerMove::PM_TraceModel( GoldSRC::physent_t *pe, float *offset )
{
	LOG_STUB();
	return 0.0f;
}


int CGoldSRCPlayerMove::COM_FileSize( char *filename )
{
	return filesystem->Size( filename, "GAME" );
}


GoldSRC::byte *CGoldSRCPlayerMove::COM_LoadFile( char *path, int usehunk, int *pLength )
{
	//LOG_STUB();
	FileHandle_t f = filesystem->Open( path, "r", "GAME" );
	return (GoldSRC::byte *)f;
}


void CGoldSRCPlayerMove::COM_FreeFile( void *buffer )
{
	//LOG_STUB();
	filesystem->Close( (FileHandle_t)buffer );
}


char *CGoldSRCPlayerMove::memfgets( GoldSRC::byte *pMemFile, int fileSize, int *pFilePos, char *pBuffer, int bufferSize )
{
	//LOG_STUB();
	return filesystem->ReadLine( pBuffer, bufferSize, (FileHandle_t)pMemFile );
}


class CPlaySoundRecipientFilter : public IRecipientFilter
{
public:
	CPlaySoundRecipientFilter()
	{
		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			CBaseEntity *pEntity = g_pGoldSRCServerGameEnts->GetEntityByIndex( i );
			if ( !pEntity )
				return;

			// Check if player
			if ( pEntity->IsPlayer() )
				m_Recipients.AddToTail( i );
		}
	}

	virtual bool IsReliable( void ) const
	{
		return false;
	}

	virtual bool IsInitMessage( void ) const
	{
		return false;
	}

	virtual int GetRecipientCount( void ) const
	{
		return m_Recipients.Size();
	}

	virtual int GetRecipientIndex( int slot ) const
	{
		return m_Recipients.Element( slot );
	}

private:
	CUtlVector<int> m_Recipients;
};


void CGoldSRCPlayerMove::PM_PlaySound( int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch )
{
	// Recipient filter
	CPlaySoundRecipientFilter filter;

	// entity index
	int entindex = g_pGoldSRCPlayerMove->m_playerMove.player_index + 1;

	// Channel ids are backwards compatible

	// Convert the attenuation to a soundlevel
	soundlevel_t sndlvl = (soundlevel_t)(int)( ( attenuation != 0 ) ? ( 50 + 20 / attenuation ) : 0 );

	// Convert flags
	int flags = 0;

	if ( fFlags & GoldSRC::G_SND_STOP )
		flags |= SND_STOP;
	if ( fFlags & GoldSRC::G_SND_CHANGE_VOL )
		flags |= SND_CHANGE_VOL;
	if ( fFlags & GoldSRC::G_SND_CHANGE_PITCH )
		flags |= SND_CHANGE_PITCH;
	if ( fFlags & GoldSRC::G_SND_SPAWNING )
		flags |= SND_SPAWNING;

	enginesound->EmitSound( filter, entindex, channel, sample, volume, sndlvl, flags, pitch );
}


const char *CGoldSRCPlayerMove::PM_TraceTexture( int ground, float *vstart, float *vend )
{
	//LOG_STUB();

	Vector vecStart = *(Vector*)vstart;
	Vector vecEnd = *(Vector*)vend;

	// TEMP !!!
	CTraceFilterWorldOnly filter;
	trace_t tr;

	Ray_t ray;
	ray.Init( vecStart, vecEnd );
	enginetrace->TraceRay( ray, MASK_PLAYERSOLID, &filter, &tr );

	if ( !tr.DidHit() )
		return NULL;

	if ( !tr.surface.name )
		return NULL;

	// Find the first slash
	// TODO: This should probably be the last slash
	const char *firstSlash = Q_strstr( tr.surface.name, "/" ) + 1;

	if ( firstSlash )
		return firstSlash;

	return tr.surface.name;
}


void CGoldSRCPlayerMove::PM_PlaybackEventFull( int flags, int clientindex, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 )
{
	LOG_STUB();
}


GoldSRC::pmtrace_t CGoldSRCPlayerMove::PM_PlayerTraceEx( float *start, float *end, int traceFlags, int ( *pfnIgnore )( GoldSRC::physent_t *pe ) )
{
	LOG_STUB();
	return GoldSRC::pmtrace_t();
}


int CGoldSRCPlayerMove::PM_TestPlayerPositionEx( float *pos, GoldSRC::pmtrace_t *ptrace, int ( *pfnIgnore )( GoldSRC::physent_t *pe ) )
{
	LOG_STUB();
	return 0;
}


GoldSRC::pmtrace_t *CGoldSRCPlayerMove::PM_TraceLineEx( float *start, float *end, int flags, int usehulll, int ( *pfnIgnore )( GoldSRC::physent_t *pe ) )
{
	LOG_STUB();
	return nullptr;
}
