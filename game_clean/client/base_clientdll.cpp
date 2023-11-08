#include "cbase.h"
#include <crtmemdebug.h>
#include "base_clientdll.h"
#include "tier1.h"
#include "tier2/tier2.h"
#include "tier3/tier3.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


int CBaseClientDLL::Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals )
{
	InitCRTMemDebug();
	MathLib_Init( 2.2f, 2.2f, 0.0f, 2.0f );

	ConnectTier1Libraries( &appSystemFactory, 1 );
	ConnectTier2Libraries( &appSystemFactory, 1 );
	ConnectTier3Libraries( &appSystemFactory, 1 );

	ConVar_Register( FCVAR_CLIENTDLL );

	return true;
}


void CBaseClientDLL::PostInit()
{
}


void CBaseClientDLL::Shutdown( void )
{
	DisconnectTier2Libraries();
	ConVar_Unregister();
	DisconnectTier1Libraries();
}


bool CBaseClientDLL::ReplayInit( CreateInterfaceFn fnReplayFactory )
{
	return false;
}


bool CBaseClientDLL::ReplayPostInit()
{
	return false;
}


void CBaseClientDLL::LevelInitPreEntity( char const *pMapName )
{
}


void CBaseClientDLL::LevelInitPostEntity()
{
}


void CBaseClientDLL::LevelShutdown( void )
{
}


ClientClass *CBaseClientDLL::GetAllClasses( void )
{
	return nullptr;
}


int CBaseClientDLL::HudVidInit( void )
{
	return 0;
}


void CBaseClientDLL::HudProcessInput( bool bActive )
{
}


void CBaseClientDLL::HudUpdate( bool bActive )
{
}


void CBaseClientDLL::HudReset( void )
{
}


void CBaseClientDLL::HudText( const char *message )
{
}


void CBaseClientDLL::IN_ActivateMouse( void )
{
}


void CBaseClientDLL::IN_DeactivateMouse( void )
{
}


void CBaseClientDLL::IN_Accumulate( void )
{
}


void CBaseClientDLL::IN_ClearStates( void )
{
}


bool CBaseClientDLL::IN_IsKeyDown( const char *name, bool &isdown )
{
	return false;
}


void CBaseClientDLL::IN_OnMouseWheeled( int nDelta )
{
}


int CBaseClientDLL::IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding )
{
	return 0;
}


void CBaseClientDLL::CreateMove( int sequence_number, float input_sample_frametime, bool active )
{
}


void CBaseClientDLL::ExtraMouseSample( float frametime, bool active )
{
}


bool CBaseClientDLL::WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand )
{
	return false;
}


void CBaseClientDLL::EncodeUserCmdToBuffer( bf_write &buf, int slot )
{
}


void CBaseClientDLL::DecodeUserCmdFromBuffer( bf_read &buf, int slot )
{
}


void CBaseClientDLL::View_Render( vrect_t *rect )
{
}


void CBaseClientDLL::RenderView( const CViewSetup &view, int nClearFlags, int whatToDraw )
{
}


void CBaseClientDLL::View_Fade( ScreenFade_t *pSF )
{
}


void CBaseClientDLL::SetCrosshairAngle( const QAngle &angle )
{
}


void CBaseClientDLL::InitSprite( CEngineSprite *pSprite, const char *loadname )
{
}


void CBaseClientDLL::ShutdownSprite( CEngineSprite *pSprite )
{
}


int CBaseClientDLL::GetSpriteSize( void ) const
{
	return 0;
}


void CBaseClientDLL::VoiceStatus( int entindex, qboolean bTalking )
{
}


void CBaseClientDLL::InstallStringTableCallback( char const *tableName )
{
}


void CBaseClientDLL::FrameStageNotify( ClientFrameStage_t curStage )
{
}


bool CBaseClientDLL::DispatchUserMessage( int msg_tye, bf_read &msg_data )
{
	return false;
}


CSaveRestoreData* CBaseClientDLL::SaveInit( int size )
{
	return nullptr;
}


void CBaseClientDLL::SaveWriteFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount )
{
}


void CBaseClientDLL::SaveReadFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount )
{
}


void CBaseClientDLL::PreSave( CSaveRestoreData *pSaveData )
{
}


void CBaseClientDLL::Save( CSaveRestoreData *pSaveData )
{
}


void CBaseClientDLL::WriteSaveHeaders( CSaveRestoreData *pSaveData )
{
}


void CBaseClientDLL::ReadRestoreHeaders( CSaveRestoreData *pSaveData )
{
}


void CBaseClientDLL::Restore( CSaveRestoreData *pSaveData, bool b )
{
}


void CBaseClientDLL::DispatchOnRestore()
{
}


CStandardRecvProxies *CBaseClientDLL::GetStandardRecvProxies()
{
	return nullptr;
}


void CBaseClientDLL::WriteSaveGameScreenshot( const char *pFilename )
{
}


void CBaseClientDLL::EmitSentenceCloseCaption( char const *tokenstream )
{
}


void CBaseClientDLL::EmitCloseCaption( char const *captionname, float duration )
{
}


bool CBaseClientDLL::CanRecordDemo( char *errorMsg, int length ) const
{
	return false;
}


void CBaseClientDLL::OnDemoRecordStart( char const *pDemoBaseName )
{
}


void CBaseClientDLL::OnDemoRecordStop()
{
}


void CBaseClientDLL::OnDemoPlaybackStart( char const *pDemoBaseName )
{
}


void CBaseClientDLL::OnDemoPlaybackStop()
{
}


bool CBaseClientDLL::ShouldDrawDropdownConsole()
{
	return true;
}


int CBaseClientDLL::GetScreenWidth()
{
	return 0;
}


int CBaseClientDLL::GetScreenHeight()
{
	return 0;
}


void CBaseClientDLL::WriteSaveGameScreenshotOfSize( const char *pFilename, int width, int height, bool bCreatePowerOf2Padded, bool bWriteVTF )
{
}


bool CBaseClientDLL::GetPlayerView( CViewSetup &playerView )
{
	return false;
}


void CBaseClientDLL::SetupGameProperties( CUtlVector< XUSER_CONTEXT > &contexts, CUtlVector< XUSER_PROPERTY > &properties )
{
}


uint CBaseClientDLL::GetPresenceID(const char *pIDName )
{
	return 0;
}


const char *CBaseClientDLL::GetPropertyIdString( const uint id )
{
	return nullptr;
}


void CBaseClientDLL::GetPropertyDisplayString( uint id, uint value, char* pOutput, int nBytes )
{
}


void CBaseClientDLL::StartStatsReporting( HANDLE handle, bool bArbitrated )
{
}


void CBaseClientDLL::InvalidateMdlCache()
{
}


void CBaseClientDLL::IN_SetSampleTime( float frametime )
{
}


void CBaseClientDLL::ReloadFilesInList( IFileList *pFilesToReload )
{
}


bool CBaseClientDLL::HandleUiToggle()
{
	return false;
}


bool CBaseClientDLL::ShouldAllowConsole()
{
	return true;
}


CRenamedRecvTableInfo *CBaseClientDLL::GetRenamedRecvTableInfos()
{
	return nullptr;
}


CMouthInfo *CBaseClientDLL::GetClientUIMouthInfo()
{
	return nullptr;
}


void CBaseClientDLL::FileReceived( const char *fileName, unsigned int transferID )
{
}


const char *CBaseClientDLL::TranslateEffectForVisionFilter( const char *pchEffectType, const char *pchEffectName )
{
	return nullptr;
}


void CBaseClientDLL::ClientAdjustStartSoundParams( struct StartSoundParams_t &params )
{
}


bool CBaseClientDLL::DisconnectAttempt( void )
{
	return false;
}

bool CBaseClientDLL::IsConnectedUserInfoChangeAllowed( IConVar *pCvar )
{
	return false;
}
