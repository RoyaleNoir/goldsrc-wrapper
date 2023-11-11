#include "cbase.h"
#include "goldsrc_servergamedll.h"
#include "goldsrc_servergameents.h"
#include "filesystem.h"
#include "goldsrc_cvars.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"
#include "goldsrc_globalvars.h"
#include "goldsrc_mapentities.h"

#if defined( _WIN32 )
#include "winlite.h"
#endif

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CGoldSRCServerGameDLL g_ServerGameDLL;
CBaseServerGameDLL *g_pServerGameDLL = g_pServerGameDLL;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCServerGameDLL, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL, g_ServerGameDLL );

CSharedEdictChangeInfo* g_pSharedChangeInfo = NULL;

IChangeInfoAccessor *CBaseEdict::GetChangeAccessor()
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}

const IChangeInfoAccessor *CBaseEdict::GetChangeAccessor() const
{
	return engine->GetChangeAccessor( (const edict_t *)this );
}


static CSysModule *pActualServerDLL = NULL;	// The server DLL being wrapped

CGlobalVars *gpGlobals = NULL;

GoldSRC::DLL_FUNCTIONS g_GoldSRCEntityInterface = {};
GoldSRC::DLL_FUNCTIONS *g_pGoldSRCEntityInterface = &g_GoldSRCEntityInterface;


GoldSRC::pfnCreateEntityFunc_t GetCreateEntityFunc( const char *szClassName )
{
	if ( !pActualServerDLL )
		return NULL;

	GoldSRC::pfnCreateEntityFunc_t pfnEntityFunc = NULL;

#ifdef WIN32
	pfnEntityFunc = ( GoldSRC::pfnCreateEntityFunc_t )GetProcAddress( ( HMODULE )pActualServerDLL, szClassName );
#else
	pfnEntityFunc = ( GoldSRC::pfnCreateEntityFunc_t )dlsym( ( void * )pActualServerDLL, szClassName );
#endif

	return pfnEntityFunc;
}


bool CGoldSRCServerGameDLL::DLLInit( CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars *pGlobals )
{
	bool bRet = CBaseServerGameDLL::DLLInit( engineFactory, physicsFactory, fileSystemFactory, pGlobals );
	if ( !bRet )
		return false;

	gpGlobals = pGlobals;
	GoldSRCGlobalVars_Init();

	g_pSharedChangeInfo = engine->GetSharedEdictChangeInfo();

	if ( !pActualServerDLL )
	{
		pActualServerDLL = g_pFullFileSystem->LoadModule( "dlls/hl" DLL_EXT_STRING, "GAMEBIN" );

		if ( !pActualServerDLL )
			return false;

		Msg( "GoldSRC server dll loaded\n" );
	}

	GoldSRC::pfnGiveFnptrsToDll_t pfnGiveFnptrsToDll = NULL;

#ifdef WIN32
	pfnGiveFnptrsToDll = ( GoldSRC::pfnGiveFnptrsToDll_t )GetProcAddress( ( HMODULE )pActualServerDLL, "GiveFnptrsToDll" );
#else
	pfnGiveFnptrsToDll = ( GoldSRC::pfnGiveFnptrsToDll_t )dlsym( ( void * )pActualServerDLL, "GiveFnptrsToDll" );
#endif

	if ( !pfnGiveFnptrsToDll )
		return false;

	pfnGiveFnptrsToDll( g_pGoldSRCEngineFuncs, g_pGoldSRCGlobalVars );
	Msg( "GiveFnptrsToDll()\n" );

	GoldSRC::pfnGetEntityAPI_t pfnGetEntityAPI = NULL;

#ifdef WIN32
	pfnGetEntityAPI = ( GoldSRC::pfnGetEntityAPI_t )GetProcAddress( ( HMODULE )pActualServerDLL, "GetEntityAPI" );
#else
	pfnGetEntityAPI = ( GoldSRC::pfnGetEntityAPI_t )dlsym( ( void * )pActualServerDLL, "GetEntityAPI" );
#endif

	if ( !pfnGetEntityAPI )
		return false;

	if ( !pfnGetEntityAPI( g_pGoldSRCEntityInterface, GOLDSRC_EIFACE_VERSION ) )
		return false;

	Msg( "GoldSRC entity interface connected.\n" );

	g_pGoldSRCCVars->Init();
	g_pGoldSRCEdict->Init();

	// GameInit() seems to be called here
	g_pGoldSRCEntityInterface->GameInit();

	return true;
}

void CGoldSRCServerGameDLL::DLLShutdown( void )
{
	g_pGoldSRCCVars->Shutdown();

	if ( pActualServerDLL )
	{
		g_pFullFileSystem->UnloadModule( pActualServerDLL );
		pActualServerDLL = NULL;
	}

	CBaseServerGameDLL::DLLShutdown();
}


void CGoldSRCServerGameDLL::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	GoldSRCGlobalVars_Update();

	g_pGoldSRCEntityInterface->ServerActivate( NULL, 0, 0 );
}


void CGoldSRCServerGameDLL::GameShutdown( void )
{
	g_pGoldSRCEntityInterface->ServerDeactivate();
	g_pGoldSRCServerGameEnts->ReleaseAll();
	g_pGoldSRCEdict->EnsureAllFreed();
	GoldSRC::FreeStrings();
}


bool CGoldSRCServerGameDLL::LevelInit( const char *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background )
{
	GoldSRCGlobalVars_Update();

	if ( loadGame )
	{
	}
	else
	{
		ParseMapEntities( pMapEntities );
	}
	return true;
}


void CGoldSRCServerGameDLL::LevelShutdown( void )
{
}


void CGoldSRCServerGameDLL::GameFrame( bool simulating )
{
	g_pGoldSRCEntityInterface->StartFrame();
}


const char *CGoldSRCServerGameDLL::GetGameDescription( void )
{
	// Engine checks this before the dll is loaded, so need to check for that case
	if ( !g_pGoldSRCEntityInterface->GetGameDescription )
		return NULL;

	return g_pGoldSRCEntityInterface->GetGameDescription();
}
