#ifndef GOLDSRC_SERVERGAMECLIENTS_H
#define GOLDSRC_SERVERGAMECLIENTS_H

#ifdef _WIN32
#pragma once
#endif

#include "base_servergameclients.h"

class CGoldSRCServerGameClients : public CBaseServerGameClients
{
public:
	virtual bool	ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen );
	virtual void	ClientPutInServer( edict_t *pEntity, char const *playername );

	virtual void	SetCommandClient( int index );

	virtual float	ProcessUsercmds( edict_t *player, bf_read *buf, int numcmds, int totalcmds, int dropped_packets, bool ignore, bool paused );

	virtual			CPlayerState *GetPlayerState( edict_t *player );
	virtual void	ClientEarPosition( edict_t *pEntity, Vector *pEarOrigin );

	CBaseEntity		*GetCommandClient();
};

extern CGoldSRCServerGameClients *g_pGoldSRCGameClients;

#endif // GOLDSRC_SERVERGAMECLIENTS_H
