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
};

#endif // GOLDSRC_SERVERGAMECLIENTS_H
