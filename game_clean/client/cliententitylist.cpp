#include "cbase.h"
#include "cliententitylist.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


static CClientEntityList g_EntityList;
CClientEntityList *g_pEntityList = &g_EntityList;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CClientEntityList, IClientEntityList, VCLIENTENTITYLIST_INTERFACE_VERSION, g_EntityList );


IClientNetworkable* CClientEntityList::GetClientNetworkable( int entnum )
{
	LOG_STUB();

	return nullptr;
}


IClientNetworkable *CClientEntityList::GetClientNetworkableFromHandle( CBaseHandle hEnt )
{
	LOG_STUB();

	return nullptr;
}


IClientUnknown *CClientEntityList::GetClientUnknownFromHandle( CBaseHandle hEnt )
{
	LOG_STUB();

	return nullptr;
}


IClientEntity *CClientEntityList::GetClientEntity( int entnum )
{
	LOG_STUB();

	return nullptr;
}


IClientEntity *CClientEntityList::GetClientEntityFromHandle( CBaseHandle hEnt )
{
	LOG_STUB();

	return nullptr;
}


int CClientEntityList::NumberOfEntities( bool bIncludeNonNetworkable )
{
	LOG_STUB();

	return 0;
}


int CClientEntityList::GetHighestEntityIndex( void )
{
	LOG_STUB();

	return 0;
}


void CClientEntityList::SetMaxEntities( int maxents )
{
	LOG_STUB();
}


int CClientEntityList::GetMaxEntities()
{
	LOG_STUB();

	return 0;
}
