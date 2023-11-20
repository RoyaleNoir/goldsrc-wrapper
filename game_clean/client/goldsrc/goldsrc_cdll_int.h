#ifndef GOLDSRC_CDLL_INT_H
#define GOLDSRC_CDLL_INT_H

#ifdef _WIN32
#pragma once
#endif

namespace GoldSRC
{
	typedef unsigned char byte;
	typedef int int32;

	typedef int qboolean;
	typedef float vec3_t[3];

	typedef int HSPRITE;

	struct cl_entity_t;
	struct client_textmessage_t;
	struct client_sprite_t;
	struct clientdata_t;
	struct cmdalias_t;
	struct con_nprint_t;
	struct cvar_t;
	struct edict_t;
	struct entity_state_t;
	struct event_args_t;
	struct hud_player_info_t;
	struct local_state_t;
	struct model_t;
	struct mstudioevent_t;
	struct netadr_t;
	struct playermove_t;
	struct pmtrace_t;
	struct r_studio_interface_t;
	struct ref_params_t;
	struct screenfade_t;
	struct SCREENINFO;
	struct sentenceEntry_t;
	struct sequenceEntry_t;
	struct tagPOINT;
	struct tempent_t;
	struct usercmd_t;
	struct weapon_data_t;

	struct client_data_t
	{
		vec3_t	origin;
		vec3_t	viewangles;
		int		iWeaponBits;
		float	fov;
	};

	struct kbutton_t
	{
		int down[2];
		int state;
	};

	struct wrect_t
	{
		int left;
		int right;
		int top;
		int bottom;
	};

	
	struct efx_api_t;
	struct engine_studio_api_t;
	struct event_api_t;
	struct IVoiceTweak_t;
	struct net_api_t;
	struct triangleapi_t;

	struct demo_api_t
	{
		int		( *IsRecording )	( void );
		int		( *IsPlayingBack )	( void );
		int		( *IsTimeDemo )		( void );
		void	( *WriteBuffer )	( int size, unsigned char *buffer );
	};


	typedef int ( *pfnUserMsgHook )( const char *pszName, int iSize, void *pbuf );


	struct cl_enginefuncs_t
	{
		HSPRITE					( *pfnEngSrc_pfnSPR_Load_t )							( const char *szPicName );
		int						( *pfnEngSrc_pfnSPR_Frames_t )							( HSPRITE hPic );
		int						( *pfnEngSrc_pfnSPR_Height_t )							( HSPRITE hPic, int frame );
		int						( *pfnEngSrc_pfnSPR_Width_t )							( HSPRITE hPic, int frame );
		void					( *pfnEngSrc_pfnSPR_Set_t )								( HSPRITE hPic, int r, int g, int b);
		void					( *pfnEngSrc_pfnSPR_Draw_t )							( int frame, int x, int y, const wrect_t *prc );
		void					( *pfnEngSrc_pfnSPR_DrawHoles_t )						( int frame, int x, int y, const wrect_t *prc );
		void					( *pfnEngSrc_pfnSPR_DrawAdditive_t )					( int frame, int x, int y, const wrect_t *prc );
		void					( *pfnEngSrc_pfnSPR_EnableScissor_t )					( int x, int y, int width, int height );
		void					( *pfnEngSrc_pfnSPR_DisableScissor_t )					( void );
		client_sprite_t*		( *pfnEngSrc_pfnSPR_GetList_t )							( char *psz, int *piCount );
		void					( *pfnEngSrc_pfnFillRGBA_t )							( int x, int y, int width, int height, int r, int g, int b, int a );
		int						( *pfnEngSrc_pfnGetScreenInfo_t ) 						( SCREENINFO *pscrinfo );
		void					( *pfnEngSrc_pfnSetCrosshair_t )						( HSPRITE hspr, wrect_t rc, int r, int g, int b );
		cvar_t*					( *pfnEngSrc_pfnRegisterVariable_t )					( char *szName, char *szValue, int flags );
		float					( *pfnEngSrc_pfnGetCvarFloat_t )						( char *szName );
		char*					( *pfnEngSrc_pfnGetCvarString_t )						( char *szName );
		int						( *pfnEngSrc_pfnAddCommand_t )							( char *cmd_name, void ( *pfnEngSrc_function )( void ) );
		int						( *pfnEngSrc_pfnHookUserMsg_t )							( char *szMsgName, pfnUserMsgHook pfn);
		int						( *pfnEngSrc_pfnServerCmd_t )							( char *szCmdString );
		int						( *pfnEngSrc_pfnClientCmd_t )							( char *szCmdString );
		void					( *pfnEngSrc_pfnGetPlayerInfo_t )						( int ent_num, hud_player_info_t *pinfo );
		void					( *pfnEngSrc_pfnPlaySoundByName_t )						( char *szSound, float volume );
		void					( *pfnEngSrc_pfnPlaySoundByIndex_t )					( int iSound, float volume );
		void					( *pfnEngSrc_pfnAngleVectors_t )						( const float *vecAngles, float *forward, float *right, float *up );
		client_textmessage_t*	( *pfnEngSrc_pfnTextMessageGet_t )						( const char *pName );
		int						( *pfnEngSrc_pfnDrawCharacter_t )						( int x, int y, int number, int r, int g, int b );
		int						( *pfnEngSrc_pfnDrawConsoleString_t )					( int x, int y, char *string );
		void					( *pfnEngSrc_pfnDrawSetTextColor_t )					( float r, float g, float b );
		void					( *pfnEngSrc_pfnDrawConsoleStringLen_t )				( const char *string, int *length, int *height );
		void					( *pfnEngSrc_pfnConsolePrint_t )						( const char *string );
		void					( *pfnEngSrc_pfnCenterPrint_t )							( const char *string );
		int						( *pfnEngSrc_GetWindowCenterX_t )						( void );
		int						( *pfnEngSrc_GetWindowCenterY_t )						( void );
		void					( *pfnEngSrc_GetViewAngles_t )							( float* );
		void					( *pfnEngSrc_SetViewAngles_t )							( float* );
		int						( *pfnEngSrc_GetMaxClients_t )							( void );
		void					( *pfnEngSrc_Cvar_SetValue_t )							( char *cvar, float value );
		int       				( *pfnEngSrc_Cmd_Argc_t )								( void );
		char*					( *pfnEngSrc_Cmd_Argv_t )								( int arg );
		void					( *pfnEngSrc_Con_Printf_t )								( char *fmt, ... );
		void					( *pfnEngSrc_Con_DPrintf_t )							( char *fmt, ... );
		void					( *pfnEngSrc_Con_NPrintf_t )							( int pos, char *fmt, ... );
		void					( *pfnEngSrc_Con_NXPrintf_t )							( con_nprint_t *info, char *fmt, ... );
		const char*				( *pfnEngSrc_PhysInfo_ValueForKey_t )					( const char *key );
		const char*				( *pfnEngSrc_ServerInfo_ValueForKey_t )					( const char *key );
		float					( *pfnEngSrc_GetClientMaxspeed_t )						( void );
		int						( *pfnEngSrc_CheckParm_t )								( char *parm, char **ppnext );
		void					( *pfnEngSrc_Key_Event_t )								( int key, int down );
		void					( *pfnEngSrc_GetMousePosition_t )						( int *mx, int *my );
		int						( *pfnEngSrc_IsNoClipping_t )							( void );
		cl_entity_t*			( *pfnEngSrc_GetLocalPlayer_t )							( void );
		cl_entity_t*			( *pfnEngSrc_GetViewModel_t )							( void );
		cl_entity_t*			( *pfnEngSrc_GetEntityByIndex_t )						( int idx );
		float					( *pfnEngSrc_GetClientTime_t )							( void );
		void					( *pfnEngSrc_V_CalcShake_t )							( void );
		void					( *pfnEngSrc_V_ApplyShake_t )							( float *origin, float *angles, float factor );
		int						( *pfnEngSrc_PM_PointContents_t )						( float *point, int *truecontents );
		int						( *pfnEngSrc_PM_WaterEntity_t )							( float *p );
		pmtrace_t*				( *pfnEngSrc_PM_TraceLine_t )							( float *start, float *end, int flags, int usehull, int ignore_pe );
		model_t*				( *pfnEngSrc_CL_LoadModel_t )							( const char *modelname, int *index);
		int						( *pfnEngSrc_CL_CreateVisibleEntity_t )					( int type, cl_entity_t *ent );
		const model_t*			( *pfnEngSrc_GetSpritePointer_t )						( HSPRITE hSprite );
		void					( *pfnEngSrc_pfnPlaySoundByNameAtLocation_t )			( char *szSound, float volume, float *origin );
		unsigned short			( *pfnEngSrc_pfnPrecacheEvent_t )						( int type, const char *psz );
		void					( *pfnEngSrc_pfnPlaybackEvent_t )						( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
		void					( *pfnEngSrc_pfnWeaponAnim_t )							( int iAnim, int body );
		float					( *pfnEngSrc_pfnRandomFloat_t )							( float flLow, float flHigh );
		int32					( *pfnEngSrc_pfnRandomLong_t )							( int32 lLow, int32 lHigh );
		void					( *pfnEngSrc_pfnHookEvent_t )							( char *name, void ( *pfnEvent )( event_args_t *args ) );
		int						( *pfnEngSrc_Con_IsVisible_t )							();
		const char*				( *pfnEngSrc_pfnGetGameDirectory_t )					( void );
		cvar_t*					( *pfnEngSrc_pfnGetCvarPointer_t )						( const char *szName );
		const char*				( *pfnEngSrc_Key_LookupBinding_t )						( const char *pBinding );
		const char*				( *pfnEngSrc_pfnGetLevelName_t )						( void );
		void					( *pfnEngSrc_pfnGetScreenFade_t )						( screenfade_t *fade );
		void					( *pfnEngSrc_pfnSetScreenFade_t )						( screenfade_t *fade );
		void*					( *pfnEngSrc_VGui_GetPanel_t )							();
		void					( *pfnEngSrc_VGui_ViewportPaintBackground_t )			( int extents[4] );
		byte*					( *pfnEngSrc_COM_LoadFile_t )							( char *path, int usehunk, int *pLength );
		char*					( *pfnEngSrc_COM_ParseFile_t )							( char *data, char *token );
		void					( *pfnEngSrc_COM_FreeFile_t )							( void *buffer );
		triangleapi_t*			pTriAPI;
		efx_api_t*				pEfxAPI;
		event_api_t*			pEventAPI;
		demo_api_t*				pDemoAPI;
		net_api_t*				pNetAPI;
		IVoiceTweak_t*			pVoiceTweak;
		int						( *pfnEngSrc_IsSpectateOnly_t )							( void );
		model_t*				( *pfnEngSrc_LoadMapSprite_t )							( const char *filename );
		void					( *pfnEngSrc_COM_AddAppDirectoryToSearchPath_t )		( const char *pszBaseDir, const char *appName );
		int						( *pfnEngSrc_COM_ExpandFilename_t )						( const char *fileName, char *nameOutBuffer, int nameOutBufferSize );
		const char*				( *pfnEngSrc_PlayerInfo_ValueForKey_t )					( int playerNum, const char *key );
		void					( *pfnEngSrc_PlayerInfo_SetValueForKey_t )				( const char *key, const char *value );
		qboolean				( *pfnEngSrc_GetPlayerUniqueID_t )						( int iPlayer, char playerID[16] );
		int						( *pfnEngSrc_GetTrackerIDForPlayer_t )					( int playerSlot );
		int						( *pfnEngSrc_GetPlayerForTrackerID_t )					( int trackerID );
		int						( *pfnEngSrc_pfnServerCmdUnreliable_t )					( char *szCmdString );
		void					( *pfnEngSrc_GetMousePos_t )							( tagPOINT *ppt );
		void					( *pfnEngSrc_SetMousePos_t )							( int x, int y );
		void					( *pfnEngSrc_SetMouseEnable_t )							( qboolean fEnable );
		cvar_t*					( *pfnEngSrc_GetFirstCVarPtr_t )						();
		unsigned int			( *pfnEngSrc_GetFirstCmdFunctionHandle_t )				();
		unsigned int			( *pfnEngSrc_GetNextCmdFunctionHandle_t )				( unsigned int cmdhandle );
		const char*				( *pfnEngSrc_GetCmdFunctionName_t )						( unsigned int cmdhandle );
		float					( *pfnEngSrc_GetClientOldTime_t )						();
		float					( *pfnEngSrc_GetServerGravityValue_t )					();
		model_t*				( *pfnEngSrc_GetModelByIndex_t )						( int index );
		void					( *pfnEngSrc_pfnSetFilterMode_t )						( int mode );
		void					( *pfnEngSrc_pfnSetFilterColor_t )						( float r, float g, float b );
		void					( *pfnEngSrc_pfnSetFilterBrightness_t )					( float brightness );
		sequenceEntry_t*		( *pfnEngSrc_pfnSequenceGet_t )							( const char *fileName, const char *entryName );
		void					( *pfnEngSrc_pfnSPR_DrawGeneric_t )						( int frame, int x, int y, const wrect_t *prc, int src, int dest, int w, int h );
		sentenceEntry_t*		( *pfnEngSrc_pfnSequencePickSentence_t )				( const char *sentenceName, int pickMethod, int *entryPicked );
		int						( *pfnEngSrc_pfnDrawString_t )							( int x, int y, const char* str, int r, int g, int b );
		int						( *pfnEngSrc_pfnDrawStringReverse_t )					( int x, int y, const char* str, int r, int g, int b );
		const char*				( *pfnEngSrc_LocalPlayerInfo_ValueForKey_t )			( const char *key );
		int						( *pfnEngSrc_pfnVGUI2DrawCharacter_t )					( int x, int y, int ch, unsigned int font );
		int						( *pfnEngSrc_pfnVGUI2DrawCharacterAdd_t )				( int x, int y, int ch, int r, int g, int b, unsigned int font );
		unsigned int			( *pfnEngSrc_COM_GetApproxWavePlayLength )				( const char *filename );
		void*					( *pfnEngSrc_pfnGetCareerUI_t )							();
		void					( *pfnEngSrc_Cvar_Set_t )								( char *cvar, char *value );
		int						( *pfnEngSrc_pfnIsPlayingCareerMatch_t )				();
		double					( *pfnEngSrc_GetAbsoluteTime_t )						( void );
		void					( *pfnEngSrc_pfnPlaySoundVoiceByName_t )				( char *szSound, float volume, int pitch );
		void					( *pfnEngSrc_pfnPrimeMusicStream_t )					( char *szFilename, int looping );
		void					( *pfnEngSrc_pfnProcessTutorMessageDecayBuffer_t )		( int *buffer, int bufferLength );
		void					( *pfnEngSrc_pfnConstructTutorMessageDecayBuffer_t )	( int *buffer, int bufferLength );
		void					( *pfnEngSrc_pfnResetTutorMessageDecayData_t )			();
		void					( *pfnEngSrc_pfnPlaySoundByNameAtPitch_t )				( char *szSound, float volume, int pitch );
		void					( *pfnEngSrc_pfnFillRGBABlend_t )						( int x, int y, int width, int height, int r, int g, int b, int a );
		int						( *pfnEngSrc_pfnGetAppID_t )							( void );
		cmdalias_t*				( *pfnEngSrc_pfnGetAliases_t )							( void );
		void					( *pfnEngSrc_pfnVguiWrap2_GetMouseDelta_t )				( int *x, int *y );
	};

	struct cldll_func_t
	{
		int				( *Initialize )						( cl_enginefuncs_t*, int );
		void			( *HUD_Init )						( void );
		int				( *HUD_VidInit )					( void );
		int				( *HUD_Redraw)						( float, int );
		int				( *HUD_UpdateClientData )			( client_data_t*, float );
		void			( *HUD_Reset )						( void );
		void			( *HUD_PlayerMove )					( playermove_t *ppmove, qboolean server );
		void			( *HUD_PlayerMoveInit )				( playermove_t *ppmove);
		char			( *HUD_PlayerMoveTexture )			( char* name );
		void			( *IN_ActivateMouse )				( void );
		void			( *IN_DeactivateMouse )				( void );
		void			( *IN_MouseEvent )					( int mstate );
		void			( *IN_ClearStates )					( void );
		void			( *IN_Accumulate )					( void );
		void			( *CL_CreateMove )					( float frametime, usercmd_t *cmd, int active );
		int				( *CL_IsThirdPerson )				( void );
		void			( *CL_CameraOffset )				( float *ofs );
		kbutton_t		( *KB_Find )						( const char *name );
		void			( *CAM_Think )						( void );
		void			( *V_CalcRefdef )					( ref_params_t *pparams );
		int				( *HUD_AddEntity )					( int type, cl_entity_t *ent, const char *modelname );
		void			( *HUD_CreateEntities )				( void );
		void			( *HUD_DrawNormalTriangles )		( void );
		void			( *HUD_DrawTransparentTriangles )	( void );
		void			( *HUD_StudioEvent )				( const mstudioevent_t *event, const cl_entity_t *entity );
		void			( *HUD_PostRunCmd )					( local_state_t *from, local_state_t *to, usercmd_t *cmd, int runfuncs, double realtime, unsigned int random_seed );
		void			( *HUD_Shutdown )					( void );
		void			( *HUD_TxferLocalOverrides )		( entity_state_t *state, const clientdata_t *client );
		void			( *HUD_ProcessPlayerState )			( entity_state_t *dst, const entity_state_t *src );
		void			( *HUD_txferPredictionData )		( entity_state_t *ps, const entity_state_t *pps, clientdata_t *pcd, const clientdata_t *ppcd, weapon_data_t *wd, const weapon_data_t *pwd );
		void			( *Demo_ReadBuffer )				( int size, unsigned char *buffer );
		int				( *HUD_ConnectionlessPacket )		( const netadr_t *net_from, const char *args, char *response_buffer, int *response_buffer_size );
		int				( *HUD_GetHullBounds )				( int hullnumber, float *mins, float *maxs );
		void			( *HUD_Frame )						( double );
		int				( *HUD_Key_Event )					( int eventcode, int keynum, const char *pszCurrentBinding );
		void			( *HUD_TempEntUpdate )				( double frametime, double client_time, double cl_gravity, tempent_t **ppTempEntFree, tempent_t **ppTempEntActive, int ( *Callback_AddVisibleEntity )( cl_entity_t *pEntity ), void ( *Callback_TempEntPlaySound )( tempent_t *pTemp, float damp ) );
		cl_entity_t*	( *HUD_GetUserEntity )				( int index );
		void			( *HUD_VoiceStatus )				( int entindex, qboolean bTalking );
		void			( *HUD_DirectorMessage )			( int iSize, void *pbuf );
		int				( *HUD_GetStudioModelInterface )	( int version, r_studio_interface_t **ppinterface, engine_studio_api_t *pstudio );
		void			( *HUD_ChatInputPosition )			( int *x, int *y );
		int				( *HUD_GetPlayerTeam )				( int iplayer );
		void*			( *ClientFactory )					();
	};

	#define GOLDSRC_CLDLL_INTERFACE_VERSION 7
	typedef void ( *pfnClientDllGiveFuncs_t )( void *pv );
}

extern GoldSRC::cl_enginefuncs_t *EngineFuncs();
extern GoldSRC::cldll_func_t *ClientFuncs();

#endif // GOLDSRC_CDLL_INT_H
