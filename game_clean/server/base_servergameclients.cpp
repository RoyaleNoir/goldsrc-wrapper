#include "cbase.h"
#include "base_servergameclients.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


void CBaseServerGameClients::GetPlayerLimits( int &minplayers, int &maxplayers, int &defaultMaxPlayers ) const
{
	LOG_STUB();
}


bool CBaseServerGameClients::ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	LOG_STUB();

	return true;
}


void CBaseServerGameClients::ClientActive( edict_t *pEntity, bool bLoadGame )
{
	LOG_STUB();
}


void CBaseServerGameClients::ClientDisconnect( edict_t *pEntity )
{
	LOG_STUB();
}


void CBaseServerGameClients::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	LOG_STUB();
}


void CBaseServerGameClients::ClientCommand( edict_t *pEntity, const CCommand &args )
{
	//LOG_STUB();
}


void CBaseServerGameClients::SetCommandClient( int index )
{
	//LOG_STUB();
}


void CBaseServerGameClients::ClientSettingsChanged( edict_t *pEdict )
{
	LOG_STUB();
}


void CBaseServerGameClients::ClientSetupVisibility( edict_t *pViewEntity, edict_t *pClient, unsigned char *pvs, int pvssize )
{
	//LOG_STUB();
}


float CBaseServerGameClients::ProcessUsercmds( edict_t *player, bf_read *buf, int numcmds, int totalcmds, int dropped_packets, bool ignore, bool paused )
{
	LOG_STUB();

	return 0.0f;
}


void CBaseServerGameClients::PostClientMessagesSent_DEPRECIATED( void )
{
	LOG_STUB();
}


CPlayerState *CBaseServerGameClients::GetPlayerState( edict_t *player )
{
	//LOG_STUB();

	return nullptr;
}


void CBaseServerGameClients::ClientEarPosition( edict_t *pEntity, Vector *pEarOrigin )
{
	LOG_STUB();
}


int CBaseServerGameClients::GetReplayDelay( edict_t *player, int &entity )
{
	LOG_STUB();

	return 0;
}


void CBaseServerGameClients::GetBugReportInfo( char *buf, int buflen )
{
	LOG_STUB();
}


void CBaseServerGameClients::NetworkIDValidated( const char *pszUserName, const char *pszNetworkID )
{
	LOG_STUB();
}


void CBaseServerGameClients::ClientCommandKeyValues( edict_t *pEntity, KeyValues *pKeyValues )
{
	LOG_STUB();
}


void CBaseServerGameClients::ClientSpawned( edict_t *pPlayer )
{
	LOG_STUB();
}
