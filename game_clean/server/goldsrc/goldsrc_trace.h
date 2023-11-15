#ifndef GOLDSRC_TRACE_H
#define GOLDSRC_TRACE_H

#ifdef _WIN32
#pragma once
#endif

#include "goldsrc_baseentity.h"
#include "goldsrc_eiface.h"


// AKA SV_Move()
GoldSRC::TraceResult UTIL_TraceEntityHull( const Vector &start, const Vector &end,
	const Vector &mins, const Vector &maxs, CBaseEntity *pIgnoreEnt = NULL );


#endif // GOLDSRC_TRACE_H