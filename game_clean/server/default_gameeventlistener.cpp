#include "cbase.h"
#include "base_gameeventlistener.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CBaseGameEventListener g_GameEventListener;
CBaseGameEventListener *g_pGameEventListener = &g_GameEventListener;
