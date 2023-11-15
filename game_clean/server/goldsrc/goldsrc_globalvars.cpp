#include "cbase.h"
#include "goldsrc_globalvars.h"
#include "goldsrc_eiface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern CGlobalVars* gpGlobals;

GoldSRC::globalvars_t g_GoldSRCGlobalVars = {};
GoldSRC::globalvars_t *g_pGoldSRCGlobalVars = &g_GoldSRCGlobalVars;


void GoldSRCGlobalVars_Init()
{
	//g_GoldSRCGlobalVars.realtime;
	//g_GoldSRCGlobalVars.frametime;
	g_GoldSRCGlobalVars.force_retouch = 0;
	g_GoldSRCGlobalVars.mapname = NULL;
	g_GoldSRCGlobalVars.startspot = NULL;
	//g_GoldSRCGlobalVars.deathmatch;
	//g_GoldSRCGlobalVars.coop;
	//g_GoldSRCGlobalVars.teamplay;
	g_GoldSRCGlobalVars.serverflags = 0;
	g_GoldSRCGlobalVars.found_secrets = 0;

	g_GoldSRCGlobalVars.v_forward[0] = 1;
	g_GoldSRCGlobalVars.v_forward[1] = 0;
	g_GoldSRCGlobalVars.v_forward[2] = 0;
	g_GoldSRCGlobalVars.v_up[0] = 0;
	g_GoldSRCGlobalVars.v_up[1] = 0;
	g_GoldSRCGlobalVars.v_up[2] = 1;
	g_GoldSRCGlobalVars.v_right[0] = 0;
	g_GoldSRCGlobalVars.v_right[1] = -1;
	g_GoldSRCGlobalVars.v_right[2] = 0;

	g_GoldSRCGlobalVars.trace_allsolid = 0;
	g_GoldSRCGlobalVars.trace_startsolid = 0;
	g_GoldSRCGlobalVars.trace_fraction = 0;
	g_GoldSRCGlobalVars.trace_endpos[0] = 0;
	g_GoldSRCGlobalVars.trace_endpos[1] = 0;
	g_GoldSRCGlobalVars.trace_endpos[2] = 0;
	g_GoldSRCGlobalVars.trace_plane_normal[0] = 0;
	g_GoldSRCGlobalVars.trace_plane_normal[1] = 0;
	g_GoldSRCGlobalVars.trace_plane_normal[2] = 0;
	g_GoldSRCGlobalVars.trace_plane_dist = 0;
	g_GoldSRCGlobalVars.trace_ent = NULL;
	g_GoldSRCGlobalVars.trace_inopen = 0;
	g_GoldSRCGlobalVars.trace_inwater = 0;
	g_GoldSRCGlobalVars.trace_hitgroup = 0;
	g_GoldSRCGlobalVars.trace_flags = 0;

	g_GoldSRCGlobalVars.msg_entity = 0;
	g_GoldSRCGlobalVars.cdAudioTrack = 0;
	//g_GoldSRCGlobalVars.maxClients
	//g_GoldSRCGlobalVars.maxEntities

	g_GoldSRCGlobalVars.pStringBase = GoldSRC::SzFromIndex( NULL );
	g_GoldSRCGlobalVars.pSaveData = NULL;
	g_GoldSRCGlobalVars.vecLandmarkOffset[0] = 0;
	g_GoldSRCGlobalVars.vecLandmarkOffset[1] = 0;
	g_GoldSRCGlobalVars.vecLandmarkOffset[2] = 0;

	GoldSRCGlobalVars_Update();
}


void GoldSRCGlobalVars_Update()
{
	g_GoldSRCGlobalVars.realtime = gpGlobals->curtime;
	g_GoldSRCGlobalVars.frametime = gpGlobals->frametime;
	//g_GoldSRCGlobalVars.force_retouch
	g_GoldSRCGlobalVars.mapname = (GoldSRC::string_t)( gpGlobals->mapname.ToCStr() - (int)GoldSRC::SzFromIndex( NULL ) );
	//g_GoldSRCGlobalVars.startspot	// TODO: string_t
	g_GoldSRCGlobalVars.deathmatch = gpGlobals->deathmatch ? 1 : 0;
	g_GoldSRCGlobalVars.coop = gpGlobals->coop ? 1 : 0;
	g_GoldSRCGlobalVars.teamplay = gpGlobals->teamplay ? 1 : 0;
	//g_GoldSRCGlobalVars.serverflags;
	//g_GoldSRCGlobalVars.found_secrets;

	//g_GoldSRCGlobalVars.v_forward
	//g_GoldSRCGlobalVars.v_up
	//g_GoldSRCGlobalVars.v_right

	//g_GoldSRCGlobalVars.trace_allsolid;
	//g_GoldSRCGlobalVars.trace_startsolid;
	//g_GoldSRCGlobalVars.trace_fraction;
	//g_GoldSRCGlobalVars.trace_endpos[0];
	//g_GoldSRCGlobalVars.trace_endpos[1];
	//g_GoldSRCGlobalVars.trace_endpos[2];
	//g_GoldSRCGlobalVars.trace_plane_normal[0];
	//g_GoldSRCGlobalVars.trace_plane_normal[1];
	//g_GoldSRCGlobalVars.trace_plane_normal[2];
	//g_GoldSRCGlobalVars.trace_plane_dist;
	//g_GoldSRCGlobalVars.trace_ent;
	//g_GoldSRCGlobalVars.trace_inopen;
	//g_GoldSRCGlobalVars.trace_inwater;
	//g_GoldSRCGlobalVars.trace_hitgroup;
	//g_GoldSRCGlobalVars.trace_flags;

	//g_GoldSRCGlobalVars.msg_entity;
	//g_GoldSRCGlobalVars.cdAudioTrack;
	g_GoldSRCGlobalVars.maxClients = gpGlobals->maxClients;
	g_GoldSRCGlobalVars.maxEntities = gpGlobals->maxEntities;

	//g_GoldSRCGlobalVars.pStringBase // TODO: string_t
	//g_GoldSRCGlobalVars.pSaveData	// TODO: saveload
	//g_GoldSRCGlobalVars.vecLandmarkOffset
}
