#include "cbase.h"
#include "cliententitylist.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CClientEntityList g_EntityList;
CClientEntityList *g_pEntityList = &g_EntityList;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CClientEntityList, IClientEntityList, VCLIENTENTITYLIST_INTERFACE_VERSION, g_EntityList );


IClientNetworkable* CClientEntityList::GetClientNetworkable( int entnum )
{
	return nullptr;
}


IClientNetworkable *CClientEntityList::GetClientNetworkableFromHandle( CBaseHandle hEnt )
{
	return nullptr;
}


IClientUnknown *CClientEntityList::GetClientUnknownFromHandle( CBaseHandle hEnt )
{
	return nullptr;
}


IClientEntity *CClientEntityList::GetClientEntity( int entnum )
{
	return nullptr;
}


IClientEntity *CClientEntityList::GetClientEntityFromHandle( CBaseHandle hEnt )
{
	return nullptr;
}


int CClientEntityList::NumberOfEntities( bool bIncludeNonNetworkable )
{
	return 0;
}


int CClientEntityList::GetHighestEntityIndex( void )
{
	return 0;
}


void CClientEntityList::SetMaxEntities( int maxents )
{
}


int CClientEntityList::GetMaxEntities()
{
	return 0;
}
