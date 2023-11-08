#include "cbase.h"
#include "iclientnetworkable.h"
#include "iclientunknown.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

class C_DummyClientClass : public IClientUnknown, public IClientNetworkable
{
public:
	DECLARE_CLASS_NOBASE( C_DummyClientClass );
	DECLARE_CLIENTCLASS();

	virtual bool Init( int entnum, int iSerialNum )
	{
		// TODO: Store off to deallocate
		return true;
	}

	// IHandleEntity
public:
	virtual void				SetRefEHandle( const CBaseHandle &handle ) {}
	virtual const CBaseHandle	&GetRefEHandle() const { return *((CBaseHandle *)NULL); }

	// IClientUnknown
public:
	virtual ICollideable		*GetCollideable() { return NULL; }
	virtual IClientNetworkable	*GetClientNetworkable() { return this; }
	virtual IClientRenderable	*GetClientRenderable() { return NULL; }
	virtual IClientEntity		*GetIClientEntity() { return NULL; }
	virtual C_BaseEntity		*GetBaseEntity() { return NULL; }
	virtual IClientThinkable	*GetClientThinkable() { return NULL; }

	// IClientNetworkable
public:
	virtual IClientUnknown		*GetIClientUnknown() { return this; }
	
	virtual void				Release(){}
	virtual void				NotifyShouldTransmit( ShouldTransmitState_t state ) {}
	virtual void				OnPreDataChanged( DataUpdateType_t updateType ) {}
	virtual void				OnDataChanged( DataUpdateType_t updateType ) {}
	virtual void				PreDataUpdate( DataUpdateType_t updateType ) {}
	virtual void				PostDataUpdate( DataUpdateType_t updateType ) {}
	virtual bool				IsDormant() { return false; }
	virtual int					entindex( void ) const { return 0; }
	virtual void				ReceiveMessage( int classID, bf_read &msg ) {}
	virtual void				*GetDataTableBasePtr() { return this; }
	virtual void				SetDestroyedOnRecreateEntities( void ) {}
	virtual void				OnDataUnchangedInPVS() {}
};

IMPLEMENT_CLIENTCLASS_DT_NOBASE( C_DummyClientClass, DT_DummyClass, CDummyServerClass )
END_RECV_TABLE()
