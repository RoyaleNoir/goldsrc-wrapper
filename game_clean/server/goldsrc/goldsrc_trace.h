#ifndef GOLDSRC_TRACE_H
#define GOLDSRC_TRACE_H

#ifdef _WIN32
#pragma once
#endif

#include "goldsrc_baseentity.h"
#include "goldsrc_eiface.h"


// AKA SV_Move()
GoldSRC::TraceResult UTIL_TraceEntityHull( const Vector &start, const Vector &end,
	const Vector &mins, const Vector &maxs, CBaseEntity *pIgnoreEnt = NULL,
	int traceType = 0 );

void UTIL_TraceHull( const Vector &start, const Vector &end, int traceType, 
	int hullNumber, CBaseEntity *pIgnoreEnt, GoldSRC::TraceResult *ptr );


void UTIL_TraceLine(const Vector &start, const Vector &end, int traceType, 
	CBaseEntity *pIgnoreEnt, GoldSRC::TraceResult *ptr );


bool UTIL_StandardTraceFilter( IHandleEntity *pHandleEntity, int fContentsMask );

#endif // GOLDSRC_TRACE_H