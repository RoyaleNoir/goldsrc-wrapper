#ifndef GOLDSRC_PLAYERMOVE_SHARED_H
#define GOLDSRC_PLAYERMOVE_SHARED_H

#ifdef _WIN32
#pragma once
#endif

namespace GoldSRC
{
	typedef unsigned char byte;
	typedef int int32;
	typedef int qboolean;
	typedef float vec3_t[3];
	struct model_t;

	enum
	{
		MAX_PHYSENTS = 600,
		MAX_MOVEENTS = 64,
		MAX_CLIP_PLANES = 3,
		MAX_PHYSINFO_STRING = 256,
		MAX_MAP_HULLS = 4,
	};

	// TODO: Should this be here?
	struct usercmd_t
	{
		short lerp_msec;
		byte msec;
		vec3_t viewangles;

		float forwardmove;
		float sidemove;
		float upmove;
		byte lightlevel;
		unsigned short buttons;
		byte impulse;
		byte weaponselect;

		int impact_index;
		vec3_t impact_position;
	};

	struct pmplane_t
	{
		vec3_t	normal;
		float	dist;
	};

	struct pmtrace_t
	{
		qboolean	allsolid;
		qboolean	startsolid;
		qboolean	inopen, inwater;
		float		fraction;
		vec3_t		endpos;
		pmplane_t	plane;
		int			ent;
		vec3_t		deltavelocity;
		int			hitgroup;
	};

	// TODO: Move to goldsrc_model
	struct mplane_t
	{
		vec3_t	normal;
		float	dist;
		byte	type;
		byte	signbits;
		byte	pad[2];
	};

	// TODO: Move to goldsrc_model
	struct dclipnode_t
	{
		int		planenum;
		short	children[2];
	};

	struct hull_t
	{
		dclipnode_t	*clipnodes;
		mplane_t	*planes;
		int			firstclipnode;
	};

	struct movevars_t
	{
		float		gravity;
		float		stopspeed;
		float		maxspeed;
		float		spectatormaxspeed;
		float		accelerate;
		float		airaccelerate;
		float		wateraccelerate;
		float		friction;
		float		edgefriction;
		float		waterfriction;
		float		entgravity;
		float		bounce;
		float		stepsize;
		float		maxvelocity;
		float		zmax;
		float		waveHeight;
		qboolean	footsteps;
		char		skyName[32];
		float		rollangle;
		float		rollspeed;
		float		skycolor_r;
		float		skycolor_g;
		float		skycolor_b;
		float		skyvec_x;
		float		skyvec_y;
		float		skyvec_z;
	};

	struct physent_t
	{
		char	name[32];	// Model name, "player" or "world"
		int		player;
		vec3_t	origin;
		model_t	*model;
		model_t	*studiomodel;
		vec3_t	mins, maxs;
		int		info;
		vec3_t	angles;

		int		solid;
		int		skin;
		int		rendermode;

		float	frame;
		int		sequence;
		byte	controller[4];
		byte	blending[2];

		int		movetype;
		int		takedamage;
		int		blooddecal;
		int		team;
		int		classnumber;

		int		iuser1;
		int		iuser2;
		int		iuser3;
		int		iuser4;
		float	fuser1;
		float	fuser2;
		float	fuser3;
		float	fuser4;
		vec3_t	vuser1;
		vec3_t	vuser2;
		vec3_t	vuser3;
		vec3_t	vuser4;
	};

	struct playermove_t
	{
		int			player_index;
		qboolean	server;

		qboolean	multiplayer;
		float		realtime;
		float		frametime;

		vec3_t		forward, right, up;

		vec3_t		origin;
		vec3_t		angles;
		vec3_t		oldangles;
		vec3_t		velocity;
		vec3_t		movedir;
		vec3_t		basevelocity;

		vec3_t		view_ofs;
		float		flDuckTime;
		qboolean	bInDuck;

		float		flTimeStepSound;
		int			iStepLeft;

		float		flFallVelocity;
		vec3_t		punchangle;

		float		flSwimTime;

		float		flNextPrimaryAttack;

		int			effects;

		int			flags;
		int			usehull;
		float		gravity;
		float		friction;
		int			oldbuttons;
		float		waterjumptime;
		qboolean	dead;
		int			deadflag;
		int			spectator;
		int			movetype;

		int			onground;
		int			waterlevel;
		int			watertype;
		int			oldwaterlevel;

		char		sztexturename[256];
		char		chtexturetype;

		float		maxspeed;
		float		clientmaxspeed;

		int			iuser1;
		int			iuser2;
		int			iuser3;
		int			iuser4;
		float		fuser1;
		float		fuser2;
		float		fuser3;
		float		fuser4;
		vec3_t		vuser1;
		vec3_t		vuser2;
		vec3_t		vuser3;
		vec3_t		vuser4;

		int			numphysent;
		physent_t	physents[MAX_PHYSENTS];

		int			nummoveent;
		physent_t	moveents[MAX_MOVEENTS];

		int			numvisent;
		physent_t	visents[MAX_PHYSENTS];

		usercmd_t	cmd;

		int			numtouch;
		pmtrace_t	touchindex[MAX_PHYSENTS];

		char		physinfo[MAX_PHYSINFO_STRING];

		movevars_t	*movevars;
		vec3_t		player_mins[4];
		vec3_t		player_maxs[4];

		const char*	( *pfnPM_Info_ValueForKey )		( const char *s, const char *key );
		void		( *pfnPM_Particle )				( float *origin, int color, float life, int zpos, int zvel );
		int			( *pfnPM_TestPlayerPosition )	( float *pos, pmtrace_t *ptrace );
		void		( *pfnCon_NPrintf )				( int idx, char *fmt, ... );
		void		( *pfnCon_DPrintf )				( char *fmt, ... );
		void		( *pfnCon_Printf )				( char *fmt, ... );
		double		( *pfnSys_FloatTime )			( void );
		void		( *pfnPM_StuckTouch )			( int hitent, pmtrace_t *ptraceresult );
		int			( *pfnPM_PointContents )		( float *p, int *truecontents );
		int			( *pfnPM_TruePointContents )	( float *p );
		int			( *pfnPM_HullPointContents )	( hull_t *hull, int num, float *p );
		pmtrace_t	( *pfnPM_PlayerTrace )			( float *start, float *end, int traceFlags, int ignore_pe );
		pmtrace_t*	( *pfnPM_TraceLine )			( float *start, float *end, int flags, int usehulll, int ignore_pe );
		int32		( *pfnRandomLong )				( int32 lLow, int32 lHigh );
		float		( *pfnRandomFloat )				( float flLow, float flHigh );
		int			( *pfnPM_GetModelType )			( model_t *mod );
		void		( *pfnPM_GetModelBounds )		( model_t *mod, float *mins, float *maxs );
		void*		( *pfnPM_HullForBsp )			( physent_t *pe, float *offset );
		float		( *pfnPM_TraceModel )			( physent_t *pe, float *offset );
		int			( *pfnCOM_FileSize )			( char *filename );
		byte*		( *pfnCOM_LoadFile )			( char *path, int usehunk, int *pLength );
		void		( *pfnCOM_FreeFile )			( void *buffer );
		char*		( *pfnmemfgets )				( byte *pMemFile, int fileSize, int *pFilePos, char *pBuffer, int bufferSize );

		qboolean	runfuncs;
		void		( *pfnPM_PlaySound )			( int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch );
		const char*	( *pfnPM_TraceTexture )			( int ground, float *vstart, float *vend );
		void		( *pfnPM_PlaybackEventFull )	( int flags, int clientindex, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
		pmtrace_t	( *pfnPM_PlayerTraceEx )		( float *start, float *end, int traceFlags, int ( *pfnIgnore )( physent_t *pe ) );
		int			( *pfnPM_TestPlayerPositionEx )	( float *pos, pmtrace_t *ptrace, int ( *pfnIgnore )( physent_t *pe ) );
		pmtrace_t*	( *pfnPM_TraceLineEx )			( float *start, float *end, int flags, int usehulll, int ( *pfnIgnore )( physent_t *pe ) );
	};
}

#endif // GOLDSRC_PLAYERMOVE_SHARED_H