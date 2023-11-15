#include "cbase.h"
#include "base_servergamedll.h"
#include "base_gameeventlistener.h"
#include "filesystem.h"
#include "igameevents.h"
#include "tier1.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"
#include "engine/ivmodelinfo.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

IFileSystem *filesystem = NULL;
IGameEventManager2 *gameeventmanager = NULL;
IEngineSound *enginesound = NULL;
IVModelInfo *modelinfo = NULL;
IEngineTrace *enginetrace = NULL;


bool CBaseServerGameDLL::DLLInit( CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars *pGlobals )
{
	LOG_STUB();

	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );

	ConnectTier1Libraries( &engineFactory, 1 );
	ConnectTier2Libraries( &engineFactory, 1 );
	ConnectTier3Libraries( &engineFactory, 1 );

	ConVar_Register( FCVAR_GAMEDLL );

	// Connect to things
	if ( ( engine = ( IVEngineServer * )engineFactory( INTERFACEVERSION_VENGINESERVER, NULL ) ) == NULL )
		return false;
	if ( ( filesystem = ( IFileSystem * )engineFactory( FILESYSTEM_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( ( gameeventmanager = ( IGameEventManager2 * )engineFactory( INTERFACEVERSION_GAMEEVENTSMANAGER2, NULL ) ) == NULL )
		return false;
	if ( ( enginesound = ( IEngineSound * )engineFactory( IENGINESOUND_SERVER_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( ( modelinfo = ( IVModelInfo * )engineFactory( VMODELINFO_SERVER_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( ( enginetrace = ( IEngineTrace * )engineFactory( INTERFACEVERSION_ENGINETRACE_SERVER, NULL ) ) == NULL )
		return false;

	g_pGameEventListener->ListenForGameEvents();

	return true;
}


bool CBaseServerGameDLL::ReplayInit( CreateInterfaceFn fnReplayFactory )
{
	LOG_STUB();
	return false;
}


bool CBaseServerGameDLL::GameInit( void )
{
	LOG_STUB();
	return false;
}


bool CBaseServerGameDLL::LevelInit( char const *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background )
{
	LOG_STUB();
	return false;
}


void CBaseServerGameDLL::ServerActivate( edict_t *pEdictList, int edictCount, int clientMax )
{
	LOG_STUB();
}


void CBaseServerGameDLL::GameFrame( bool simulating )
{
	//LOG_STUB();
}


void CBaseServerGameDLL::PreClientUpdate( bool simulating )
{
	//LOG_STUB();
}


void CBaseServerGameDLL::LevelShutdown( void )
{
	LOG_STUB();
}


void CBaseServerGameDLL::GameShutdown( void )
{
	LOG_STUB();
}


void CBaseServerGameDLL::DLLShutdown( void )
{
	LOG_STUB();

	DisconnectTier3Libraries();
	DisconnectTier2Libraries();
	ConVar_Unregister();
	DisconnectTier1Libraries();
}


float CBaseServerGameDLL::GetTickInterval( void ) const
{
	LOG_STUB();

	return DEFAULT_TICK_INTERVAL;
}

ServerClass *CBaseServerGameDLL::GetAllServerClasses( void )
{
	return g_pServerClassHead;
}


const char* CBaseServerGameDLL::GetGameDescription( void )
{
	LOG_STUB();

	return nullptr;
}


void CBaseServerGameDLL::CreateNetworkStringTables( void )
{
	LOG_STUB();
}


CSaveRestoreData *CBaseServerGameDLL::SaveInit( int size )
{
	LOG_STUB();

	return nullptr;
}


void CBaseServerGameDLL::SaveWriteFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount )
{
	LOG_STUB();
}


void CBaseServerGameDLL::SaveReadFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount )
{
	LOG_STUB();
}


void CBaseServerGameDLL::SaveGlobalState( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseServerGameDLL::RestoreGlobalState( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseServerGameDLL::PreSave( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseServerGameDLL::Save( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseServerGameDLL::GetSaveComment( char *comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime )
{
	LOG_STUB();
}


void CBaseServerGameDLL::WriteSaveHeaders( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseServerGameDLL::ReadRestoreHeaders( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseServerGameDLL::Restore( CSaveRestoreData *pSaveData, bool b )
{
	LOG_STUB();
}


bool CBaseServerGameDLL::IsRestoring()
{
	//LOG_STUB();

	return false;
}


int CBaseServerGameDLL::CreateEntityTransitionList( CSaveRestoreData *pSaveData, int a )
{
	LOG_STUB();

	return 0;
}


void CBaseServerGameDLL::BuildAdjacentMapList( void )
{
	LOG_STUB();
}


bool CBaseServerGameDLL::GetUserMessageInfo( int msg_type, char *name, int maxnamelength, int &size )
{
	LOG_STUB();

	return false;
}


CStandardSendProxies *CBaseServerGameDLL::GetStandardSendProxies()
{
	return &g_StandardSendProxies;
}


void CBaseServerGameDLL::PostInit()
{
	LOG_STUB();
}


void CBaseServerGameDLL::Think( bool finalTick )
{
	//LOG_STUB();
}


void CBaseServerGameDLL::PreSaveGameLoaded( char const *pSaveName, bool bCurrentlyInGame )
{
	LOG_STUB();
}


bool CBaseServerGameDLL::ShouldHideServer( void )
{
	LOG_STUB();

	return false;
}


void CBaseServerGameDLL::InvalidateMdlCache()
{
	LOG_STUB();
}


void CBaseServerGameDLL::OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue )
{
	LOG_STUB();
}


void CBaseServerGameDLL::GameServerSteamAPIActivated( void )
{
	LOG_STUB();
}


void CBaseServerGameDLL::GameServerSteamAPIShutdown( void )
{
	LOG_STUB();
}


void CBaseServerGameDLL::SetServerHibernation( bool bHibernating )
{
	LOG_STUB();
}


IServerGCLobby *CBaseServerGameDLL::GetServerGCLobby()
{
	LOG_STUB();

	return nullptr;
}


const char *CBaseServerGameDLL::GetServerBrowserMapOverride()
{
	LOG_STUB();

	return nullptr;
}


const char *CBaseServerGameDLL::GetServerBrowserGameData()
{
	LOG_STUB();

	return nullptr;
}


void CBaseServerGameDLL::Status( void ( *print ) ( const char *fmt, ... ) )
{
	LOG_STUB();
}


void CBaseServerGameDLL::PrepareLevelResources( char *pszMapName, size_t nMapNameSize, char *pszMapFile, size_t nMapFileSize )
{
	LOG_STUB();
}


IServerGameDLL::ePrepareLevelResourcesResult CBaseServerGameDLL::AsyncPrepareLevelResources( char *pszMapName, size_t nMapNameSize, char *pszMapFile, size_t nMapFileSize, float *flProgress )
{
	LOG_STUB();

	if ( flProgress )
	{
		*flProgress = 1.0f;
	}

	return IServerGameDLL::ePrepareLevelResources_Prepared;
}


IServerGameDLL::eCanProvideLevelResult CBaseServerGameDLL::CanProvideLevel( char *pMapName, int nMapNameMax )
{
	LOG_STUB();

	return IServerGameDLL::eCanProvideLevel_CannotProvide;
}


bool CBaseServerGameDLL::IsManualMapChangeOkay( const char **pszReason )
{
	LOG_STUB();

	return true;
}
