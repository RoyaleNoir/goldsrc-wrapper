#ifndef BASE_GAMEEVENTLISTENER_H
#define BASE_GAMEEVENTLISTENER_H

#ifdef _WIN32
#pragma once
#endif

#include "igameevents.h"

class CBaseGameEventListener : public IGameEventListener2
{
public:
	CBaseGameEventListener();
	virtual			~CBaseGameEventListener();

	virtual void	ListenForGameEvents();
	void			StopListeningForGameEvents();
	
	void			ListenForGameEvent( const char *name );
	virtual void	FireGameEvent( IGameEvent *event );

private:
	bool			m_bListening;
};

extern CBaseGameEventListener *g_pGameEventListener;

#endif // BASE_GAMEEVENTLISTENER_H
