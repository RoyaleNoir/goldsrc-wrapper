#include "cbase.h"
#include "goldsrc_c_baseentity.h"
#include "engine/ivmodelinfo.h"
#include "engine/ivmodelrender.h"
#include "ivrenderview.h"
#include "model_types.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IVModelInfoClient *modelinfo;
extern IVModelRender *modelrender;


void SetActivityForSequence( CStudioHdr *pstudiohdr, int i )
{
}

#undef CBaseEntity


void RecvProxy_ModelIndex( const CRecvProxyData *pData, void *pStruct, void *pOut )
{
	C_BaseEntity *pEnt = (C_BaseEntity *)pStruct;

	int nModelIndex = pData->m_Value.m_Int;

	pEnt->SetModelIndex( nModelIndex );
}


IMPLEMENT_CLIENTCLASS_DT_NOBASE( C_BaseEntity, DT_BaseEntity, CBaseEntity )
	RecvPropInt( RECVINFO( m_nModelIndex ), 0, RecvProxy_ModelIndex ),
	RecvPropVector( RECVINFO( m_origin ) ),
	RecvPropQAngles( RECVINFO( m_angles ) ),
	RecvPropVector( RECVINFO( m_view_ofs ) ),
	RecvPropDataTable( RECVINFO( m_Collideable ), &REFERENCE_RECV_TABLE( DT_Collideable ) ),
END_RECV_TABLE()

#define CBaseEntity C_BaseEntity


C_BaseEntity::C_BaseEntity()
{
	m_RefEHandle.Term();
	m_nEntIndex = -1;

	SetIdentityMatrix( m_entityToWorld );

	m_hThinkHandle = NULL;	// TODO: Think list
	m_hRenderHandle = INVALID_CLIENT_RENDER_HANDLE;

	m_pModel = NULL;
	m_nModelIndex = 0;
	m_ModelInstance = MODEL_INSTANCE_INVALID;
}


C_BaseEntity::~C_BaseEntity()
{
	if ( m_RefEHandle != CBaseHandle( INVALID_EHANDLE_INDEX ) )
	{
		g_pBaseEntityList->RemoveEntityByHandle( m_RefEHandle );
	}

	if ( m_ModelInstance != MODEL_INSTANCE_INVALID )
	{
		modelrender->DestroyInstance( m_ModelInstance );
		m_ModelInstance = MODEL_INSTANCE_INVALID;
	}
}


bool C_BaseEntity::Init( int entnum, int iSerialNum )
{
	m_nEntIndex = entnum;

	g_pBaseEntityList->AddEntity( this, entnum, iSerialNum );

	return true;
}


void C_BaseEntity::Release()
{
	delete this;
}


const Vector &C_BaseEntity::GetViewOffset() const
{
	return m_view_ofs;
}


matrix3x4_t& C_BaseEntity::EntityToWorldTransform()
{
	return m_entityToWorld;
}


void C_BaseEntity::SetModelIndex( int nModelIndex )
{
	if ( nModelIndex == m_nModelIndex )
		return;

	if ( m_ModelInstance != MODEL_INSTANCE_INVALID )
	{
		modelrender->DestroyInstance( m_ModelInstance );
		m_ModelInstance = MODEL_INSTANCE_INVALID;
	}

	m_pModel = modelinfo->GetModel( nModelIndex );
	m_nModelIndex = nModelIndex;
}


void C_BaseEntity::SetRefEHandle( const CBaseHandle &handle )
{
	m_RefEHandle = handle;
}


const CBaseHandle &C_BaseEntity::GetRefEHandle() const
{
	return m_RefEHandle;
}


const Vector &C_BaseEntity::GetAbsOrigin( void ) const
{
	return m_origin;
}


const QAngle &C_BaseEntity::GetAbsAngles( void ) const
{
	return m_angles;
}


CMouthInfo *C_BaseEntity::GetMouth( void )
{
	return NULL;
}


bool C_BaseEntity::GetSoundSpatialization( SpatializationInfo_t &info )
{
	// TODO: PVS
	return true;
}


void C_BaseEntity::ClientThink()
{
}


ClientThinkHandle_t C_BaseEntity::GetThinkHandle()
{
	return m_hThinkHandle;
}


void C_BaseEntity::SetThinkHandle( ClientThinkHandle_t hThink )
{
	m_hThinkHandle = hThink;
}


void C_BaseEntity::NotifyShouldTransmit( ShouldTransmitState_t state )
{
	// TODO: Networking
}


void C_BaseEntity::OnPreDataChanged( DataUpdateType_t updateType )
{
	// TODO: Networking
}


void C_BaseEntity::OnDataChanged( DataUpdateType_t updateType )
{
	// TODO: Networking
}


void C_BaseEntity::PreDataUpdate( DataUpdateType_t updateType )
{
	// TODO: Networking
}


void C_BaseEntity::PostDataUpdate( DataUpdateType_t updateType )
{
	// TODO: Networking
}


bool C_BaseEntity::IsDormant( void )
{
	// TODO: Networking, PVS
	return false;
}


int C_BaseEntity::entindex( void ) const
{
	return m_nEntIndex;
}


void C_BaseEntity::ReceiveMessage( int classID, bf_read &msg )
{
	// TODO: Networking
}


void *C_BaseEntity::GetDataTableBasePtr()
{
	return this;
}


void C_BaseEntity::SetDestroyedOnRecreateEntities( void )
{
	// TODO: Networking
}


void C_BaseEntity::OnDataUnchangedInPVS()
{
	// TODO: Networking
}


Vector const &C_BaseEntity::GetRenderOrigin( void )
{
	return GetAbsOrigin();
}


QAngle const &C_BaseEntity::GetRenderAngles( void )
{
	return GetAbsAngles();
}


bool C_BaseEntity::ShouldDraw( void )
{
	// TODO: Rendering
	return m_pModel != NULL && m_nEntIndex != 0;
}


bool C_BaseEntity::IsTransparent( void )
{
	// TODO: Rendering
	return false;
}


bool C_BaseEntity::UsesPowerOfTwoFrameBufferTexture()
{
	// TODO: Rendering
	return false;
}


bool C_BaseEntity::UsesFullFrameBufferTexture()
{
	// TODO: Rendering
	return false;
}


ClientShadowHandle_t C_BaseEntity::GetShadowHandle() const
{
	// TODO: Rendering
	return CLIENTSHADOW_INVALID_HANDLE;
}


ClientRenderHandle_t &C_BaseEntity::RenderHandle()
{
	return m_hRenderHandle;
}


const model_t *C_BaseEntity::GetModel() const
{
	return m_pModel;
}


int C_BaseEntity::DrawModel( int flags )
{
	// TODO: Rendering
	
	if ( !m_pModel )
		return false;

	int modelType = modelinfo->GetModelType( m_pModel );

	if ( modelType == mod_brush )
	{
		render->DrawBrushModel( this, (model_t *)m_pModel, GetAbsOrigin(), GetAbsAngles(), false );
		return true;
	}

	if (modelType == mod_studio)
	{
		if ( m_ModelInstance == MODEL_INSTANCE_INVALID )
		{
			m_ModelInstance = modelrender->CreateInstance( this );
		}

		if ( IsTwoPass() )
		{
			flags |= STUDIO_TWOPASS;
		}

		int drawn = modelrender->DrawModel( flags, this, m_ModelInstance,m_nEntIndex, m_pModel, GetAbsOrigin(), GetAbsAngles(),GetSkin(), GetBody(), 0 );

		return drawn;
	}
	
	return 0;
}


int C_BaseEntity::GetBody()
{
	// TODO: Rendering
	return 0;
}


void C_BaseEntity::ComputeFxBlend()
{
	// TODO: Rendering
}


int C_BaseEntity::GetFxBlend( void )
{
	// TODO: Rendering
	return 255;
}


void C_BaseEntity::GetColorModulation( float *color )
{
	// TODO: Rendering
	color[0] = color[1] = color[2] = 1.0f;
}


bool C_BaseEntity::SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
{
	// TODO: Rendering
	if ( !m_pModel || modelinfo->GetModelType( m_pModel ) != mod_studio )
		return false;

	if ( pBoneToWorldOut )
	{
		matrix3x4_t baseMatrix;
		AngleMatrix( GetRenderAngles(), GetRenderOrigin(), baseMatrix );
		
		for ( int i = 0; i < nMaxBones; i++ )
		{
			pBoneToWorldOut[i] = baseMatrix;
		}
	}

	return true;
}


void C_BaseEntity::SetupWeights( const matrix3x4_t* pBoneToWorld, int nFlexWeightCount, float* pFlexWeights, float* pFlexDelayedWeights )
{
	// TODO: Rendering
}


void C_BaseEntity::DoAnimationEvents( void )
{
	// TODO: Rendering
}


IPVSNotify *C_BaseEntity::GetPVSNotifyInterface()
{
	// TODO: PVS
	return NULL;
}


void C_BaseEntity::GetRenderBounds( Vector &mins, Vector &maxs )
{
	// TODO: Rendering
	// TODO: Handle rotation
	mins = m_Collideable.OBBMins();
	maxs = m_Collideable.OBBMaxs();
}


void C_BaseEntity::GetRenderBoundsWorldspace( Vector &mins, Vector &maxs )
{
	// TODO: Handle rotation
	Vector localMins, localMaxs;
	GetRenderBounds( localMins, localMaxs );

	VectorAdd( localMins, GetRenderOrigin(), mins );
	VectorAdd( localMaxs, GetRenderOrigin(), maxs );
}


void C_BaseEntity::GetShadowRenderBounds( Vector &mins, Vector &maxs, ShadowType_t shadowType )
{
	GetRenderBounds( mins, maxs );
}


bool C_BaseEntity::ShouldReceiveProjectedTextures( int flags )
{
	// TODO: Rendering
	return false;
}


bool C_BaseEntity::GetShadowCastDistance( float *pDist, ShadowType_t shadowType ) const
{
	// TODO: Rendering
	return false;
}


bool C_BaseEntity::GetShadowCastDirection( Vector *pDirection, ShadowType_t shadowType ) const
{
	// TODO: Rendering
	return false;
}


bool C_BaseEntity::IsShadowDirty()
{
	// TODO: Rendering
	return false;
}


void C_BaseEntity::MarkShadowDirty( bool bDirty )
{
	// TODO: Rendering
}


IClientRenderable *C_BaseEntity::GetShadowParent()
{
	// TODO: Rendering
	return NULL;
}


IClientRenderable* C_BaseEntity::FirstShadowChild()
{
	// TODO: Rendering
	return NULL;
}


IClientRenderable* C_BaseEntity::NextShadowPeer()
{
	// TODO: Rendering
	return NULL;
}


ShadowType_t C_BaseEntity::ShadowCastType()
{
	// TODO: Rendering
	return SHADOWS_NONE;
}


void C_BaseEntity::CreateModelInstance()
{
	// TODO: Rendering
}


ModelInstanceHandle_t C_BaseEntity::GetModelInstance()
{
	// TODO: Rendering
	return MODEL_INSTANCE_INVALID;
}


const matrix3x4_t &C_BaseEntity::RenderableToWorldTransform()
{
	return EntityToWorldTransform();
}


int C_BaseEntity::LookupAttachment( const char *pAttachmentName )
{
	// TODO: Rendering
	return -1;
}


bool C_BaseEntity::GetAttachment( int number, Vector &origin, QAngle &angles )
{
	// TODO: Rendering
	return false;
}


bool C_BaseEntity::GetAttachment( int number, matrix3x4_t &matrix )
{
	// TODO: Rendering
	return false;
}


float *C_BaseEntity::GetRenderClipPlane( void )
{
	// TODO: Rendering
	return NULL;
}


int C_BaseEntity::GetSkin()
{
	// TODO: Rendering
	return 0;
}


bool C_BaseEntity::IsTwoPass( void )
{
	// TODO: Rendering
	return modelinfo->IsTranslucentTwoPass( m_pModel );
}


void C_BaseEntity::OnThreadedDrawSetup()
{
}


bool C_BaseEntity::UsesFlexDelayedWeights()
{
	// TODO: Rendering
	return false;
}


void C_BaseEntity::RecordToolMessage()
{
}


bool C_BaseEntity::IgnoresZBuffer( void ) const
{
	// TODO: Rendering
	return false;
}


#include "tier0/memdbgoff.h"

void *C_BaseEntity::operator new( size_t stAllocateBlock )
{
	MEM_ALLOC_CREDIT();
	void *pMem = MemAlloc_Alloc( stAllocateBlock );
	memset( pMem, 0, stAllocateBlock );
	return pMem;
}


void *C_BaseEntity::operator new[]( size_t stAllocateBlock )
{
	MEM_ALLOC_CREDIT();
	void *pMem = MemAlloc_Alloc( stAllocateBlock );
	memset( pMem, 0, stAllocateBlock );
	return pMem;
}


void *C_BaseEntity::operator new( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine )
{
	void *pMem = MemAlloc_Alloc( stAllocateBlock, pFileName, nLine );
	memset( pMem, 0, stAllocateBlock );
	return pMem;
}


void *C_BaseEntity::operator new[]( size_t stAllocateBlock, int nBlockUse, const char *pFileName, int nLine )
{
	void *pMem = MemAlloc_Alloc( stAllocateBlock, pFileName, nLine );
	memset( pMem, 0, stAllocateBlock );
	return pMem;
}


void C_BaseEntity::operator delete( void *pMem )
{
	MemAlloc_Free( pMem );
}


void C_BaseEntity::operator delete( void *pMem, int nBlockUse, const char *pFileName, int inLine )
{
	operator delete( pMem );
}

#include "tier0/memdbgon.h"

static C_BaseEntityList g_BaseEntityList;
C_BaseEntityList *g_pBaseEntityList = &g_BaseEntityList;


C_BaseEntityList::C_BaseEntityList()
{
	for ( int i = 0; i < NUM_ENT_ENTRIES; i++ )
	{
		m_entries[i].m_pEntity = NULL;
		m_entries[i].m_SerialNumber = rand() & 0x7fff;
	}
}


C_BaseEntityList::~C_BaseEntityList()
{
}


CBaseHandle C_BaseEntityList::AddEntity( C_BaseEntity *pEnt, int index, int iSerialNum )
{
	if ( index == INVALID_EHANDLE_INDEX )
		return CBaseHandle( INVALID_EHANDLE_INDEX );

	C_BaseEntityListEntry *pEntry = &m_entries[index];
	pEntry->m_pEntity = pEnt;
	pEntry->m_SerialNumber = iSerialNum;

	CBaseHandle hEnt( index, iSerialNum );
	pEnt->SetRefEHandle( hEnt );

	return hEnt;
}


void C_BaseEntityList::RemoveEntityByIndex( int index )
{
	if ( index == INVALID_EHANDLE_INDEX )
		return;

	C_BaseEntityListEntry *pEntry = &m_entries[index];

	if ( pEntry->m_pEntity )
	{
		pEntry->m_pEntity->SetRefEHandle( INVALID_EHANDLE_INDEX );
		pEntry->m_pEntity = NULL;
		pEntry->m_SerialNumber = ( pEntry->m_SerialNumber + 1) & 0x7fff;
	}
}


void C_BaseEntityList::RemoveEntityByHandle( CBaseHandle hEnt )
{
	RemoveEntityByIndex( hEnt.GetEntryIndex() );
}


C_BaseEntity *C_BaseEntityList::GetEntityByIndex( int index )
{
	if ( index == INVALID_EHANDLE_INDEX )
		return NULL;

	return m_entries[index].m_pEntity;
}


void C_BaseEntityList::ReleaseAllEntities()
{
	for ( int i = 0; i < NUM_ENT_ENTRIES; i++ )
	{
		C_BaseEntity *pEnt = m_entries[i].m_pEntity;

		if ( !pEnt )
			continue;

		RemoveEntityByIndex( i );
		pEnt->Release();
	}
}
