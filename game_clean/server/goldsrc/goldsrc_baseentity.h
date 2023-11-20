#ifndef GOLDSRC_BASEENTITY_H
#define GOLDSRC_BASEENTITY_H

#ifdef _WIN32
#pragma once
#endif

#include "iserverentity.h"
#include "networkvar.h"
#include "goldsrc_eiface.h"
#include "goldsrc_edict.h"
#include "goldsrc_collideable_shared.h"
#include "PlayerState.h"
#include "goldsrc_model.h"


//-----------------------------------------------------------------------------
// The engine's representation of network data for an entity
//-----------------------------------------------------------------------------
class CServerNetworkable : public IServerNetworkable
{
public:
	CServerNetworkable();
	virtual ~CServerNetworkable();

	void Init( CBaseEntity *pEntity );

	void AttachEdict( edict_t *pEdict );
	void DetachEdict();

	virtual IHandleEntity *GetEntityHandle();
	virtual ServerClass *GetServerClass();
	virtual edict_t *GetEdict() const;
	virtual const char *GetClassName() const;
	virtual void Release();
	virtual int AreaNum() const;

	virtual CBaseNetworkable *GetBaseNetworkable();
	virtual CBaseEntity *GetBaseEntity();
	virtual PVSInfo_t *GetPVSInfo();

private:
	CBaseEntity *m_pEntity;
	edict_t *m_pEdict;
};


//-----------------------------------------------------------------------------
// Represents a GoldSRC entity
//-----------------------------------------------------------------------------
class CBaseEntity : public IServerEntity
{
public:
	DECLARE_CLASS_NOBASE( CBaseEntity );
	DECLARE_SERVERCLASS();

	CBaseEntity();
	virtual ~CBaseEntity();

	GoldSRC::entvars_t *EntVars();

	void PostGoldSrcCreate();
	void PostGoldSrcSpawn();

	void RunTick();
	bool TryPush( float movetime );
	GoldSRC::TraceResult TryMove( Vector &move );

	void AddGravity( float scale );
	void CheckVelocity();
	void ClipVelocity( GoldSRC::vec3_t inVel, GoldSRC::vec3_t normal, GoldSRC::vec3_t outVel, float overbounce );

	void Impact( CBaseEntity *pOther );

	void Relink( bool touchTriggers );

	// Helpers
public:

	void SetClassName( GoldSRC::string_t iClassName );
	GoldSRC::string_t GetClassName();

	void AttachEdict( edict_t *pOverrideEdict = NULL );
	void DetachGoldSRCEdict();

	edict_t *GetSourceEdict() const;
	GoldSRC::edict_t *GoldSrcEdict() const;

	const Vector &GetAbsOrigin() const;
	const QAngle &GetAbsAngles() const;

	void UpdateFromEntVars();

	CPlayerState *GetPlayerState();
	
	void SetIsPlayer( bool bPlayer );
	bool IsPlayer();

	CCollideable *GetCCollideable() { return &m_Collideable; }

	// GoldSRC entity interface
public:
	void Spawn();
	bool Think( float useRealTime = false );
	void Use( CBaseEntity *pOtherEnt );
	void Touch( CBaseEntity *pOtherEnt );

	// GoldSRC progs interface
public:
	void SetOrigin( const float *flOrigin );
	void SetSize( const float *min, const float *max );
	void SetModel( const char *szModelName );

	int DropToFloor();
	int MoveStep( const Vector &move, bool relink );

	bool StepDirection( float yaw, float dist );
	void NewChaseDir( const float *pflGoal, float dist );
	void MoveToGoal( const float *pflGoal, float dist, int iMovetype );

	void ChangeYaw();

	void *GetModelPtr();
	void GetBonePosition( int iBone, float *rgflOrigin, float *rgflAngles );

	const model_t *GetSourceModel();

	// IHandleEntity
public:
	virtual void SetRefEHandle( const CBaseHandle &handle );
	virtual const CBaseHandle& GetRefEHandle() const;

	// IServerUnknown
public:
	virtual ICollideable *GetCollideable();
	virtual IServerNetworkable *GetNetworkable();
	virtual CBaseEntity *GetBaseEntity();

	// IServerEntity
public:
	virtual int GetModelIndex( void ) const;
	virtual string_t GetModelName( void ) const;
	virtual void SetModelIndex( int index );

	// Memory
public:
	void *operator new( size_t stAllocateBlock );
	void *operator new( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine );
	void operator delete( void *pMem );
	void operator delete( void *pMem, int nBlockUse, const char *pFileName, int inLine );

public:
	void NetworkStateChanged();
	void NetworkStateChanged( void *pVar );

private:
	CServerNetworkable m_Networkable;
	CNetworkVarEmbedded( CCollideable, m_Collideable );
	CBaseHandle m_RefEHandle;

	GoldSRC::edict_t *m_pGoldSrcEdict;

	GoldSRC::string_t m_iClassName;

	// Model stuff
	CNetworkVar( int, m_nModelIndex );

	// Common entvars
	CNetworkVector( m_origin );
	CNetworkQAngle( m_angles );
	CNetworkVector( m_view_ofs );

	CNetworkVector( m_velocity );

	// Player entvars
	// SEND THESE VIA USERCMD?
	//QAngle m_v_angle;


	// Rendering entvars
	CNetworkVar( float, m_renderamt );
	CNetworkVar( int, m_rendermode );

	const char *m_szDebugClassName;

	CPlayerState m_PlayerState;
	bool m_bPlayer;

	GoldSRC::studiohdr_t *m_pGoldSRCModel;
};

#endif // GOLDSRC_BASEENTITY_H