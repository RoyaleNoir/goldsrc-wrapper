#ifndef GOLDSRC_CL_ENTITY_H
#define GOLDSRC_CL_ENTITY_H

#ifdef _WIN32
#pragma once
#endif

namespace GoldSRC
{
	typedef int qboolean;
	typedef unsigned char byte;
	typedef float vec3_t[3];

	struct model_t;
	struct efrag_t;
	struct mnode_t;

	enum EntityType
	{
		ET_NORMAL = 0,
		ET_PLAYER,
		ET_TEMPENTITY,
		ET_BEAM,
		ET_FRAGMENTED
	};

	struct colorVec
	{
		unsigned r, g, b, a;
	};

	struct color24
	{
		byte r, g, b;
	};

	struct entity_state_t
	{
		int		entityType;
		int		number;
		float	msg_time;
		int		messagenum;
		vec3_t	origin;
		vec3_t	angles;
		int		modelindex;
		int		sequence;
		float	frame;
		int		colormap;
		short	skin;
		short	solid;
		int		effects;
		float	scale;
		byte	eflags;
		int		rendermode;
		int		renderamt;
		color24	rendercolor;
		int		renderfx;
		int		movetype;
		float	animtime;
		float	frametime;
		int		body;
		byte	controller[4];
		byte	blending[4];
		vec3_t	velocity;
		vec3_t	mins;
		vec3_t	maxs;
		int		aiment;
		int		owner;
		float	friction;
		float	gravity;
		int		team;
		int		playerclass;
		int		health;
		qboolean	spectation;
		int		weaponmodel;
		int		gaitsequence;
		vec3_t	basevelocity;
		int		usehull;
		int		oldbuttons;
		int		onground;
		int		iStepLeft;
		float	flFallVelocity;
		float	fov;
		int		weaponanim;
		vec3_t	startpos;
		vec3_t	endpos;
		float	impacttime;
		float	starttime;
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

	struct latchedvars_t
	{
		float	prevanimtime;
		float	sequencetime;
		byte	prevseqblending[2];
		vec3_t	prevorigin;
		vec3_t	prevangles;
		int		prevsequence;
		float	prevframe;
		byte	prevcontroller[4];
		byte	prevblending[2];
	};

	struct mouth_t
	{
		byte	mouthopen;
		byte	sndcount;
		int		sndavg;
	};

	struct position_history_t
	{
		float	animtime;
		vec3_t	origin;
		vec3_t	angles;
	};

	#define HISTORY_MAX		64  // Must be power of 2
	#define HISTORY_MASK	( HISTORY_MAX - 1 )

	struct cl_entity_t
	{
		int					index;
		qboolean			player;
		entity_state_t		baseline;
		entity_state_t		prevstate;
		entity_state_t		curstate;
		int					current_position;
		position_history_t	ph[HISTORY_MAX];
		mouth_t				mouth;
		latchedvars_t		latched;
		float				lastmove;
		vec3_t				origin;
		vec3_t				angles;
		vec3_t				attachment[4];
		int					trivial_accept;
		model_t				*model;
		efrag_t				*efrag;
		mnode_t				*topnode;
		float				syncbase;
		int					visframe;
		colorVec			cvFloorColor;
	};
}

#endif // GOLDSRC_CL_ENTITY_H