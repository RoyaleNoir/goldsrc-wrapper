#include "cbase.h"
#include "goldsrc_cdll_int.h"
#include "goldsrc_c_baseentity.h"
#include "goldsrc_cvars_shared.h"
#include "goldsrc_sprites.h"
#include "goldsrc_usermessages_shared.h"
#include "filesystem.h"
#include "vgui_stub/vgui_stub.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IFileSystem *filesystem;

//#undef LOG_STUB
//#define LOG_STUB() DevMsg( "[CLIENT GOLDSRC STUB] %s\n", __FUNCTION__ );

namespace GoldSRC
{
	// DEMO API

	int IsRecording( void )
	{
		return 0;
	}

	int IsPlayingBack( void )
	{
		return 0;
	}

	int IsTimeDemo( void )
	{
		return 0;
	}

	void WriteBuffer( int size, unsigned char *buffer )
	{
	}

	// ENGINE API

	HSPRITE SPR_Load( const char *szPicName )
	{
		LOG_STUB();
		return 0;
	}

	int SPR_Frames( HSPRITE hPic )
	{
		LOG_STUB();
		return 0;
	}

	int SPR_Height( HSPRITE hPic, int frame )
	{
		LOG_STUB();
		return 0;
	}

	int SPR_Width( HSPRITE hPic, int frame )
	{
		LOG_STUB();
		return 0;
	}

	void SPR_Set( HSPRITE hPic, int r, int g, int b )
	{
		LOG_STUB();
	}

	void SPR_Draw( int frame, int x, int y, const wrect_t *prc )
	{
		LOG_STUB();
	}
	
	void SPR_DrawHoles( int frame, int x, int y, const wrect_t *prc )
	{
		LOG_STUB();
	}
	
	void SPR_DrawAdditive( int frame, int x, int y, const wrect_t *prc )
	{
		LOG_STUB();
	}
	
	void SPR_EnableScissor( int x, int y, int width, int height )
	{
		LOG_STUB();
	}
	
	void SPR_DisableScissor( void )
	{
		LOG_STUB();
	}
	
	client_sprite_t *SPR_GetList( char *psz, int *piCount )
	{
		return SpriteManager()->GetList( psz, piCount );
	}
	
	void FillRGBA( int x, int y, int width, int height, int r, int g, int b, int a )
	{
		LOG_STUB();
	}
	
	int GetScreenInfo( SCREENINFO *pscrinfo )
	{
		LOG_STUB();
		return 0;
	}
	
	void SetCrosshair( HSPRITE hspr, wrect_t rc, int r, int g, int b )
	{
		LOG_STUB();
	}
	
	cvar_t *RegisterVariable( char *szName, char *szValue, int flags )
	{
		return g_pGoldSRCCVars->RegisterVariable( szName, szValue, flags );
	}
	
	float GetCvarFloat( char *szName )
	{
		return g_pGoldSRCCVars->GetFloat( szName );
	}
	
	char *GetCvarString( char *szName )
	{
		return (char*)g_pGoldSRCCVars->GetString( szName );
	}
	
	int AddCommand( char *cmd_name, void ( *pfnEngSrc_function )( void ) )
	{
		LOG_STUB();
		return 0;
	}
	
	int HookUserMsg( char *szMsgName, pfnUserMsgHook hook )
	{
		return UserMessages()->HookUserMsg( szMsgName, hook );
	}
	
	int ServerCmd( char *szCmdString )
	{
		LOG_STUB();
		return 0;
	}
	
	int ClientCmd( char *szCmdString )
	{
		LOG_STUB();
		return 0;
	}
	
	void PrimeMusicStream( char *szFilename, int looping )
	{
		LOG_STUB();
	}
	
	void GetPlayerInfo( int ent_num, hud_player_info_t *pinfo )
	{
		LOG_STUB();
	}
	
	void PlaySoundByName( char *szSound, float volume )
	{
		LOG_STUB();
	}
	
	void PlaySoundByNameAtPitch( char *szSound, float volume, int pitch )
	{
		LOG_STUB();
	}
	
	void PlaySoundVoiceByName( char *szSound, float volume, int pitch )
	{
		LOG_STUB();
	}
	
	void PlaySoundByIndex( int iSound, float volume )
	{
		LOG_STUB();
	}
	
	void AngleVectors( const float *vecAngles, float *forward, float *right, float *up )
	{
		::AngleVectors( *(QAngle*)vecAngles, (Vector*)forward, (Vector*)right, (Vector*)up );
	}
	
	client_textmessage_t *TextMessageGet( const char *pName )
	{
		LOG_STUB();
		return NULL;
	}
	
	int DrawCharacter( int x, int y, int number, int r, int g, int b )
	{
		LOG_STUB();
		return 0;
	}
	
	int DrawConsoleString( int x, int y, char *string )
	{
		LOG_STUB();
		return 0;
	}
	
	void DrawSetTextColor( float r, float g, float b )
	{
		LOG_STUB();
	}
	
	void DrawConsoleStringLen( const char *string, int *length, int *height )
	{
		LOG_STUB();
	}
	
	void ConsolePrint( const char *string )
	{
		LOG_STUB();
	}
	
	void CenterPrint( const char *string )
	{
		LOG_STUB();
	}
	
	int GetWindowCenterX( void )
	{
		LOG_STUB();
		return 0;
	}
	
	int GetWindowCenterY( void )
	{
		LOG_STUB();
		return 0;
	}
	
	void GetViewAngles( float *rgflAngle )
	{
		LOG_STUB();
	}
	
	void SetViewAngles( float *rgflAngle )
	{
		LOG_STUB();
	}
	
	int GetMaxClients( void )
	{
		LOG_STUB();
		return 0;
	}
	
	void Cvar_SetValue( char *cvar, float value )
	{
		g_pGoldSRCCVars->SetFloat( cvar, value );
	}
	
	int Cmd_Argc( void )
	{
		LOG_STUB();
		return 0;
	}
	
	char *Cmd_Argv( int arg )
	{
		LOG_STUB();
		return NULL;
	}
	
	void Con_Printf( char *fmt, ... )
	{
		LOG_STUB();
	}
	
	void Con_DPrintf( char *fmt, ... )
	{
		LOG_STUB();
	}
	
	void Con_NPrintf( int pos, char *fmt, ... )
	{
		LOG_STUB();
	}
	
	void Con_NXPrintf( con_nprint_t *info, char *fmt, ... )
	{
		LOG_STUB();
	}
	
	const char *PhysInfo_ValueForKey( const char *key )
	{
		LOG_STUB();
		return "";
	}
	
	const char *ServerInfo_ValueForKey( const char *key )
	{
		LOG_STUB();
		return "";
	}
	
	float GetClientMaxspeed( void )
	{
		LOG_STUB();
		return 0;
	}
	
	int CheckParm( char *parm, char **ppnext )
	{
		LOG_STUB();
		return 0;
	}
	
	void Key_Event( int key, int down )
	{
		LOG_STUB();
	}
	
	void GetMousePosition( int *mx, int *my )
	{
		LOG_STUB();
	}
	
	int IsNoClipping( void )
	{
		LOG_STUB();
		return 0;
	}
	
	cl_entity_t *GetLocalPlayer( void )
	{
		C_BaseEntity *pPlayer = g_pBaseEntityList->GetLocalPlayer();
		return pPlayer ? pPlayer->ClEnt() : NULL;
	}
	
	cl_entity_t *GetViewModel( void )
	{
		static cl_entity_t viewmodel = {};
		//LOG_STUB();
		return &viewmodel;
	}
	
	cl_entity_t *GetEntityByIndex( int idx )
	{
		LOG_STUB();
		return NULL;
	}
	
	float GetClientTime( void )
	{
		LOG_STUB();
		return 0;
	}
	
	void V_CalcShake( void )
	{
		LOG_STUB();
	}
	
	void V_ApplyShake( float *origin, float *angles, float factor )
	{
		LOG_STUB();
	}
	
	int PM_PointContents( float *point, int *truecontents )
	{
		LOG_STUB();
		return 0;
	}
	
	int PM_WaterEntity( float *p )
	{
		LOG_STUB();
		return 0;
	}
	
	pmtrace_t *PM_TraceLine( float *start, float *end, int flags, int usehull, int ignore_pe )
	{
		LOG_STUB();
		return NULL;
	}
	
	model_t *CL_LoadModel( const char *modelname, int *index)
	{
		LOG_STUB();
		return NULL;
	}
	
	int CL_CreateVisibleEntity( int type, cl_entity_t *ent )
	{
		LOG_STUB();
		return 0;
	}
	
	const model_t *GetSpritePointer( HSPRITE hSprite )
	{
		LOG_STUB();
		return NULL;
	}
	
	void PlaySoundByNameAtLocation( char *szSound, float volume, float *origin )
	{
		LOG_STUB();
	}
	
	unsigned short PrecacheEvent( int type, const char *psz )
	{
		LOG_STUB();
		return 0;
	}
	
	void PlaybackEvent( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 )
	{
		LOG_STUB();
	}
	
	void WeaponAnim( int iAnim, int body )
	{
		LOG_STUB();
	}
	
	float RandomFloat( float flLow, float flHigh )
	{
		LOG_STUB();
		return 0;
	}
	
	int32 RandomLong( int32 lLow, int32 lHigh )
	{
		LOG_STUB();
		return 0;
	}
	
	void HookEvent( char *name, void ( *Event )( event_args_t *args ) )
	{
		LOG_STUB();
	}
	
	int Con_IsVisible()
	{
		LOG_STUB();
		return 0;
	}
	
	const char *GetGameDirectory( void )
	{
		static char s_szDirectory[MAX_PATH];
		static bool bInited = false;

		if ( !bInited )
		{
			char searchPaths[MAX_PATH];
			filesystem->GetSearchPath( "GAME", false, searchPaths, MAX_PATH );

			char *pPath = strtok( searchPaths, ";" );
			Q_strncpy( s_szDirectory, pPath, MAX_PATH );
			bInited = true;
		}

		return s_szDirectory;
	}
	
	cvar_t *GetCvarPointer( const char *szName )
	{
		return g_pGoldSRCCVars->GetPointer( szName );
	}
	
	const char *Key_LookupBinding( const char *pBinding )
	{
		LOG_STUB();
		return NULL;
	}
	
	const char *GetLevelName( void )
	{
		LOG_STUB();
		return "";
	}
	
	void GetScreenFade( screenfade_t *fade )
	{
		LOG_STUB();
	}
	
	void SetScreenFade( screenfade_t *fade )
	{
		LOG_STUB();
	}
	
	void *VGui_GetPanel()
	{
		return vguistub->GetRootPanel();
	}
	
	void VGui_ViewportPaintBackground( int extents[4] )
	{
		LOG_STUB();
	}
	
	byte *COM_LoadFile( char *path, int usehunk, int *pLength )
	{
		LOG_STUB();
		return NULL;
	}
	
	char *COM_ParseFile( char *data, char *token )
	{
		LOG_STUB();
		return NULL;
	}
	
	void COM_FreeFile( void *buffer )
	{
		LOG_STUB();
	}
	
	int IsSpectateOnly( void )
	{
		LOG_STUB();
		return 0;
	}
	
	model_t *LoadMapSprite( const char *filename )
	{
		LOG_STUB();
		return NULL;
	}
	
	void COM_AddAppDirectoryToSearchPath( const char *pszBaseDir, const char *appName )
	{
		LOG_STUB();
	}
	
	int COM_ExpandFilename( const char *fileName, char *nameOutBuffer, int nameOutBufferSize )
	{
		LOG_STUB();
		return 0;
	}
	
	const char *PlayerInfo_ValueForKey( int playerNum, const char *key )
	{
		LOG_STUB();
		return NULL;
	}
	
	void PlayerInfo_SetValueForKey( const char *key, const char *value )
	{
		LOG_STUB();
	}
	
	qboolean GetPlayerUniqueID( int iPlayer, char playerID[16] )
	{
		LOG_STUB();
		return false;
	}
	
	int GetTrackerIDForPlayer( int playerSlot )
	{
		LOG_STUB();
		return 0;
	}
	
	int GetPlayerForTrackerID( int trackerID )
	{
		LOG_STUB();
		return 0;
	}
	
	int ServerCmdUnreliable( char *szCmdString )
	{
		LOG_STUB();
		return 0;
	}
	
	void GetMousePos( tagPOINT *ppt )
	{
		LOG_STUB();
	}
	
	void SetMousePos( int x, int y )
	{
		LOG_STUB();
	}
	
	void SetMouseEnable( qboolean fEnable )
	{
		LOG_STUB();
	}
	
	cvar_t *GetFirstCVarPtr()
	{
		LOG_STUB();
		return NULL;
	}
	
	unsigned int GetFirstCmdFunctionHandle()
	{
		LOG_STUB();
		return 0;
	}
	
	unsigned int GetNextCmdFunctionHandle( unsigned int cmdhandle )
	{
		LOG_STUB();
		return 0;
	}
	
	const char *GetCmdFunctionName( unsigned int cmdhandle )
	{
		LOG_STUB();
		return NULL;
	}
	
	float GetClientOldTime()
	{
		LOG_STUB();
		return 0;
	}
	
	float GetServerGravityValue()
	{
		LOG_STUB();
		return 0;
	}
	
	model_t *GetModelByIndex( int index )
	{
		LOG_STUB();
		return NULL;
	}
	
	void SetFilterMode( int mode )
	{
		LOG_STUB();
	}
	
	void SetFilterColor( float r, float g, float b )
	{
		LOG_STUB();
	}
	
	void SetFilterBrightness( float brightness )
	{
		LOG_STUB();
	}
	
	sequenceEntry_t *SequenceGet( const char *fileName, const char *entryName )
	{
		LOG_STUB();
		return NULL;
	}
	
	void SPR_DrawGeneric( int frame, int x, int y, const wrect_t *prc, int src, int dest, int w, int h )
	{
		LOG_STUB();
	}
	
	sentenceEntry_t *SequencePickSentence( const char *sentenceName, int pickMethod, int *entryPicked ) 
	{
		LOG_STUB();
		return NULL;
	}
	
	int DrawString( int x, int y, const char* str, int r, int g, int b )
	{
		LOG_STUB();
		return 0;
	}
	
	int DrawStringReverse( int x, int y, const char* str, int r, int g, int b )
	{
		LOG_STUB();
		return 0;
	}
	
	const char *LocalPlayerInfo_ValueForKey( const char *key )
	{
		LOG_STUB();
		return NULL;
	}
	
	int VGUI2DrawCharacter( int x, int y, int ch, unsigned int font )
	{
		LOG_STUB();
		return 0;
	}
	
	int VGUI2DrawCharacterAdd( int x, int y, int ch, int r, int g, int b, unsigned int font )
	{
		LOG_STUB();
		return 0;
	}
	
	unsigned int COM_GetApproxWavePlayLength( const char *filename )
	{
		LOG_STUB();
		return 0;
	}
	
	void *GetCareerUI()
	{
		LOG_STUB();
		return NULL;
	}
	
	void Cvar_Set( char *cvar, char *value )
	{
		g_pGoldSRCCVars->SetString( cvar, value );
	}
	
	int IsPlayingCareerMatch()
	{
		LOG_STUB();
		return 0;
	}
	
	double GetAbsoluteTime( void )
	{
		LOG_STUB();
		return 0;
	}
	
	void ProcessTutorMessageDecayBuffer( int *buffer, int bufferLength )
	{
		LOG_STUB();
	}
	
	void ConstructTutorMessageDecayBuffer( int *buffer, int bufferLength )
	{
		LOG_STUB();
	}
	
	void ResetTutorMessageDecayData()
	{
		LOG_STUB();
	}
	
	void FillRGBABlend( int x, int y, int width, int height, int r, int g, int b, int a )
	{
		LOG_STUB();
	}
	
	int GetAppID( void )
	{
		LOG_STUB();
		return 0;
	}
	
	cmdalias_t *GetAliases( void )
	{
		LOG_STUB();
		return NULL;
	}
	
	void VguiWrap2_GetMouseDelta( int *x, int *y )
	{
		LOG_STUB();
	}
}


static GoldSRC::demo_api_t s_DemoAPI = {
	GoldSRC::IsRecording,
	GoldSRC::IsPlayingBack,
	GoldSRC::IsTimeDemo,
	GoldSRC::WriteBuffer,
};


static GoldSRC::cl_enginefuncs_t s_EngineFuncs = {
	GoldSRC::SPR_Load,
	GoldSRC::SPR_Frames,
	GoldSRC::SPR_Height,
	GoldSRC::SPR_Width,
	GoldSRC::SPR_Set,
	GoldSRC::SPR_Draw,
	GoldSRC::SPR_DrawHoles,
	GoldSRC::SPR_DrawAdditive,
	GoldSRC::SPR_EnableScissor,
	GoldSRC::SPR_DisableScissor,
	GoldSRC::SPR_GetList,
	GoldSRC::FillRGBA,
	GoldSRC::GetScreenInfo,
	GoldSRC::SetCrosshair,
	GoldSRC::RegisterVariable,
	GoldSRC::GetCvarFloat,
	GoldSRC::GetCvarString,
	GoldSRC::AddCommand,
	GoldSRC::HookUserMsg,
	GoldSRC::ServerCmd,
	GoldSRC::ClientCmd,
	GoldSRC::GetPlayerInfo,
	GoldSRC::PlaySoundByName,
	GoldSRC::PlaySoundByIndex,
	GoldSRC::AngleVectors,
	GoldSRC::TextMessageGet,
	GoldSRC::DrawCharacter,
	GoldSRC::DrawConsoleString,
	GoldSRC::DrawSetTextColor,
	GoldSRC::DrawConsoleStringLen,
	GoldSRC::ConsolePrint,
	GoldSRC::CenterPrint,
	GoldSRC::GetWindowCenterX,
	GoldSRC::GetWindowCenterY,
	GoldSRC::GetViewAngles,
	GoldSRC::SetViewAngles,
	GoldSRC::GetMaxClients,
	GoldSRC::Cvar_SetValue,
	GoldSRC::Cmd_Argc,
	GoldSRC::Cmd_Argv,
	GoldSRC::Con_Printf,
	GoldSRC::Con_DPrintf,
	GoldSRC::Con_NPrintf,
	GoldSRC::Con_NXPrintf,
	GoldSRC::PhysInfo_ValueForKey,
	GoldSRC::ServerInfo_ValueForKey,
	GoldSRC::GetClientMaxspeed,
	GoldSRC::CheckParm,
	GoldSRC::Key_Event,
	GoldSRC::GetMousePosition,
	GoldSRC::IsNoClipping,
	GoldSRC::GetLocalPlayer,
	GoldSRC::GetViewModel,
	GoldSRC::GetEntityByIndex,
	GoldSRC::GetClientTime,
	GoldSRC::V_CalcShake,
	GoldSRC::V_ApplyShake,
	GoldSRC::PM_PointContents,
	GoldSRC::PM_WaterEntity,
	GoldSRC::PM_TraceLine,
	GoldSRC::CL_LoadModel,
	GoldSRC::CL_CreateVisibleEntity,
	GoldSRC::GetSpritePointer,
	GoldSRC::PlaySoundByNameAtLocation,
	GoldSRC::PrecacheEvent,
	GoldSRC::PlaybackEvent,
	GoldSRC::WeaponAnim,
	GoldSRC::RandomFloat,
	GoldSRC::RandomLong,
	GoldSRC::HookEvent,
	GoldSRC::Con_IsVisible,
	GoldSRC::GetGameDirectory,
	GoldSRC::GetCvarPointer,
	GoldSRC::Key_LookupBinding,
	GoldSRC::GetLevelName,
	GoldSRC::GetScreenFade,
	GoldSRC::SetScreenFade,
	GoldSRC::VGui_GetPanel,
	GoldSRC::VGui_ViewportPaintBackground,
	GoldSRC::COM_LoadFile,
	GoldSRC::COM_ParseFile,
	GoldSRC::COM_FreeFile,
	NULL, //pTriAPI,
	NULL, //pEfxAPI,
	NULL, //pEventAPI,
	&s_DemoAPI,
	NULL, //pNetAPI,
	NULL, //pVoiceTweak,
	GoldSRC::IsSpectateOnly,
	GoldSRC::LoadMapSprite,
	GoldSRC::COM_AddAppDirectoryToSearchPath,
	GoldSRC::COM_ExpandFilename,
	GoldSRC::PlayerInfo_ValueForKey,
	GoldSRC::PlayerInfo_SetValueForKey,
	GoldSRC::GetPlayerUniqueID,
	GoldSRC::GetTrackerIDForPlayer,
	GoldSRC::GetPlayerForTrackerID,
	GoldSRC::ServerCmdUnreliable,
	GoldSRC::GetMousePos,
	GoldSRC::SetMousePos,
	GoldSRC::SetMouseEnable,
	GoldSRC::GetFirstCVarPtr,
	GoldSRC::GetFirstCmdFunctionHandle,
	GoldSRC::GetNextCmdFunctionHandle,
	GoldSRC::GetCmdFunctionName,
	GoldSRC::GetClientOldTime,
	GoldSRC::GetServerGravityValue,
	GoldSRC::GetModelByIndex,
	GoldSRC::SetFilterMode,
	GoldSRC::SetFilterColor,
	GoldSRC::SetFilterBrightness,
	GoldSRC::SequenceGet,
	GoldSRC::SPR_DrawGeneric,
	GoldSRC::SequencePickSentence,
	GoldSRC::DrawString,
	GoldSRC::DrawStringReverse,
	GoldSRC::LocalPlayerInfo_ValueForKey,
	GoldSRC::VGUI2DrawCharacter,
	GoldSRC::VGUI2DrawCharacterAdd,
	GoldSRC::COM_GetApproxWavePlayLength,
	GoldSRC::GetCareerUI,
	GoldSRC::Cvar_Set,
	GoldSRC::IsPlayingCareerMatch,
	GoldSRC::GetAbsoluteTime,
	GoldSRC::PlaySoundVoiceByName,
	GoldSRC::PrimeMusicStream,
	GoldSRC::ProcessTutorMessageDecayBuffer,
	GoldSRC::ConstructTutorMessageDecayBuffer,
	GoldSRC::ResetTutorMessageDecayData,
	GoldSRC::PlaySoundByNameAtPitch,
	GoldSRC::FillRGBABlend,
	GoldSRC::GetAppID,
	GoldSRC::GetAliases,
	GoldSRC::VguiWrap2_GetMouseDelta,
};

GoldSRC::cl_enginefuncs_t *EngineFuncs()
{
	return &s_EngineFuncs;
}

static GoldSRC::cldll_func_t s_ClientFuncs = {};
GoldSRC::cldll_func_t* ClientFuncs()
{
	return &s_ClientFuncs;
}
