#include "cbase.h"
#include "goldsrc_clientdll.h"
#include "goldsrc_c_baseentity.h"
#include "goldsrc_input.h"
#include "ienginevgui.h"
#include "ivrenderview.h"
#include "view_shared.h"

#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include <matsys_controls/matsyscontrols.h>

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineClient *engine;


CGoldSRCClientDLL g_BaseClientDLL;
IBaseClientDLL *g_pClientDLL = &g_BaseClientDLL;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCClientDLL, IBaseClientDLL, CLIENT_DLL_INTERFACE_VERSION, g_BaseClientDLL );


int CGoldSRCClientDLL::Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals )
{
	int nRet = CBaseClientDLL::Init( appSystemFactory, physicsFactory, pGlobals );
	if ( !nRet )
		return false;

	// VGUI Startup
	if ( !vgui::VGui_InitInterfacesList( "CLIENT", &appSystemFactory, 1 ) )
		return false;

	if ( !vgui::VGui_InitMatSysInterfacesList( "CLIENT", &appSystemFactory, 1 ) )
		return false;

	g_pGoldSRCInput->Init();

	return true;
}

void CGoldSRCClientDLL::LevelShutdown( void )
{
	g_pGoldSRCInput->ClearAllLists();
	g_pBaseEntityList->ReleaseAllEntities();
}


void CGoldSRCClientDLL::IN_ActivateMouse( void )
{
	g_pGoldSRCInput->ActivateMouse();
}


void CGoldSRCClientDLL::IN_DeactivateMouse( void )
{
	g_pGoldSRCInput->DeactivateMouse();
}


void CGoldSRCClientDLL::IN_Accumulate( void )
{
	g_pGoldSRCInput->AccumulateMouse();
}


void CGoldSRCClientDLL::IN_ClearStates( void )
{
	g_pGoldSRCInput->ClearMouseState();
}


void CGoldSRCClientDLL::CreateMove( int sequence_number, float input_sample_frametime, bool active )
{
	g_pGoldSRCInput->CreateMove( sequence_number, input_sample_frametime, active );
}

void CGoldSRCClientDLL::ExtraMouseSample( float frametime, bool active )
{
	g_pGoldSRCInput->ExtraMouseSample( frametime, active );
}


bool CGoldSRCClientDLL::WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand )
{
	return g_pGoldSRCInput->WriteUsercmdDeltaToBuffer( buf, from, to, isnewcommand );
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
		//viewSetup.angles = pPlayer->GetAbsAngles();
		engine->GetViewAngles( viewSetup.angles );
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

	AudioState_t audioState;
	audioState.m_Origin = viewSetup.origin;
	audioState.m_Angles = viewSetup.angles;
	audioState.m_bIsUnderwater = false;	// TODO

	engine->SetAudioState( audioState );

	Frustum frustum;

	render->SetMainView( viewSetup.origin, viewSetup.angles );

	render->SceneBegin();

	render->ViewSetupVis( false, 1, &viewSetup.origin );
	render->Push3DView( viewSetup, VIEW_CLEAR_COLOR | VIEW_CLEAR_DEPTH, NULL, frustum );
	render->BeginUpdateLightmaps();

	IWorldRenderList *pList = render->CreateWorldList();
	WorldListInfo_t pListInfo;
	render->BuildWorldLists( pList, &pListInfo, -1 );

	render->EndUpdateLightmaps();
	render->DrawWorldLists( pList, DRAWWORLDLISTS_DRAW_SKYBOX | DRAWWORLDLISTS_DRAW_INTERSECTSWATER | DRAWWORLDLISTS_DRAW_STRICTLYABOVEWATER, 0.0f );
	pList->Release();

	for ( int i = 1; i < NUM_ENT_ENTRIES; i++ )
	{
		C_BaseEntity *pEntity = g_pBaseEntityList->GetEntityByIndex( i );
		
		if ( !pEntity || !pEntity->ShouldDraw() )
			continue;

		render->SetBlend( 1.0 );
		pEntity->DrawModel( 1 /*STUDIO_RENDER*/ );
	}

	render->Draw3DDebugOverlays();

	engine->DrawPortals();

	render->SceneEnd();

	render->DrawLights();

	render->PopView( frustum );
	
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

void CGoldSRCClientDLL::FrameStageNotify( ClientFrameStage_t curStage )
{
	//if ( curStage == FRAME_START )
	//{
	//	Msg( "--FRAME START--\n" );
	//}
}
