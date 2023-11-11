#include "cbase.h"
#include "goldsrc_servergameclients.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_edict.h"
#include "goldsrc_eiface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CGoldSRCServerGameClients g_ServerGameClients;
extern CBaseServerGameClients *g_pServerGameClients = &g_ServerGameClients;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CGoldSRCServerGameClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS, g_ServerGameClients );


bool CGoldSRCServerGameClients::ClientConnect( edict_t *pEntity, const char *pszName, const char *pszAddress, char *reject, int maxrejectlen )
{
	/*
	GoldSRC::edict_t *pGoldSRCEdict = g_pGoldSRCEdict->GetGoldSRCEdict( pEntity, true );

	if ( !pGoldSRCEdict )
		return false;

	char szRejectReason[128];
	GoldSRC::qboolean bConnect =  g_pGoldSRCEntityInterface->ClientConnect( pGoldSRCEdict, pszName, pszAddress, szRejectReason );
	Q_strncpy( reject, szRejectReason, maxrejectlen );

	return bConnect != FALSE;
	*/

	return true;
}


void CGoldSRCServerGameClients::ClientPutInServer( edict_t *pEntity, char const *playername )
{
	CBaseEntity *pBaseEntity = g_pGoldSRCEdict->CreateEntity( pEntity->m_EdictIndex );
	if ( !pBaseEntity )
		return;

	GoldSRC::edict_t *pGoldSRCEdict = pBaseEntity->GoldSrcEdict();
	if ( !pGoldSRCEdict )
		return;

	g_pGoldSRCEntityInterface->ClientPutInServer( pGoldSRCEdict );
	
	// Need to do this manually
	pBaseEntity->PostGoldSrcCreate();
	pBaseEntity->PostGoldSrcSpawn();
}
