#ifndef BASE_SERVERGAMECLIENTS_H
#define BASE_SERVERGAMECLIENTS_H

#ifdef _WIN32
#pragma once
#endif

#include "eiface.h"

class CBaseServerGameClients : public IServerGameClients
{
public:
	virtual void	GetPlayerLimits( int &minplayers, int &maxplayers, int &defaultMaxPlayers ) const;
	virtual bool	ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
	virtual void	ClientActive( edict_t *pEntity, bool bLoadGame );
	virtual void	ClientDisconnect( edict_t *pEntity );
	virtual void	ClientPutInServer( edict_t *pEntity, char const *playername );
	virtual void	ClientCommand( edict_t *pEntity, const CCommand &args );
	virtual void	SetCommandClient( int index );
	virtual void	ClientSettingsChanged( edict_t *pEdict );
	virtual void	ClientSetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char *pvs, int pvssize );
	virtual float	ProcessUsercmds( edict_t *player, bf_read *buf, int numcmds, int totalcmds, int dropped_packets, bool ignore, bool paused );
	virtual void	PostClientMessagesSent_DEPRECIATED( void );
	virtual CPlayerState	*GetPlayerState( edict_t *player );
	virtual void	ClientEarPosition( edict_t *pEntity, Vector *pEarOrigin );
	virtual int		GetReplayDelay( edict_t *player, int &entity );
	virtual void	GetBugReportInfo( char *buf, int buflen );
	virtual void	NetworkIDValidated( const char *pszUserName, const char *pszNetworkID );
	virtual void	ClientCommandKeyValues( edict_t *pEntity, KeyValues *pKeyValues );
	virtual void	ClientSpawned( edict_t *pPlayer );
};

extern CBaseServerGameClients *g_pServerGameClients;

#endif // BASE_SERVERGAMECLIENTS_H
