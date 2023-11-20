#include "cbase.h"
#include "base_clientdll.h"
#include "crtmemdebug.h"
#include "engine/ivmodelinfo.h"
#include "ivrenderview.h"
#include "engine/ivmodelrender.h"
#include "tier1.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


IVEngineClient *engine = NULL;
IVRenderView *render = NULL;
IVModelInfoClient *modelinfo = NULL;
IVModelRender *modelrender = NULL;
CGlobalVarsBase *gpGlobals = NULL;
IFileSystem *filesystem = NULL;


int CBaseClientDLL::Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals )
{
	LOG_STUB();

	InitCRTMemDebug();
	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );

	ConnectTier1Libraries( &appSystemFactory, 1 );
	ConnectTier2Libraries( &appSystemFactory, 1 );
	ConnectTier3Libraries( &appSystemFactory, 1 );

	ConVar_Register( FCVAR_CLIENTDLL );

	gpGlobals = pGlobals;

	if ( (engine = (IVEngineClient *)appSystemFactory( VENGINE_CLIENT_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( (render = (IVRenderView *)appSystemFactory( VENGINE_RENDERVIEW_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( (modelinfo = (IVModelInfoClient *)appSystemFactory( VMODELINFO_CLIENT_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( (modelrender = (IVModelRender *)appSystemFactory( VENGINE_HUDMODEL_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;
	if ( (filesystem = (IFileSystem *)appSystemFactory( FILESYSTEM_INTERFACE_VERSION, NULL ) ) == NULL )
		return false;

	return true;
}


void CBaseClientDLL::PostInit()
{
	LOG_STUB();
}


void CBaseClientDLL::Shutdown( void )
{
	LOG_STUB();

	DisconnectTier2Libraries();
	ConVar_Unregister();
	DisconnectTier1Libraries();
}


bool CBaseClientDLL::ReplayInit( CreateInterfaceFn fnReplayFactory )
{
	LOG_STUB();

	return false;
}


bool CBaseClientDLL::ReplayPostInit()
{
	LOG_STUB();

	return false;
}


void CBaseClientDLL::LevelInitPreEntity( char const *pMapName )
{
	LOG_STUB();
}


void CBaseClientDLL::LevelInitPostEntity()
{
	LOG_STUB();
}


void CBaseClientDLL::LevelShutdown( void )
{
	LOG_STUB();
}


ClientClass *CBaseClientDLL::GetAllClasses( void )
{
	return g_pClientClassHead;
}


int CBaseClientDLL::HudVidInit( void )
{
	LOG_STUB();

	return 0;
}


void CBaseClientDLL::HudProcessInput( bool bActive )
{
	//LOG_STUB();
}


void CBaseClientDLL::HudUpdate( bool bActive )
{
	//LOG_STUB();
}


void CBaseClientDLL::HudReset( void )
{
	LOG_STUB();
}


void CBaseClientDLL::HudText( const char *message )
{
	LOG_STUB();
}


void CBaseClientDLL::IN_ActivateMouse( void )
{
	//LOG_STUB();
}


void CBaseClientDLL::IN_DeactivateMouse( void )
{
	//LOG_STUB();
}


void CBaseClientDLL::IN_Accumulate( void )
{
	//LOG_STUB();
}


void CBaseClientDLL::IN_ClearStates( void )
{
	//LOG_STUB();
}


bool CBaseClientDLL::IN_IsKeyDown( const char *name, bool &isdown )
{
	//LOG_STUB();

	return false;
}


void CBaseClientDLL::IN_OnMouseWheeled( int nDelta )
{
	//LOG_STUB();
}


int CBaseClientDLL::IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	//LOG_STUB();

	return 1;	// Make engine process it
}


void CBaseClientDLL::CreateMove( int sequence_number, float input_sample_frametime, bool active )
{
	//LOG_STUB();
}


void CBaseClientDLL::ExtraMouseSample( float frametime, bool active )
{
	//LOG_STUB();
}


bool CBaseClientDLL::WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand )
{
	//LOG_STUB();

	return false;
}


void CBaseClientDLL::EncodeUserCmdToBuffer( bf_write &buf, int slot )
{
	LOG_STUB();
}


void CBaseClientDLL::DecodeUserCmdFromBuffer( bf_read &buf, int slot )
{
	LOG_STUB();
}


void CBaseClientDLL::View_Render( vrect_t *rect )
{
	//LOG_STUB();
}


void CBaseClientDLL::RenderView( const CViewSetup &view, int nClearFlags, int whatToDraw )
{
	LOG_STUB();
}


void CBaseClientDLL::View_Fade( ScreenFade_t *pSF )
{
	LOG_STUB();
}


void CBaseClientDLL::SetCrosshairAngle( const QAngle &angle )
{
	LOG_STUB();
}


void CBaseClientDLL::InitSprite( CEngineSprite *pSprite, const char *loadname )
{
	LOG_STUB();
}


void CBaseClientDLL::ShutdownSprite( CEngineSprite *pSprite )
{
	LOG_STUB();
}


int CBaseClientDLL::GetSpriteSize( void ) const
{
	LOG_STUB();

	return 0;
}


void CBaseClientDLL::VoiceStatus( int entindex, qboolean bTalking )
{
	LOG_STUB();
}


void CBaseClientDLL::InstallStringTableCallback( char const *tableName )
{
	LOG_STUB();
}


void CBaseClientDLL::FrameStageNotify( ClientFrameStage_t curStage )
{
	//LOG_STUB();
}


bool CBaseClientDLL::DispatchUserMessage( int msg_tye, bf_read &msg_data )
{
	LOG_STUB();

	return false;
}


CSaveRestoreData *CBaseClientDLL::SaveInit( int size )
{
	LOG_STUB();

	return nullptr;
}


void CBaseClientDLL::SaveWriteFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount )
{
	LOG_STUB();
}


void CBaseClientDLL::SaveReadFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount )
{
	LOG_STUB();
}


void CBaseClientDLL::PreSave( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseClientDLL::Save( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseClientDLL::WriteSaveHeaders( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseClientDLL::ReadRestoreHeaders( CSaveRestoreData *pSaveData )
{
	LOG_STUB();
}


void CBaseClientDLL::Restore( CSaveRestoreData *pSaveData, bool b )
{
	LOG_STUB();
}


void CBaseClientDLL::DispatchOnRestore()
{
	LOG_STUB();
}


CStandardRecvProxies *CBaseClientDLL::GetStandardRecvProxies()
{
	return &g_StandardRecvProxies;
}


void CBaseClientDLL::WriteSaveGameScreenshot( const char *pFilename )
{
	LOG_STUB();
}


void CBaseClientDLL::EmitSentenceCloseCaption( char const *tokenstream )
{
	LOG_STUB();
}


void CBaseClientDLL::EmitCloseCaption( char const *captionname, float duration )
{
	LOG_STUB();
}


bool CBaseClientDLL::CanRecordDemo( char *errorMsg, int length ) const
{
	LOG_STUB();

	return false;
}


void CBaseClientDLL::OnDemoRecordStart( char const *pDemoBaseName )
{
	LOG_STUB();
}


void CBaseClientDLL::OnDemoRecordStop()
{
	LOG_STUB();
}


void CBaseClientDLL::OnDemoPlaybackStart( char const *pDemoBaseName )
{
	LOG_STUB();
}


void CBaseClientDLL::OnDemoPlaybackStop()
{
	LOG_STUB();
}


bool CBaseClientDLL::ShouldDrawDropdownConsole()
{
	//LOG_STUB();

	return true;
}


int CBaseClientDLL::GetScreenWidth()
{
	LOG_STUB();

	return 0;
}


int CBaseClientDLL::GetScreenHeight()
{
	LOG_STUB();

	return 0;
}


void CBaseClientDLL::WriteSaveGameScreenshotOfSize( const char *pFilename, int width, int height, bool bCreatePowerOf2Padded, bool bWriteVTF )
{
	LOG_STUB();
}


bool CBaseClientDLL::GetPlayerView( CViewSetup &playerView )
{
	LOG_STUB();

	return false;
}


void CBaseClientDLL::SetupGameProperties( CUtlVector< XUSER_CONTEXT > &contexts, CUtlVector< XUSER_PROPERTY > &properties )
{
	LOG_STUB();
}


uint CBaseClientDLL::GetPresenceID( const char *pIDName )
{
	LOG_STUB();

	return 0;
}


const char *CBaseClientDLL::GetPropertyIdString( const uint id )
{
	LOG_STUB();

	return nullptr;
}


void CBaseClientDLL::GetPropertyDisplayString( uint id, uint value, char* pOutput, int nBytes )
{
	LOG_STUB();
}


void CBaseClientDLL::StartStatsReporting( HANDLE handle, bool bArbitrated )
{
	LOG_STUB();
}


void CBaseClientDLL::InvalidateMdlCache()
{
	LOG_STUB();
}


void CBaseClientDLL::IN_SetSampleTime( float frametime )
{
	//LOG_STUB();
}


void CBaseClientDLL::ReloadFilesInList( IFileList *pFilesToReload )
{
	LOG_STUB();
}


bool CBaseClientDLL::HandleUiToggle()
{
	LOG_STUB();

	return false;
}


bool CBaseClientDLL::ShouldAllowConsole()
{
	LOG_STUB();

	return true;
}


CRenamedRecvTableInfo *CBaseClientDLL::GetRenamedRecvTableInfos()
{
	LOG_STUB();

	return nullptr;
}


CMouthInfo *CBaseClientDLL::GetClientUIMouthInfo()
{
	LOG_STUB();

	return nullptr;
}


void CBaseClientDLL::FileReceived( const char *fileName, unsigned int transferID )
{
	LOG_STUB();
}


const char *CBaseClientDLL::TranslateEffectForVisionFilter( const char *pchEffectType, const char *pchEffectName )
{
	LOG_STUB();

	return pchEffectName;
}


void CBaseClientDLL::ClientAdjustStartSoundParams( struct StartSoundParams_t &params )
{
	LOG_STUB();
}


bool CBaseClientDLL::DisconnectAttempt( void )
{
	LOG_STUB();

	return false;
}

bool CBaseClientDLL::IsConnectedUserInfoChangeAllowed( IConVar *pCvar )
{
	LOG_STUB();

	return false;
}
