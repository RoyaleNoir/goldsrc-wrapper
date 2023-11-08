#include "cbase.h"
#include "base_centerprint.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CBaseCenterPrint g_CenterPrint;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBaseCenterPrint, ICenterPrint, VCENTERPRINT_INTERFACE_VERSION, g_CenterPrint );
