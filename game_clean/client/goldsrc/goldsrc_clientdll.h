#ifndef GOLDSRC_CLIENTDLL_H
#define GOLDSRC_CLIENTDLL_H

#ifdef _WIN32
#pragma once
#endif

#include "base_clientdll.h"

class CGoldSRCClientDLL : public CBaseClientDLL
{
public:
	virtual int		Init( CreateInterfaceFn appSystemFactory, CreateInterfaceFn physicsFactory, CGlobalVarsBase *pGlobals );

	virtual void	LevelShutdown( void );

	virtual void	IN_ActivateMouse( void );
	virtual void	IN_DeactivateMouse( void );
	virtual void	IN_Accumulate( void );
	virtual void	IN_ClearStates( void );

	virtual void	CreateMove( int sequence_number, float input_sample_frametime, bool active );
	virtual void	ExtraMouseSample( float frametime, bool active );
	virtual bool	WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand );

	virtual void	View_Render( vrect_t *rect );

	virtual void	FrameStageNotify( ClientFrameStage_t curStage );
};

#endif // GOLDSRC_CLIENTDLL_H