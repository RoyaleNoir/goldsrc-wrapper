#include "cbase.h"
#include "goldsrc_clientdll.h"
#include "goldsrc_cdll_int.h"
#include "goldsrc_c_baseentity.h"
#include "goldsrc_input.h"
#include "goldsrc_render.h"
#include "goldsrc_usermessages_shared.h"
#include "ienginevgui.h"
#include "ivrenderview.h"
#include "view_shared.h"
#include "filesystem.h"
#include "globalvars_base.h"
#include "vgui_stub/vgui_stub.h"
#include "networkstringtabledefs.h"

#include <vgui/IVGui.h>
#include <vgui_controls/Panel.h>
#include <matsys_controls/matsyscontrols.h>

#include "winlite.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IVGUIStub *vguistub = NULL;

extern IVEngineClient *engine;
extern CGlobalVarsBase *gpGlobals;
INetworkStringTableContainer *networkstringtable = NULL;

static CSysModule *pParticleManDLL = NULL;
static CSysModule *pFakeVGUI1DLL = NULL;
static CSysModule *pActualClientDLL = NULL;

CGoldSRCClientDLL g_BaseClientDLL;
IBaseClientDLL *g_pClientDLL = &g_BaseClientDLL;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCClientDLL, IBaseClientDLL, CLIENT_DLL_INTERFACE_VERSION, g_BaseClientDLL );


int CGoldSRCClientDLL::Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals )
{
	int nRet = CBaseClientDLL::Init( appSystemFactory, physicsFactory, pGlobals );
	if ( !nRet )
		return false;

	if ( (networkstringtable = (INetworkStringTableContainer *)appSystemFactory( INTERFACENAME_NETWORKSTRINGTABLECLIENT, NULL ) ) == NULL )
		return false;

	// Load ParticleMan
	if ( !pParticleManDLL )
	{
		pParticleManDLL = g_pFullFileSystem->LoadModule( "cl_dlls/particleman" DLL_EXT_STRING, "GAMEBIN" );
	
		if ( !pParticleManDLL )
		{
			Error( "Failed to load particleman dll\n" );
			return false;
		}
	
		Msg( "GoldSRC particleman dll loaded\n" );
	}

	// Load VGUI Stub
	if ( !pFakeVGUI1DLL )
	{
		pFakeVGUI1DLL = g_pFullFileSystem->LoadModule( "vgui" DLL_EXT_STRING, "GAMEBIN" );
	
		if ( !pFakeVGUI1DLL )
		{
			Error( "Failed to load vgui1 stub dll\n" );
			return false;
		}
	
		Msg( "GoldSRC vgui1 stub dll loaded\n" );
	}

	pfnGetVGUIStub_t pfnGetVGUIStub;

#ifdef WIN32
	pfnGetVGUIStub = ( pfnGetVGUIStub_t )GetProcAddress( ( HMODULE )pFakeVGUI1DLL, "GetVGUIStub" );
#else
	pfnGetVGUIStub = (pfnGetVGUIStub_t)dlsym( ( void * )pFakeVGUI1DLL, "GetVGUIStub" );
#endif

	if ( !pfnGetVGUIStub )
	{
		Error( "Couldn't find VGUI Stub interface!" );
		return false;
	}

	vguistub = pfnGetVGUIStub();

	// Load GoldSRC Client DLL
	if ( !pActualClientDLL )
	{
		pActualClientDLL = Sys_LoadModule( "C:\\Users\\jackc\\Documents\\Programming\\Source\\EmptySource\\game\\goldsrc\\bin\\cl_dlls\\client.dll");

		if ( !pActualClientDLL )
			return false;
	
		Msg( "GoldSRC client dll loaded\n" );
	}

	GoldSRC::pfnClientDllGiveFuncs_t pfnClientFllGiveFuncs;

#ifdef WIN32
	pfnClientFllGiveFuncs = ( GoldSRC::pfnClientDllGiveFuncs_t )GetProcAddress( ( HMODULE )pActualClientDLL, "F" );
#else
	pfnClientFllGiveFuncs = ( GoldSRC::pfnClientDllGiveFuncs_t )dlsym( ( void * )pActualClientDLL, "F" );
#endif

	if ( !pfnClientFllGiveFuncs )
	{
		Error( "Couldn't find F() in client dll!" );
		return false;
	}

	pfnClientFllGiveFuncs( ClientFuncs() );
	if ( !ClientFuncs()->Initialize( EngineFuncs(), GOLDSRC_CLDLL_INTERFACE_VERSION ) )
	{
		Error( "Client DLL doesn't support interface version %d\n", GOLDSRC_CLDLL_INTERFACE_VERSION );
		return false;
	}

	// VGUI Startup
	if ( !vgui::VGui_InitInterfacesList( "CLIENT", &appSystemFactory, 1 ) )
		return false;

	if ( !vgui::VGui_InitMatSysInterfacesList( "CLIENT", &appSystemFactory, 1 ) )
		return false;

	vguistub->Init();

	g_pGoldSRCInput->Init();

	ClientFuncs()->HUD_Init();

	return true;
}

void CGoldSRCClientDLL::Shutdown( void )
{
	ClientFuncs()->HUD_Shutdown();

	vguistub->Shutdown();

	if ( pActualClientDLL )
	{
		g_pFullFileSystem->UnloadModule( pActualClientDLL );
		pActualClientDLL = NULL;
	}

	if ( pFakeVGUI1DLL )
	{
		g_pFullFileSystem->UnloadModule( pFakeVGUI1DLL );
		pFakeVGUI1DLL = NULL;
	}

	if ( pParticleManDLL )
	{
		g_pFullFileSystem->UnloadModule( pParticleManDLL );
		pParticleManDLL = NULL;
	}

	CBaseClientDLL::Shutdown();
}


void CGoldSRCClientDLL::LevelInitPreEntity( char const *pMapName )
{
}


void CGoldSRCClientDLL::LevelShutdown( void )
{
	g_pGoldSRCInput->ClearAllLists();
	g_pBaseEntityList->ReleaseAllEntities();
}

int CGoldSRCClientDLL::HudVidInit(void)
{
	return ClientFuncs()->HUD_VidInit();
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

	int iPlayerEntIndex = engine->GetLocalPlayer();
	C_BaseEntity *pPlayer = g_pBaseEntityList->GetEntityByIndex( iPlayerEntIndex );

	if ( pPlayer )
	{
		QAngle viewAngles;
		engine->GetViewAngles( viewAngles );

		GoldSRC::ref_params_t params = {};
		//vieworg - handled
		//viewangles - handled
		//right - handled
		//up - handled
		params.frametime = gpGlobals->frametime;
		params.curtime = gpGlobals->curtime;
		//intermission
		//paused
		//spectator
		//onground
		//waterlevel
		VectorCopy( pPlayer->GetVelocity().Base(), params.simvel );
		VectorCopy( pPlayer->GetAbsOrigin().Base(), params.simorg );
		VectorCopy( pPlayer->GetViewOffset().Base(), params.viewheight );
		//idealpitch
		VectorCopy( viewAngles.Base(), params.cl_viewangles );
		//health
		//crosshairangle
		//viewsize
		//punchangle
		params.maxclients = gpGlobals->maxClients;
		//demoplayback
		//hardware
		//smoothing
		//cmd
		//movevars
		//viewport
		//nextview
		//onlyClientDraw

		ClientFuncs()->V_CalcRefdef( &params );

		VectorCopy( params.vieworg, viewSetup.origin.Base() );
		VectorCopy( params.viewangles, viewSetup.angles.Base() );
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

		int flags = 1;	// STUDIO_RENDER

		float blend = pEntity->GetFxBlend();

		if ( blend == 0.0f )
			blend = 255.0f;

		if ( blend < 255.0f )
			flags |= 0x80000000;	// STUDIO_TRANSPARENCY

		render->SetBlend( blend / 255.0f );

		float color[3];
		pEntity->GetColorModulation( color );
		render->SetColorModulation( color );

		pEntity->DrawModel( flags );
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

		ClientFuncs()->HUD_Redraw( gpGlobals->curtime, 0 );

		render->VGui_Paint( PAINT_UIPANELS | PAINT_CURSOR );
		render->PopView( frustum );
	}
}


void CGoldSRCClientDLL::InstallStringTableCallback( char const *tableName )
{
	if ( UserMessages()->InstallStringTableCallback( tableName ) )
		return;
}


bool CGoldSRCClientDLL::DispatchUserMessage( int msg_type, bf_read &msg_data )
{
	return UserMessages()->Dispatch( msg_type, msg_data );
}


static void OnRenderStart()
{
	GoldSRC::client_data_t cdata;
	cdata.iWeaponBits = ( 1 << 31 );
	cdata.fov = 90;

	ClientFuncs()->HUD_UpdateClientData( &cdata, gpGlobals->frametime );
}


void CGoldSRCClientDLL::FrameStageNotify( ClientFrameStage_t curStage )
{
	if ( curStage == FRAME_START )
	{
		OnRenderStart();
	}
}
