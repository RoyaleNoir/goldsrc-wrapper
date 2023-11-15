#ifndef GOLDSRC_EDICT_H
#define GOLDSRC_EDICT_H

#ifdef _WIN32
#pragma once
#endif

// Using this namespace to avoid conflicts with base source
namespace GoldSRC
{
	typedef int qboolean;
	typedef unsigned int string_t;
	typedef float vec3_t[3];

	struct edict_t;

	struct link_t
	{
		link_t *prev, *next;
	};

	enum MoveType_t
	{
		MOVETYPE_NONE = 0,
		MOVETYPE_WALK = 3,
		MOVETYPE_STEP,
		MOVETYPE_FLY,
		MOVETYPE_TOSS,
		MOVETYPE_PUSH,
		MOVETYPE_NOCLIP,
		MOVETYPE_FLYMISSILE,
		MOVETYPE_BOUNCE,
		MOVETYPE_BOUNCEMISSILE,
		MOVETYPE_FOLLOW,
		MOVETYPE_PUSHSTEP
	};

	struct entvars_t
	{
		string_t	classname;
		string_t	globalname;
		vec3_t		origin;
		vec3_t		oldorigin;
		vec3_t		velocity;
		vec3_t		basevelocity;
		vec3_t      clbasevelocity;
		vec3_t		movedir;
		vec3_t		angles;
		vec3_t		avelocity;
		vec3_t		punchangle;
		vec3_t		v_angle;
		vec3_t		endpos;
		vec3_t		startpos;
		float		impacttime;
		float		starttime;
		int			fixangle;
		float		idealpitch;
		float		pitch_speed;
		float		ideal_yaw;
		float		yaw_speed;
		int			modelindex;
		string_t	model;
		int			viewmodel;
		int			weaponmodel;
		vec3_t		absmin;
		vec3_t		absmax;
		vec3_t		mins;
		vec3_t		maxs;
		vec3_t		size;
		float		ltime;
		float		nextthink;
		int			movetype;
		int			solid;
		int			skin;			
		int			body;
		int 		effects;
		float		gravity;
		float		friction;
		int			light_level;
		int			sequence;
		int			gaitsequence;
		float		frame;
		float		animtime;
		float		framerate;
		byte		controller[4];
		byte		blending[2];
		float		scale;
		int			rendermode;
		float		renderamt;
		vec3_t		rendercolor;
		int			renderfx;
		float		health;
		float		frags;
		int			weapons;
		float		takedamage;
		int			deadflag;
		vec3_t		view_ofs;
		int			button;
		int			impulse;
		edict_t		*chain;
		edict_t		*dmg_inflictor;
		edict_t		*enemy;
		edict_t		*aiment;
		edict_t		*owner;
		edict_t		*groundentity;
		int			spawnflags;
		int			flags;
		int			colormap;
		int			team;
		float		max_health;
		float		teleport_time;
		float		armortype;
		float		armorvalue;
		int			waterlevel;
		int			watertype;
		string_t	target;
		string_t	targetname;
		string_t	netname;
		string_t	message;
		float		dmg_take;
		float		dmg_save;
		float		dmg;
		float		dmgtime;
		string_t	noise;
		string_t	noise1;
		string_t	noise2;
		string_t	noise3;
		float		speed;
		float		air_finished;
		float		pain_finished;
		float		radsuit_finished;
		edict_t		*pContainingEntity;
		int			playerclass;
		float		maxspeed;
		float		fov;
		int			weaponanim;
		int			pushmsec;
		int			bInDuck;
		int			flTimeStepSound;
		int			flSwimTime;
		int			flDuckTime;
		int			iStepLeft;
		float		flFallVelocity;
		int			gamestate;
		int			oldbuttons;
		int			groupinfo;
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
		edict_t		*euser1;
		edict_t		*euser2;
		edict_t		*euser3;
		edict_t		*euser4;
	};

	struct edict_t
	{
		qboolean	free;
		int			serialnumber;
		link_t		area;
		int			headnode;
		int			num_leafs;
		short		leafnums[48];
		float		freetime;
		void		*pvPrivateData;
		entvars_t	v;
	};
}

class CBaseEntity;

class CGoldSRCEdict
{
public:
	CGoldSRCEdict();
	~CGoldSRCEdict();

	void				Init();
	void				EnsureAllFreed();

	CBaseEntity			*CreateEntity( int iForceEdict = -1 );
	CBaseEntity			*CreateEntityByName( GoldSRC::string_t classname, int iForceEdict = -1 );

	GoldSRC::edict_t	*GetGoldSRCEdict( edict_t *pRealEdict, bool bCreate = false );
	void				DestroyGoldSRCEdict( GoldSRC::edict_t *pEdict );

	GoldSRC::edict_t	*CreateGoldSRCEntity( int iForceEdict = -1 );
	GoldSRC::edict_t	*CreateGoldSRCEntityByName( GoldSRC::string_t classname, int iForceEdict = -1 );
	void				RemoveEntityByGoldSRCEdict( GoldSRC::edict_t *pEdict );

	void				*AllocPrivateData( GoldSRC::edict_t *pEdict, int32 size );
	void				FreePrivateData( GoldSRC::edict_t *pEdict );
	void				*GetPrivateData( GoldSRC::edict_t *pEdict );

	int					ToOffset( const GoldSRC::edict_t *pEdict );
	GoldSRC::edict_t	*FromOffset( int offset );

	int					ToIndex( const GoldSRC::edict_t *pEdict );
	GoldSRC::edict_t	*FromIndex( int index );

	GoldSRC::edict_t	*FindByClassName( GoldSRC::edict_t *pStart, const char *szClassName );
	GoldSRC::edict_t	*FindByTargetName( GoldSRC::edict_t *pStart, const char *szTargetName );

	GoldSRC::edict_t	*FindInSphere( GoldSRC::edict_t *pStart, const float *org, float rad );

	CBaseEntity			*GetSourceEntity( GoldSRC::edict_t *pGoldSRCEdict );

	// TODO: Move these

	void				LinkEdict( GoldSRC::edict_t *pEdict, bool touch_triggers );
	void				UnlinkEdict( GoldSRC::edict_t *pEdict );

private:
	void				InitEdict( GoldSRC::edict_t *pEdict );
	void				FreeEdict( GoldSRC::edict_t *pEdict );

	GoldSRC::edict_t	m_edicts[MAX_EDICTS];
	edict_t				*m_realedicts[MAX_EDICTS];
};

extern CGoldSRCEdict *g_pGoldSRCEdict;

namespace GoldSRC
{
	enum EntFlags
	{
		G_FL_FLY			= (1 << 0),
		G_FL_SWIM			= (1 << 1),
		G_FL_CONVEYOR		= (1 << 2),
		G_FL_CLIENT			= (1 << 3),
		G_FL_INWATER		= (1 << 4),
		G_FL_MONSTER		= (1 << 5),
		G_FL_GODMODE		= (1 << 6),
		G_FL_NOTARGET		= (1 << 7),
		G_FL_SKIPLOCALHOST	= (1 << 8),
		G_FL_ONGROUND		= (1 << 9),
		G_FL_PARTIALGROUND	= (1 << 10),
		G_FL_WATERJUMP		= (1 << 11),
		G_FL_FROZEN			= (1 << 12),
		G_FL_FAKECLIENT		= (1 << 13),
		G_FL_DUCKING		= (1 << 14),
		G_FL_FLOAT			= (1 << 15),
		G_FL_GRAPHED		= (1 << 16),
		G_FL_IMMUNE_WATER	= (1 << 17),
		G_FL_IMMUNE_SLIME	= (1 << 18),
		G_FL_IMMNE_LAVA		= (1 << 19),
		G_FL_PROXY			= (1 << 20),
		G_FL_ALWAYSTHINK	= (1 << 21),
		G_FL_BASEVELOCITY	= (1 << 22),
		G_FL_MONSTERCLIP	= (1 << 23),
		G_FL_ONTRAIN		= (1 << 24),
		G_FL_WORLDBRUSH		= (1 << 25),
		G_FL_SPECTATOR		= (1 << 26),
		G_FL_CUSTOMENTITY	= (1 << 29),
		G_FL_KILLME			= (1 << 30),
		G_FL_DORMANT		= (1 << 31),
	};
}

#endif // GOLDSRC_EDICT_H