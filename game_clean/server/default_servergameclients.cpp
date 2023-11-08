#include "cbase.h"
#include "base_servergameclients.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CBaseServerGameClients g_ServerGameClients;
extern CBaseServerGameClients *g_pServerGameClients = &g_ServerGameClients;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseServerGameClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS, g_ServerGameClients );
