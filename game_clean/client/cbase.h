#ifndef CBASE_H
#define CBASE_H

#ifdef _WIN32
#pragma once
#endif

// C Standard Library
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Tier 0
#include <tier0/platform.h>
#include <tier0/dbg.h>
#include <tier0/basetypes.h>

// Tier 1
#include <tier1/strtools.h>
#include <utlvector.h>

// Tier 2
#include <string_t.h>

// Engine (Shared)
#include <const.h>

#include <client_class.h>
#include <dt_recv.h>
#include <networkvar.h>

//#define LOG_STUB() DevMsg( "[CLIENT DLL STUB] %s\n", __FUNCTION__ );
#define LOG_STUB()

#endif // CBASE_H