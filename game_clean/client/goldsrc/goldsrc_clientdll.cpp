#include "cbase.h"
#include "goldsrc_clientdll.h"
#include "goldsrc_c_baseentity.h"
#include "ienginevgui.h"
#include "ivrenderview.h"
#include "view_shared.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineClient *engine;


CGoldSRCClientDLL g_BaseClientDLL;
IBaseClientDLL *g_pClientDLL = &g_BaseClientDLL;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCClientDLL, IBaseClientDLL, CLIENT_DLL_INTERFACE_VERSION, g_BaseClientDLL );


void CGoldSRCClientDLL::LevelShutdown( void )
{
	g_pBaseEntityList->ReleaseAllEntities();
}


void CGoldSRCClientDLL::View_Render( vrect_t *rect )
{
	CViewSetup viewSetup;

	// Get the player
	int iPlayerEntIndex = engine->GetLocalPlayer();

	C_BaseEntity *pPlayer = g_pBaseEntityList->GetEntityByIndex( iPlayerEntIndex );

	if ( pPlayer )
	{
		viewSetup.origin = pPlayer->GetAbsOrigin() + pPlayer->GetViewOffset();
		viewSetup.angles = pPlayer->GetAbsAngles();
	}
	else
	{
		viewSetup.origin.Init();
		viewSetup.angles.Init();
	}
	
	viewSetup.fov = 90;
	viewSetup.zNear = 7;
	viewSetup.zFar = 28400;
	viewSetup.x = rect->x;
	viewSetup.y = rect->y;
	viewSetup.width = rect->width;
	viewSetup.height = rect->height;
	viewSetup.m_flAspectRatio = (float)rect->width / (float)rect->height;

	Frustum frustum;

	render->SetMainView( viewSetup.origin, viewSetup.angles );

	render->SceneBegin();
	render->ViewSetupVis( false, 1, &viewSetup.origin );
	render->Push3DView( viewSetup, VIEW_CLEAR_COLOR, NULL, frustum );
	render->BeginUpdateLightmaps();

	IWorldRenderList *pList = render->CreateWorldList();
	WorldListInfo_t pListInfo;

	render->BuildWorldLists( pList, &pListInfo, -1 );
	
	render->EndUpdateLightmaps();
	
	render->DrawWorldLists( pList, DRAWWORLDLISTS_DRAW_SKYBOX | DRAWWORLDLISTS_DRAW_INTERSECTSWATER | DRAWWORLDLISTS_DRAW_STRICTLYABOVEWATER, 0.0f );
	pList->Release();
	render->PopView( frustum );
	render->SceneEnd();
	
	// VGUI On top
	{
		CViewSetup view2d;
		view2d.x = rect->x;
		view2d.y = rect->y;
		view2d.width = rect->width;
		view2d.height = rect->height;

		render->Push2DView( view2d, 0, NULL, frustum );
		render->VGui_Paint( PAINT_UIPANELS | PAINT_CURSOR );
		render->PopView( frustum );
	}
}
