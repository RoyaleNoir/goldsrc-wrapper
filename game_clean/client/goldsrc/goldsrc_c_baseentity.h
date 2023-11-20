#ifndef GOLDSRC_C_BASEENTITY_H
#define GOLDSRC_C_BASEENTITY_H

#ifdef _WIN32
#pragma once
#endif

#include "icliententity.h"
#include "goldsrc_collideable_shared.h"
#include "goldsrc_cl_entity.h"

#if defined( CLIENT_DLL )
#define CBaseEntity C_BaseEntity
#endif


class C_BaseEntity : public IClientEntity
{
public:
	DECLARE_CLASS_NOBASE( C_BaseEntity );
	DECLARE_CLIENTCLASS();

	C_BaseEntity();
	virtual ~C_BaseEntity();

	GoldSRC::cl_entity_t *ClEnt();

	bool Init( int entnum, int iSerialNum );
	virtual void Release() override;

	const Vector &GetViewOffset() const;
	const Vector &GetVelocity() const;

	matrix3x4_t &EntityToWorldTransform();

	void SetModelIndex( int nModelIndex );

	// IHandleEntity
public:
	virtual void SetRefEHandle( const CBaseHandle &handle ) override;
	virtual const CBaseHandle &GetRefEHandle() const override;


	// IClientEntity
public:
	virtual const Vector &GetAbsOrigin( void ) const override;
	virtual const QAngle &GetAbsAngles( void ) const override;
	virtual CMouthInfo *GetMouth( void ) override;
	virtual bool GetSoundSpatialization( SpatializationInfo_t &info ) override;


	// IClientThinkable
public:
	virtual void ClientThink() override;
	virtual ClientThinkHandle_t GetThinkHandle() override;
	virtual void SetThinkHandle( ClientThinkHandle_t hThink ) override;


	// IClientNetworkable
public:
	virtual void NotifyShouldTransmit( ShouldTransmitState_t state ) override;
	virtual void OnPreDataChanged( DataUpdateType_t updateType ) override;
	virtual void OnDataChanged( DataUpdateType_t updateType ) override;
	virtual void PreDataUpdate( DataUpdateType_t updateType ) override;
	virtual void PostDataUpdate( DataUpdateType_t updateType ) override;
	virtual bool IsDormant( void ) override;
	virtual int entindex( void ) const;
	virtual void ReceiveMessage( int classID, bf_read &msg ) override;
	virtual void *GetDataTableBasePtr() override;
	virtual void SetDestroyedOnRecreateEntities( void ) override;
	virtual void OnDataUnchangedInPVS() override;


	// IClientRenderable
public:
	virtual Vector const &GetRenderOrigin( void ) override;
	virtual QAngle const &GetRenderAngles( void ) override;
	virtual bool ShouldDraw( void ) override;
	virtual bool IsTransparent( void ) override;
	virtual bool UsesPowerOfTwoFrameBufferTexture() override;
	virtual bool UsesFullFrameBufferTexture() override;
	virtual ClientShadowHandle_t GetShadowHandle() const override;
	virtual ClientRenderHandle_t &RenderHandle() override;
	virtual const model_t *GetModel() const override;
	virtual int DrawModel( int flags ) override;
	virtual int GetBody() override;
	virtual void ComputeFxBlend() override;
	virtual int GetFxBlend( void ) override;
	virtual void GetColorModulation( float *color ) override;
	virtual bool LODTest() override { return true; }	// Obsolete
	virtual bool SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime ) override;
	virtual void SetupWeights( const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights ) override;
	virtual void DoAnimationEvents( void ) override;
	virtual IPVSNotify *GetPVSNotifyInterface() override;
	virtual void GetRenderBounds( Vector &mins, Vector &maxs ) override;
	virtual void GetRenderBoundsWorldspace( Vector &mins, Vector &maxs ) override;
	virtual void GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType ) override;
	virtual bool ShouldReceiveProjectedTextures( int flags ) override;
	virtual bool GetShadowCastDistance( float *pDist, ShadowType_t shadowType ) const;
	virtual bool GetShadowCastDirection( Vector *pDirection, ShadowType_t shadowType ) const;
	virtual bool IsShadowDirty() override;
	virtual void MarkShadowDirty( bool bDirty ) override;
	virtual IClientRenderable *GetShadowParent() override;
	virtual IClientRenderable *FirstShadowChild() override;
	virtual IClientRenderable *NextShadowPeer() override;
	virtual ShadowType_t ShadowCastType() override;
	virtual void CreateModelInstance() override;
	virtual ModelInstanceHandle_t GetModelInstance() override;
	virtual const matrix3x4_t &RenderableToWorldTransform() override;
	virtual int LookupAttachment( const char *pAttachmentName ) override;
	virtual bool GetAttachment( int number, Vector &origin, QAngle &angles ) override;
	virtual bool GetAttachment( int number, matrix3x4_t &matrix ) override;
	virtual float *GetRenderClipPlane( void ) override;
	virtual int GetSkin() override;
	virtual bool IsTwoPass( void ) override;
	virtual void OnThreadedDrawSetup() override;
	virtual bool UsesFlexDelayedWeights() override;
	virtual void RecordToolMessage() override;
	virtual bool IgnoresZBuffer( void ) const override;


	// IClientUnknown
public:
	virtual IClientUnknown *GetIClientUnknown() override { return this; }
	virtual ICollideable *GetCollideable() override { return &m_Collideable; }
	virtual IClientNetworkable *GetClientNetworkable() override { return this; }
	virtual IClientRenderable *GetClientRenderable() override { return this; }
	virtual IClientEntity *GetIClientEntity() override { return this; }
	virtual C_BaseEntity *GetBaseEntity() override { return this; }
	virtual IClientThinkable *GetClientThinkable() override { return this; }

	// Memory
public:
	void *operator new( size_t stAllocateBlock );
	void *operator new[]( size_t stAllocateBlock );
	void *operator new( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine );
	void *operator new[]( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine );
	void operator delete( void *pMem );
	void operator delete( void *pMem, int nBlockUse, const char *pFileName, int inLine );

private:
	CCollideable m_Collideable;
	CBaseHandle m_RefEHandle;

	int m_nEntIndex;

	const model_t *m_pModel;
	int m_nModelIndex;

	Vector m_origin;
	QAngle m_angles;
	Vector m_view_ofs;

	Vector m_velocity;

	matrix3x4_t m_entityToWorld;

	ClientThinkHandle_t m_hThinkHandle;
	ClientRenderHandle_t m_hRenderHandle;

	ModelInstanceHandle_t m_ModelInstance;

	// Rendering
	float m_renderamt;
	int m_rendermode;

private:
	GoldSRC::cl_entity_t m_ClEnt;
};


struct C_BaseEntityListEntry
{
	C_BaseEntity *m_pEntity;
	int m_SerialNumber;
};


class C_BaseEntityList
{
public:
	C_BaseEntityList();
	~C_BaseEntityList();

	CBaseHandle AddEntity( C_BaseEntity *pEnt, int index, int iSerialNum );
	void RemoveEntityByIndex( int index );
	void RemoveEntityByHandle( CBaseHandle hEnt );

	C_BaseEntity *GetEntityByIndex( int index );
	C_BaseEntity *GetLocalPlayer();

	void ReleaseAllEntities();

private:
	C_BaseEntityListEntry m_entries[NUM_ENT_ENTRIES];
};

extern C_BaseEntityList *g_pBaseEntityList;


#endif // GOLDSRC_C_BASEENTITY_H