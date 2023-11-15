#ifndef GOLDSRC_GLOBALVARS_H
#define GOLDSRC_GLOBALVARS_H

#ifdef _WIN32
#pragma once
#endif

// Using this namespace to avoid conflicts with base source
namespace GoldSRC
{
	struct edict_t;
	typedef unsigned int string_t;
	typedef float vec3_t[3];

	struct globalvars_t
	{
		float		realtime;	// renamed from 'time'
		float		frametime;
		float		force_retouch;
		string_t	mapname;
		string_t	startspot;
		float		deathmatch;
		float		coop;
		float		teamplay;
		float		serverflags;
		float		found_secrets;
		vec3_t		v_forward;
		vec3_t		v_up;
		vec3_t		v_right;
		float		trace_allsolid;
		float		trace_startsolid;
		float		trace_fraction;
		vec3_t		trace_endpos;
		vec3_t		trace_plane_normal;
		float		trace_plane_dist;
		edict_t		*trace_ent;
		float		trace_inopen;
		float		trace_inwater;
		int			trace_hitgroup;
		int			trace_flags;
		int			msg_entity;
		int			cdAudioTrack;
		int			maxClients;
		int			maxEntities;
		const char	*pStringBase;
		void		*pSaveData;
		vec3_t		vecLandmarkOffset;
	};
}

extern GoldSRC::globalvars_t *g_pGoldSRCGlobalVars;
extern void GoldSRCGlobalVars_Init();
extern void GoldSRCGlobalVars_Update();

inline GoldSRC::globalvars_t *GlobalVars()
{
	return g_pGoldSRCGlobalVars;
}

#endif // GOLDSRC_GLOBALVARS_H