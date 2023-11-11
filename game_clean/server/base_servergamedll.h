#ifndef BASE_SERVERGAMEDLL_H
#define BASE_SERVERGAMEDLL_H

#ifdef _WIN32
#pragma once
#endif

#include "eiface.h"

extern IVEngineServer *engine = NULL;

class CBaseServerGameDLL : public IServerGameDLL
{
public:
	virtual bool	DLLInit( CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars *pGlobals );
	virtual bool	ReplayInit( CreateInterfaceFn fnReplayFactory );
	virtual bool	GameInit( void );
	virtual bool	LevelInit( char const *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background );
	virtual void	ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
	virtual void	GameFrame( bool simulating );
	virtual void	PreClientUpdate( bool simulating );
	virtual void	LevelShutdown( void );
	virtual void	GameShutdown( void );
	virtual void	DLLShutdown( void );
	virtual float	GetTickInterval( void ) const;
	virtual ServerClass	*GetAllServerClasses( void );
	virtual const char	*GetGameDescription( void );
	virtual void	CreateNetworkStringTables( void );
	virtual CSaveRestoreData	*SaveInit( int size );
	virtual void	SaveWriteFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount );
	virtual void	SaveReadFields( CSaveRestoreData *pSaveData, const char *szName, void *pBaseData, datamap_t *pMap, typedescription_t *pFields, int fieldCount );
	virtual void	SaveGlobalState( CSaveRestoreData *pSaveData );
	virtual void	RestoreGlobalState( CSaveRestoreData *pSaveData );
	virtual void	PreSave( CSaveRestoreData *pSaveData );
	virtual void	Save( CSaveRestoreData *pSaveData );
	virtual void	GetSaveComment( char *comment, int maxlength, float flMinutes, float flSeconds, bool bNoTime );
	virtual void	WriteSaveHeaders( CSaveRestoreData *pSaveData );
	virtual void	ReadRestoreHeaders( CSaveRestoreData *pSaveData );
	virtual void	Restore( CSaveRestoreData *pSaveData, bool b );
	virtual bool	IsRestoring();
	virtual int		CreateEntityTransitionList( CSaveRestoreData *pSaveData, int a );
	virtual void	BuildAdjacentMapList( void );
	virtual bool	GetUserMessageInfo( int msg_type, char *name, int maxnamelength, int &size );
	virtual CStandardSendProxies	*GetStandardSendProxies();
	virtual void	PostInit();
	virtual void	Think( bool finalTick );
	virtual void	PreSaveGameLoaded( char const *pSaveName, bool bCurrentlyInGame );
	virtual bool	ShouldHideServer( void );
	virtual void	InvalidateMdlCache();
	virtual void	OnQueryCvarValueFinished( QueryCvarCookie_t iCookie, edict_t *pPlayerEntity, EQueryCvarValueStatus eStatus, const char *pCvarName, const char *pCvarValue );
	virtual void	GameServerSteamAPIActivated( void );
	virtual void	GameServerSteamAPIShutdown( void );
	virtual void	SetServerHibernation( bool bHibernating );
	virtual IServerGCLobby	*GetServerGCLobby();
	virtual const char	*GetServerBrowserMapOverride();
	virtual const char	*GetServerBrowserGameData();
	virtual void	Status( void ( *print ) ( const char *fmt, ... ) );
	virtual void	PrepareLevelResources( char *pszMapName, size_t nMapNameSize, char *pszMapFile, size_t nMapFileSize );
	virtual ePrepareLevelResourcesResult AsyncPrepareLevelResources( char *pszMapName, size_t nMapNameSize, char *pszMapFile, size_t nMapFileSize, float *flProgress );
	virtual eCanProvideLevelResult	CanProvideLevel( char *pMapName, int nMapNameMax );
	virtual bool	IsManualMapChangeOkay( const char **pszReason );
};

extern CBaseServerGameDLL *g_pServerGameDLL;

#endif // BASE_SERVERGAMEDLL_H
