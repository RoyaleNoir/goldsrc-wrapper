#include "cbase.h"
#include "base_servergameents.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


CBaseServerGameEnts g_ServerGameEnts;
CBaseServerGameEnts *g_pServerGameEnts = &g_ServerGameEnts;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseServerGameEnts, IServerGameEnts, INTERFACEVERSION_SERVERGAMEENTS, g_ServerGameEnts );
