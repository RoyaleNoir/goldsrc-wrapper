#ifndef CLIENTENTITYLIST_H
#define CLIENTENTITYLIST_H

#ifdef _WIN32
#pragma once
#endif

#include "icliententitylist.h"
#include "basehandle.h"

class CClientEntityList : public IClientEntityList
{
public:
	virtual IClientNetworkable	*GetClientNetworkable( int entnum );
	virtual IClientNetworkable	*GetClientNetworkableFromHandle( CBaseHandle hEnt );
	virtual IClientUnknown		*GetClientUnknownFromHandle( CBaseHandle hEnt );

	virtual IClientEntity		*GetClientEntity( int entnum );
	virtual IClientEntity		*GetClientEntityFromHandle( CBaseHandle hEnt );

	virtual int					NumberOfEntities( bool bIncludeNonNetworkable );

	virtual int					GetHighestEntityIndex( void );

	virtual void				SetMaxEntities( int maxents );
	virtual int					GetMaxEntities();
};


extern CClientEntityList *g_pEntityList;

#endif // CLIENTENTITYLIST_H