#include "cbase.h"
#include "base_gameclientexports.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

static CBaseGameClientExports g_GameClientExports;
CBaseGameClientExports* g_pGameClientExports = &g_GameClientExports;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseGameClientExports, IGameClientExports, GAMECLIENTEXPORTS_INTERFACE_VERSION, g_GameClientExports );
