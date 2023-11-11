#ifndef GOLDSRC_CLIENTDLL_H
#define GOLDSRC_CLIENTDLL_H

#ifdef _WIN32
#pragma once
#endif

#include "base_clientdll.h"

class CGoldSRCClientDLL : public CBaseClientDLL
{
public:
	virtual void	LevelShutdown( void );
	virtual void	View_Render( vrect_t *rect );
};

#endif // GOLDSRC_CLIENTDLL_H