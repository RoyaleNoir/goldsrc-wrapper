#include "cbase.h"
#include "base_clientdll.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



CBaseClientDLL g_BaseClientDLL;
IBaseClientDLL *g_pClientDLL = &g_BaseClientDLL;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseClientDLL, IBaseClientDLL, CLIENT_DLL_INTERFACE_VERSION, g_BaseClientDLL );

