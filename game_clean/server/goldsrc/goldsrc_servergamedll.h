#ifndef GOLDSRC_SERVERGAMEDLL_H
#define GOLDSRC_SERVERGAMEDLL_H

#ifdef _WIN32
#pragma once
#endif

#include "base_servergamedll.h"

class CGoldSRCServerGameDLL : public CBaseServerGameDLL
{
public:
	virtual bool		DLLInit( CreateInterfaceFn engineFactory, CreateInterfaceFn physicsFactory, CreateInterfaceFn fileSystemFactory, CGlobalVars *pGlobals );
	virtual void		DLLShutdown( void );

	virtual void		ServerActivate( edict_t *pEdictList, int edictCount, int clientMax );
	virtual void		GameShutdown( void );

	virtual bool		LevelInit( const char *pMapName, char const *pMapEntities, char const *pOldLevel, char const *pLandmarkName, bool loadGame, bool background );
	virtual void		LevelShutdown( void );

	virtual void		GameFrame( bool simulating );

	virtual const char	*GetGameDescription( void );
};

#endif // GOLDSRC_SERVERGAMEDLL_H
