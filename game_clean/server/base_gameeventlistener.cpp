#include "cbase.h"
#include "base_gameeventlistener.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IGameEventManager2 *gameeventmanager;


CBaseGameEventListener::CBaseGameEventListener()
{
	m_bListening = false;
}


CBaseGameEventListener::~CBaseGameEventListener()
{
	StopListeningForGameEvents();
}


void CBaseGameEventListener::ListenForGameEvents()
{
	// Needed to not crash
	ListenForGameEvent( "player_connect" );
	ListenForGameEvent( "player_activate" );
	ListenForGameEvent( "player_death" );
	ListenForGameEvent( "player_team" );
	ListenForGameEvent( "player_disconnect" );
	ListenForGameEvent( "player_connect" );
}

void CBaseGameEventListener::StopListeningForGameEvents()
{
	if ( m_bListening && gameeventmanager )
	{
		gameeventmanager->RemoveListener( this );
	}

	m_bListening = false;
}


void CBaseGameEventListener::ListenForGameEvent( const char *name )
{
	m_bListening = true;

	if ( gameeventmanager )
	{
		gameeventmanager->AddListener( this, name, true );
	}
}


void CBaseGameEventListener::FireGameEvent( IGameEvent *event )
{
}
