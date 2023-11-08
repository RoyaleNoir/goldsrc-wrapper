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

// Server
#include <edict.h>
#include <eiface.h>
#include <iserverentity.h>

#define LOG_STUB() DevMsg( "[SERVER DLL STUB] %s\n", __FUNCTION__ );

#endif // CBASE_H