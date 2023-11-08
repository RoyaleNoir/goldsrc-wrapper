#ifndef BASE_CLIENTDLL_H
#define BASE_CLIENTDLL_H

#ifdef _WIN32
#pragma once
#endif

#include "cdll_int.h"

//-----------------------------------------------------------------------------
// Purpose: Interface from client.dll to engine.dll
//-----------------------------------------------------------------------------
class CBaseClientDLL : public IBaseClientDLL
{
public:
	virtual int		Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals );
	virtual void	PostInit();
	virtual void	Shutdown( void );

	virtual bool	ReplayInit( CreateInterfaceFn fnReplayFactory );
	virtual bool	ReplayPostInit();

	virtual void	LevelInitPreEntity( char const *pMapName );
	virtual void	LevelInitPostEntity();
	virtual void	LevelShutdown( void );

	virtual ClientClass	*GetAllClasses( void );

	virtual int		HudVidInit( void );
	virtual void	HudProcessInput( bool bActive );
	virtual void	HudUpdate( bool bActive );
	virtual void	HudReset( void );
	virtual void	HudText( const char *message );

	virtual void	IN_ActivateMouse( void );
	virtual void	IN_DeactivateMouse( void );
	virtual void	IN_Accumulate( void );
	virtual void	IN_ClearStates( void );
	virtual bool	IN_IsKeyDown( const char *name, bool &isdown );
	virtual void	IN_OnMouseWheeled( int nDelta );
	virtual int		IN_KeyEvent( int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding );

	virtual void	CreateMove( int sequence_number, float input_sample_frametime, bool active );

	virtual void	ExtraMouseSample( float frametime, bool active );

	virtual bool	WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand );
	virtual void	EncodeUserCmdToBuffer( bf_write &buf, int slot );
	virtual void	DecodeUserCmdFromBuffer( bf_read &buf, int slot );

	virtual void	View_Render( vrect_t *rect );
	virtual void	RenderView( const CViewSetup &view, int nClearFlags, int whatToDraw );
	virtual void	View_Fade( ScreenFade_t *pSF );

	virtual void	SetCrosshairAngle( const QAngle &angle );

	virtual void	InitSprite( CEngineSprite *pSprite, const char *loadname );
	virtual void	ShutdownSprite( CEngineSprite *pSprite );
	virtual int		GetSpriteSize( void ) const;

	virtual void	VoiceStatus( int entindex, qboolean bTalking );

	virtual void	InstallStringTableCallback( char const *tableName );

	virtual void	FrameStageNotify( ClientFrameStage_t curStage );

	virtual bool	DispatchUserMessage( int msg_tye, bf_read &msg_data );

	virtual CSaveRestoreData	*SaveInit( int size );
	virtual void	SaveWriteFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount );
	virtual void	SaveReadFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount );
	virtual void	PreSave( CSaveRestoreData *pSaveData );
	virtual void	Save( CSaveRestoreData *pSaveData );
	virtual void	WriteSaveHeaders( CSaveRestoreData *pSaveData );
	virtual void	ReadRestoreHeaders( CSaveRestoreData *pSaveData );
	virtual void	Restore( CSaveRestoreData *pSaveData, bool b );
	virtual void	DispatchOnRestore();

	virtual CStandardRecvProxies	*GetStandardRecvProxies();

	virtual void	WriteSaveGameScreenshot( const char *pFilename );

	virtual void	EmitSentenceCloseCaption( char const *tokenstream );
	virtual void	EmitCloseCaption( char const *captionname, float duration );

	virtual bool	CanRecordDemo( char *errorMsg, int length ) const;

	virtual void	OnDemoRecordStart( char const *pDemoBaseName );
	virtual void	OnDemoRecordStop();
	virtual void	OnDemoPlaybackStart( char const *pDemoBaseName );
	virtual void	OnDemoPlaybackStop();

	virtual bool	ShouldDrawDropdownConsole();

	virtual int		GetScreenWidth();
	virtual int		GetScreenHeight();

	virtual void	WriteSaveGameScreenshotOfSize( const char *pFilename, int width, int height, bool bCreatePowerOf2Padded, bool bWriteVTF );

	virtual bool	GetPlayerView( CViewSetup &playerView );

	virtual void	SetupGameProperties( CUtlVector< XUSER_CONTEXT > &contexts, CUtlVector< XUSER_PROPERTY > &properties );
	virtual uint	GetPresenceID( const char *pIDName );
	virtual const char	*GetPropertyIdString( const uint id );
	virtual void	GetPropertyDisplayString( uint id, uint value, char *pOutput, int nBytes );

	virtual void	StartStatsReporting( HANDLE handle, bool bArbitrated );

	virtual void	InvalidateMdlCache();

	virtual void	IN_SetSampleTime( float frametime );

	virtual void	ReloadFilesInList( IFileList *pFilesToReload );

	virtual bool	HandleUiToggle();

	virtual bool	ShouldAllowConsole();

	virtual CRenamedRecvTableInfo	*GetRenamedRecvTableInfos();

	virtual CMouthInfo* GetClientUIMouthInfo();

	virtual void	FileReceived( const char *fileName, unsigned int transferID );

	virtual const char	*TranslateEffectForVisionFilter( const char *pchEffectType, const char *pchEffectName );

	virtual void	ClientAdjustStartSoundParams( struct StartSoundParams_t &params );

	virtual bool	DisconnectAttempt( void );

	virtual bool	IsConnectedUserInfoChangeAllowed( IConVar *pCvar );
};

#endif // BASE_CLIENTDLL_H