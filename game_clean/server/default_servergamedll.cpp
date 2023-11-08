#include "cbase.h"
#include "base_servergamedll.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CBaseServerGameDLL g_ServerGameDLL;
CBaseServerGameDLL *g_pServerGameDLL = g_pServerGameDLL;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseServerGameDLL, IServerGameDLL, INTERFACEVERSION_SERVERGAMEDLL, g_ServerGameDLL );
