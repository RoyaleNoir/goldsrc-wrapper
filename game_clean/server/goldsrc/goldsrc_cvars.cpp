#include "cbase.h"
#include "goldsrc_cvars.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

// These ones are read directly by hl.dll
GoldSRC::cvar_t sv_gravity = { "sv_gravity", "800" };
GoldSRC::cvar_t mp_footsteps = { "mp_footsteps", "1" };
GoldSRC::cvar_t sv_aim = { "sv_aim", "1", GoldSRC::G_FCVAR_SERVER | GoldSRC::G_FCVAR_ARCHIVE };

// These are read through the functions
ConVar sv_skyname( "sv_skyname", "desert" );	// TODO: Replicated
ConVar sv_language( "sv_language", "0" );
ConVar showtriggers( "showtriggers", "0", FCVAR_CHEAT, "Shows trigger brushes" );


static CGoldSRCCVars g_GoldSRCCVars;
CGoldSRCCVars *g_pGoldSRCCVars = &g_GoldSRCCVars;


CGoldSRCCVars::CGoldSRCCVars()
{
}


CGoldSRCCVars::~CGoldSRCCVars()
{
	m_OwnedConVars.PurgeAndDeleteElements();
}


void CGoldSRCCVars::Init()
{
	Register( &sv_gravity, false );
	Register( &mp_footsteps, false );
	Register( &sv_aim, false );
}


void CGoldSRCCVars::Shutdown()
{
	// This is called just before the goldsrc dll is unloaded

	for ( int i = m_CVars.First(); i != m_CVars.InvalidIndex(); i = m_CVars.Next( i ) )
	{
		GoldSRC::cvar_t *pCVar = m_CVars.Element( i );
		free( pCVar->string );
	}

	for ( int i = 0; i < m_OwnedCVars.Count(); i++ )
	{
		GoldSRC::cvar_t *pCVar = m_OwnedCVars.Element( i );
		free( pCVar->name );
	}

	m_CVars.Purge();
	m_OwnedCVars.PurgeAndDeleteElements();
}


void CGoldSRCCVars::Register( GoldSRC::cvar_t *pCvar, bool bFromDLL )
{
	if ( !pCvar )
		return;

	if ( g_pCVar->FindVar( pCvar->name ) )
	{
		Assert( 0 );
		return;
	}

	// Copy the value into a dynamically allocated string
	const char* originalstring = pCvar->string;
	int stringLen = Q_strlen( originalstring );
	pCvar->string = (char *)malloc( stringLen + 1 );
	Q_strcpy( pCvar->string, originalstring );

	// Set the float value
	pCvar->value = Q_atof( pCvar->string );

	// Mark this as added by the EXTDLL
	if ( bFromDLL )
	{
		pCvar->flags |= GoldSRC::G_FCVAR_EXTDLL;
	}

	// Register ConVar
	ConVar *pConVar = new ConVar( pCvar->name, originalstring, 0 );
	//pConVar->InstallChangeCallback( &CGoldSRCCVars::ConVarChangeCallback );
	m_OwnedConVars.AddToTail( pConVar );

	m_CVars.Insert( pCvar->name, pCvar );
}


void CGoldSRCCVars::RegisterVariable( GoldSRC::cvar_t *variable )
{
	LOG_STUB();
}


GoldSRC::cvar_t *CGoldSRCCVars::Find( const char *szVarName )
{
	int index = m_CVars.Find( szVarName );

	if ( m_CVars.IsValidIndex( index ) )
		return m_CVars.Element( index );

	return NULL;
}


GoldSRC::cvar_t *CGoldSRCCVars::GetPointer( const char *szVarName )
{
	GoldSRC::cvar_t *pCvar = Find( szVarName );

	if ( pCvar )
		return pCvar;

	// Check for Source ConVars with the same name and add them
	ConVar *pSourceVar = g_pCVar->FindVar( szVarName );
	if ( pSourceVar )
		return AddConVar( pSourceVar );

	Warning( "Could not find ConVar %s\n", szVarName );
	return NULL;
}


float CGoldSRCCVars::GetFloat( const char *szVarName )
{
	GoldSRC::cvar_t *pVar = GetPointer( szVarName );
	return pVar ? pVar->value : NULL;
}


const char *CGoldSRCCVars::GetString( const char *szVarName )
{
	GoldSRC::cvar_t *pVar = GetPointer( szVarName );
	return pVar ? pVar->string : NULL;
}


void CGoldSRCCVars::SetFloat( const char *szVarName, float flValue )
{
	// TODO: Source ConVar
}


void CGoldSRCCVars::SetString( const char *szVarName, const char *szValue )
{
	// TODO: Source ConVar
}


void CGoldSRCCVars::DirectSet( GoldSRC::cvar_t *var, char *value )
{
	LOG_STUB();
}


GoldSRC::cvar_t *CGoldSRCCVars::AddConVar( ConVar *pConVar )
{
	if ( !pConVar )
		return NULL;

	GoldSRC::cvar_t *pCvar = new GoldSRC::cvar_t;

	// Copy name
	const char *szName = pConVar->GetName();
	int nNameLength = Q_strlen( szName );
	pCvar->name = (char *)malloc( nNameLength + 1 );
	Q_strcpy( pCvar->name, szName );

	// Copy string
	const char *szString = pConVar->GetString();
	int nStringLength = Q_strlen( szString );
	pCvar->string = (char *)malloc( nStringLength + 1 );
	Q_strcpy( pCvar->string, szString );

	// Set flags
	if ( pConVar->IsFlagSet( FCVAR_GAMEDLL ) )
		pCvar->flags |= GoldSRC::G_FCVAR_SERVER;

	// Set value
	pCvar->value = pConVar->GetFloat();

	m_OwnedCVars.AddToTail( pCvar );
	return pCvar;
}


void CGoldSRCCVars::ConVarChangeCallback( IConVar *var, const char *pOldValue, float flOldValue )
{
	const ConVar *pConVar = g_pCVar->FindVar( var->GetName() );
	GoldSRC::cvar_t *pCvar = g_pGoldSRCCVars->Find( var->GetName() );

	// Happens when first initialized
	if ( !pCvar )
		return;

	float flValue = pConVar->GetFloat();

	char szTemp[32];
	Q_snprintf( szTemp, 32, "%f", flValue );

	// Reallocate new string for new value
	free( pCvar->string );
	int stringLength = Q_strlen( szTemp );
	pCvar->string = (char *)malloc( stringLength + 1 );
	Q_strcpy( pCvar->string, szTemp );

	// Update value
	pCvar->value = flValue;
}
