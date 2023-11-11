#ifndef GOLDSRC_CVARS_SHARED_H
#define GOLDSRC_CVARS_SHARED_H

#ifdef _WIN32
#pragma once
#endif

#include "utlvector.h"
#include "utldict.h"

namespace GoldSRC
{
	enum CVarFlags_t
	{
		G_FCVAR_ARCHIVE				= (1 << 0),
		G_FCVAR_USERINFO			= (1 << 1),
		G_FCVAR_SERVER				= (1 << 2),
		G_FCVAR_EXTDLL				= (1 << 3),
		G_FCVAR_CLIENTDLL			= (1 << 4),
		G_FCVAR_PROTECTED			= (1 << 5),
		G_FCVAR_SPONLY				= (1 << 6),
		G_FCVAR_PRINTABLEONLY		= (1 << 7),
		G_FCVAR_UNLOGGED			= (1 << 8),
		G_FCVAR_NOEXTRAWHITESPACE	= (1 << 9),
	};

	struct cvar_t
	{
		char *name;
		char *string;
		int flags;
		float value;
		cvar_t *next;
	};
}

class CGoldSRCCVars
{
public:
	CGoldSRCCVars();
	~CGoldSRCCVars();

	void Init();
	void Shutdown();

	void Register( GoldSRC::cvar_t *pCvar, bool bFromDLL = true );
	void RegisterVariable( GoldSRC::cvar_t *variable );

	GoldSRC::cvar_t *Find( const char *szVarName );
	GoldSRC::cvar_t *GetPointer( const char *szVarName );

	float GetFloat( const char *szVarName );
	const char *GetString( const char *szVarName );

	void SetFloat( const char *szVarName, float flValue );
	void SetString( const char *szVarName, const char *szValue );

	void DirectSet( GoldSRC::cvar_t *var, char *value );

private:
	GoldSRC::cvar_t *AddConVar( ConVar *pConVar );
	static void ConVarChangeCallback( IConVar *var, const char *pOldValue, float flOldValue );

	CUtlDict<GoldSRC::cvar_t *> m_CVars;
	CUtlVector<GoldSRC::cvar_t *> m_OwnedCVars;
	CUtlVector<ConVar *> m_OwnedConVars;
};

extern CGoldSRCCVars *g_pGoldSRCCVars;

#endif // GOLDSRC_CVARS_SHARED_H