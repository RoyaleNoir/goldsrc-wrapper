#ifndef GOLDSRC_PLAYERMOVE_SHARED_H
#define GOLDSRC_PLAYERMOVE_SHARED_H

#ifdef _WIN32
#pragma once
#endif

namespace GoldSRC
{
	typedef int qboolean;
	typedef float vec3_t[3];

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

		// TODO
	};
}

#endif // GOLDSRC_PLAYERMOVE_SHARED_H