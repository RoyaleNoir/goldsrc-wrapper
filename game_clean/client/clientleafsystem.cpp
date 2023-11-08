#include "cbase.h"
#include "clientleafsystem.h"
#include "interface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CClientLeafSystem g_ClientLeafSystem;
CClientLeafSystem *g_pClientLeafSystem = &g_ClientLeafSystem;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CClientLeafSystem, IClientLeafSystemEngine, CLIENTLEAFSYSTEM_INTERFACE_VERSION, g_ClientLeafSystem );


void CClientLeafSystem::CreateRenderableHandle( IClientRenderable *pRenderable, bool bIsStaticProp )
{
}


void CClientLeafSystem::RemoveRenderable( ClientRenderHandle_t handle )
{
}


void CClientLeafSystem::AddRenderableToLeaves( ClientRenderHandle_t renderable, int nLeafCount, unsigned short *pLeaves )
{
}


void CClientLeafSystem::ChangeRenderableRenderGroup( ClientRenderHandle_t handle, RenderGroup_t group )
{
}
