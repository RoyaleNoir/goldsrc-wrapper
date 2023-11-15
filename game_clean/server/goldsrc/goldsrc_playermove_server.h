#ifndef GOLDSRC_PLAYERMOVE_SERVER_H
#define GOLDSRC_PLAYERMOVE_SERVER_H

#ifdef _WIN32
#pragma once
#endif

#include "goldsrc_playermove_shared.h"

#define MAX_CMDLIST_SIZE 16

struct CUserCmdList
{
	GoldSRC::usercmd_t m_Cmds[MAX_CMDLIST_SIZE];
	int m_nNumCmds;
};


class CGoldSRCPlayerMove
{
public:
	CGoldSRCPlayerMove();
	~CGoldSRCPlayerMove();

	void Init();

	void ProcessUsercmds( CBaseEntity *pEntity, CUserCmdList *cmdlists, int numcmds, int totalcmds, int dropped_packets, bool paused );

private:
	void RunUserCmd( CBaseEntity *pEntity, GoldSRC::usercmd_t *pCmd );

	void SetupPlayerMove( CBaseEntity *pEntity, GoldSRC::usercmd_t *pCmd );
	void ApplyPlayerMove( CBaseEntity *pEntity );

	// playermove_t callbacks
private:
	static const char *PM_Info_ValueForKey( const char *s, const char *key );
	static void PM_Particle( float *origin, int color, float life, int zpos, int zvel );
	static int	PM_TestPlayerPosition( float *pos, GoldSRC::pmtrace_t *ptrace );
	static void Con_NPrintf( int idx, char *fmt, ... );
	static void Con_DPrintf( char *fmt, ... );
	static void Con_Printf( char *fmt, ... );
	static double PM_Sys_FloatTime( void );
	static void PM_StuckTouch( int hitent, GoldSRC::pmtrace_t *ptraceresult );
	static int PM_PointContents( float *p, int *truecontents );
	static int PM_TruePointContents( float *p );
	static int PM_HullPointContents( GoldSRC::hull_t *hull, int num, float *p );
	static GoldSRC::pmtrace_t PM_PlayerTrace( float *start, float *end, int traceFlags, int ignore_pe );
	static GoldSRC::pmtrace_t *PM_TraceLine( float *start, float *end, int flags, int usehulll, int ignore_pe );
	static GoldSRC::int32 RandomLong( GoldSRC::int32 lLow, GoldSRC::int32 lHigh );
	static float RandomFloat( float lLow, float lHigh );
	static int PM_GetModelType( GoldSRC::model_t *mod );
	static void PM_GetModelBounds( GoldSRC::model_t *mod, float *mins, float *maxs );
	static void *PM_HullForBsp( GoldSRC::physent_t *pe, float *offset );
	static float PM_TraceModel( GoldSRC::physent_t *pe, float *offset );
	static int COM_FileSize( char *filename );
	static GoldSRC::byte *COM_LoadFile( char *path, int usehunk, int *pLength );
	static void COM_FreeFile( void *buffer );
	static char *memfgets( GoldSRC::byte *pMemFile, int fileSize, int *pFilePos, char *pBuffer, int bufferSize );
	static void PM_PlaySound( int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch );
	static const char *PM_TraceTexture( int ground, float *vstart, float *vend );
	static void PM_PlaybackEventFull( int flags, int clientindex, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 );
	static GoldSRC::pmtrace_t PM_PlayerTraceEx( float *start, float *end, int traceFlags, int ( *pfnIgnore )( GoldSRC::physent_t *pe ) );
	static int PM_TestPlayerPositionEx( float *pos, GoldSRC::pmtrace_t *ptrace, int ( *pfnIgnore )( GoldSRC::physent_t *pe ) );
	static GoldSRC::pmtrace_t *PM_TraceLineEx( float *start, float *end, int flags, int usehulll, int ( *pfnIgnore )( GoldSRC::physent_t *pe ) );

private:
	GoldSRC::playermove_t m_playerMove;
	GoldSRC::usercmd_t m_cmd;
};

extern CGoldSRCPlayerMove *g_pGoldSRCPlayerMove;

#endif // GOLDSRC_PLAYERMOVE_SERVER_H