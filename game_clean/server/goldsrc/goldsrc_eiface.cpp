#include "cbase.h"
#include "goldsrc_eiface.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_servergameents.h"
#include "goldsrc_cvars_shared.h"
#include "goldsrc_edict.h"
#include "goldsrc_globalvars.h"
#include "goldsrc_trace.h"
#include "goldsrc_usermessages_shared.h"
#include "engine/IEngineSound.h"
#include "engine/IEngineTrace.h"
#include "engine/ivmodelinfo.h"
#include "utlhashtable.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineServer *engine;
extern IEngineSound *enginesound;
extern IEngineTrace *enginetrace;
extern IVModelInfo *modelinfo;
extern IFileSystem *filesystem;
extern CGlobalVars *gpGlobals;


namespace GoldSRC
{
	class StringPool
	{
	public:
		StringPool() : m_Strings( 256 ) {}

		const char *AllocString( const char *szValue )
		{
			return m_Strings[ m_Strings.Insert( szValue ) ].Get();
		}

		void FreeStrings()
		{
			m_Strings.Purge();
		}

	private:
		CUtlHashtable<CUtlConstString> m_Strings;
	};

	static StringPool stringPool;

	const char* szStringBase = "";

	int AllocString( const char *szValue );
	const char *SzFromIndex( int iString );

	void FreeStrings()
	{
		stringPool.FreeStrings();
	}

	int PrecacheModel( char *s )
	{
		if ( !s || !*s )
			return 0;

		int index = engine->PrecacheModel( s );

		return index;
	}


	int PrecacheSound( char *s )
	{
		// GoldSrc would return an index, but it's never used so I think it's fine just to send 1 or 0
		return enginesound->PrecacheSound( s, true );
	}


	void SetModel( edict_t *e, const char *m )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( e );
		if ( pEnt ) 
		{
			pEnt->SetModel( m );
		}
	}


	int ModelIndex( const char *m )
	{
		if ( !m )
			return 0;

		return modelinfo->GetModelIndex( m );
	}


	int ModelFrames( int modelIndex )
	{
		LOG_STUB();
		return 0;
	}


	void SetSize( edict_t *e, const float *rgflMin, const float *rgflMax )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( e );
		if ( pEnt ) 
		{
			pEnt->SetSize( rgflMin, rgflMax );
		}
	}


	void ChangeLevel( char *s1, char *s2 )
	{
		LOG_STUB();
	}


	void GetSpawnParms( edict_t *ent )
	{
		LOG_STUB();
	}


	void SaveSpawnParms( edict_t *ent )
	{
		LOG_STUB();
	}


	float VecToYaw( const float *rgflVector )
	{
		QAngle vecAngle;
		::VectorAngles( *(Vector*)rgflVector, vecAngle );
		return vecAngle.y;
	}


	void VecToAngles( const float *rgflVectorIn, float *rgflVectorOut )
	{
		QAngle vecAngle;
		::VectorAngles( *(Vector*)rgflVectorIn, vecAngle );

		rgflVectorOut[0] = vecAngle.x;
		rgflVectorOut[1] = vecAngle.y;
		rgflVectorOut[2] = vecAngle.z;
	}


	void MoveToOrigin( edict_t *ent, const float *pflGoal, float dist, int iMoveType )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( ent );
		if ( pEnt ) 
		{
			pEnt->MoveToGoal( pflGoal, dist, iMoveType );
		}
	}

	
	void ChangeYaw( edict_t *ent )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( ent );
		if ( pEnt ) 
		{
			pEnt->ChangeYaw();
		}
	}


	void ChangePitch( edict_t *ent )
	{
		LOG_STUB();
	}


	edict_t *FindEntityByString( edict_t *pEdictStartSearchAfter, const char *pszField, const char *pszValue )
	{
		if ( !Q_strcmp( pszField, "classname" ) )
		{
			return g_pGoldSRCEdict->FindByClassName( pEdictStartSearchAfter, pszValue );
		}

		if ( !Q_strcmp( pszField, "targetname" ) )
		{
			return g_pGoldSRCEdict->FindByTargetName( pEdictStartSearchAfter, pszValue );
		}

		LOG_STUB();
		return NULL;
	}


	int GetEntityIllum( edict_t *pEnt )
	{
		LOG_STUB();
		return 0;
	}


	edict_t *FindEntityInSphere( edict_t *pEdictStartSearchAfter, const float *org, float rad )
	{
		return g_pGoldSRCEdict->FindInSphere( pEdictStartSearchAfter, org, rad );
	}


	edict_t *FindClientInPVS( edict_t *pEdict )
	{
		for ( int i = 1; i <= gpGlobals->maxClients; i++ )
		{
			CBaseEntity *pPlayer = g_pGoldSRCServerGameEnts->GetEntityByIndex( i );
			if ( !pPlayer )
				continue;

			if ( !pPlayer->IsPlayer() )
				return NULL;

			Vector eyePos;
			VectorAdd( pPlayer->EntVars()->origin, pPlayer->EntVars()->view_ofs, eyePos.Base() );

			::byte pvs[MAX_MAP_LEAFS / 8];

			int cluster = engine->GetClusterForOrigin( eyePos );
			engine->GetPVSForCluster( cluster, sizeof( pvs ), pvs );
			if ( engine->CheckOriginInPVS( eyePos, pvs, sizeof( pvs ) ) )
				return pPlayer->GoldSrcEdict();
		}

		return NULL;
	}


	edict_t *EntitiesInPVS( edict_t *pPlayer )
	{
		LOG_STUB();
		return NULL;
	}


	void MakeVectors( const float *rgflVector )
	{
		::AngleVectors( 
			*(QAngle*)rgflVector, 
			(Vector*)g_pGoldSRCGlobalVars->v_forward, 
			(Vector*)g_pGoldSRCGlobalVars->v_right, 
			(Vector*)g_pGoldSRCGlobalVars->v_up 
		);
	}

	
	void AngleVectors( const float *rgflVector, float *forward, float *right, float *up )
	{
		::AngleVectors( *(QAngle*)rgflVector, (Vector *)forward, (Vector *)right, (Vector *)up );
	}


	edict_t *CreateEntity( void )
	{
		return g_pGoldSRCEdict->CreateGoldSRCEntity();
	}


	void RemoveEntity( edict_t *e )
	{
		g_pGoldSRCEdict->RemoveEntityByGoldSRCEdict( e );
	}


	edict_t *CreateNamedEntity( int className )
	{
		return g_pGoldSRCEdict->CreateGoldSRCEntityByName( className );
	}


	void MakeStatic( edict_t *ent )
	{
		LOG_STUB();
	}


	int EntIsOnFloor( edict_t *e )
	{
		LOG_STUB();
		return 0;
	}


	int DropToFloor( edict_t *e )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( e );
		if ( pEnt ) 
		{
			return pEnt->DropToFloor();
		}

		return 0;
	}


	int WalkMove( edict_t *ent, float yaw, float dist, int iMode )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( ent );
		if ( pEnt ) 
		{
			if ( !( pEnt->EntVars()->flags & ( G_FL_ONGROUND | G_FL_FLY | G_FL_SWIM ) ) )
			{
				return 0;
			}

			yaw = yaw * M_PI_F * 2 / 360;
			Vector move( cos( yaw ) * dist, sin( yaw ) * dist, 0 );
			return pEnt->MoveStep( move, true );
		}

		return 0;
	}


	void SetOrigin( edict_t *e, const float *rgflOrigin )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( e );
		if ( pEnt ) 
		{
			pEnt->SetOrigin( rgflOrigin );
		}
	}

	class CPlaySoundRecipientFilter : public IRecipientFilter
	{
	public:
		CPlaySoundRecipientFilter()
		{
			for ( int i = 1; i <= gpGlobals->maxClients; i++ )
			{
				CBaseEntity *pEntity = g_pGoldSRCServerGameEnts->GetEntityByIndex( i );
				if ( !pEntity )
					return;
	
				// Check if player
				if ( pEntity->IsPlayer() )
					m_Recipients.AddToTail( i );
			}
		}
	
		virtual bool IsReliable( void ) const
		{
			return false;
		}
	
		virtual bool IsInitMessage( void ) const
		{
			return false;
		}
	
		virtual int GetRecipientCount( void ) const
		{
			return m_Recipients.Size();
		}
	
		virtual int GetRecipientIndex( int slot ) const
		{
			return m_Recipients.Element( slot );
		}
	
	private:
		CUtlVector<int> m_Recipients;
	};

	void EmitSound( edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch )
	{
		// Recipient filter
		CPlaySoundRecipientFilter filter;

		// entity index
		int entindex = g_pGoldSRCEdict->ToIndex( entity );

		// Channel ids are backwards compatible

		// Convert the attenuation to a soundlevel
		soundlevel_t sndlvl = (soundlevel_t)(int)( ( attenuation != 0 ) ? ( 50 + 20 / attenuation ) : 0 );

		// Convert flags
		int flags = 0;

		if ( fFlags & GoldSRC::G_SND_STOP )
			flags |= SND_STOP;
		if ( fFlags & GoldSRC::G_SND_CHANGE_VOL )
			flags |= SND_CHANGE_VOL;
		if ( fFlags & GoldSRC::G_SND_CHANGE_PITCH )
			flags |= SND_CHANGE_PITCH;
		if ( fFlags & GoldSRC::G_SND_SPAWNING )
			flags |= SND_SPAWNING;

		if ( sample[0] == '!' )
		{
			enginesound->EmitSentenceByIndex( filter, entindex, channel, atoi( sample + 1 ), volume, sndlvl, flags, pitch );
		}
		else
		{
			enginesound->EmitSound( filter, entindex, channel, sample, volume, sndlvl, flags, pitch );
		}
	}


	void EmitAmbientSound( edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int iFlags, int pitch )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( entity );
		if ( !pEnt )
			return;

		int entindex = pEnt->GetSourceEdict()->m_EdictIndex;

		// Convert the origin
		Vector origin = *( Vector *)pos;

		// Convert the attenuation to a soundlevel
		soundlevel_t sndlvl = (soundlevel_t)(int)( ( attenuation != 0 ) ? ( 50 + 20 / attenuation ) : 0 );

		// Convert flags
		int flags = 0;

		if ( iFlags & G_SND_STOP )
			flags |= SND_STOP;
		if ( iFlags & G_SND_CHANGE_VOL )
			flags |= SND_CHANGE_VOL;
		if ( iFlags & G_SND_CHANGE_PITCH )
			flags |= SND_CHANGE_PITCH;
		if ( iFlags & G_SND_SPAWNING )
			flags |= SND_SPAWNING;

		engine->EmitAmbientSound( entindex, origin, samp, vol, sndlvl, flags, pitch );
	}


	void TraceLine( const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( pentToSkip );

		Vector vecStart;
		Vector vecEnd;
		VectorCopy( v1, vecStart.Base() );
		VectorCopy( v2, vecEnd.Base() );

		UTIL_TraceLine( vecStart, vecEnd, fNoMonsters, pEnt, ptr );
	}

	
	void TraceToss( edict_t *pEnt, edict_t *pentToIgnore, TraceResult *ptr )
	{
		LOG_STUB();
	}


	int TraceMonsterHull( edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr )
	{
		//LOG_STUB();
		return 0;
	}

	
	void TraceHull( const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( pentToSkip );

		Vector vecStart;
		Vector vecEnd;
		VectorCopy( v1, vecStart.Base() );
		VectorCopy( v2, vecEnd.Base() );

		UTIL_TraceHull( vecStart, vecEnd, fNoMonsters, hullNumber, pEnt, ptr );
	}


	void TraceModel( const float *v1, const float *v2, int hullNumber, edict_t *pent, TraceResult *ptr )
	{
		LOG_STUB();
	}


	const char* TraceTexture( edict_t *pTextureEntity, const float *v1, const float *v2 )
	{
		LOG_STUB();
		return NULL;
	}


	void TraceSphere( const float *v1, const float *v2, int fNoMonsters, float radius, edict_t *pentToSkip, TraceResult *ptr )
	{
		LOG_STUB();
	}


	void GetAimVector( edict_t *ent, float speed, float *rgflReturn )
	{
		LOG_STUB();
	}


	void ServerCommand( char *str )
	{
		engine->ServerCommand( str );
	}


	void ServerExecute( void )
	{
		engine->ServerExecute();
	}


	void ClientCommand( edict_t *pEdict, char *szFmt, ... )
	{
		LOG_STUB();
	}


	void ParticleEffect( const float *org, const float *dir, float color, float count )
	{
		LOG_STUB();
	}


	void LightStyle( int style, char *val )
	{
		engine->LightStyle( style, val );
	}


	int DecalIndex( const char *name )
	{
		LOG_STUB();
		return 0;
	}


	int PointContents( const float *rgflVector )
	{
		//LOG_STUB();
		return 0;
	}


	void MessageBegin( int msg_dest, int msg_type, const float *pOrigin, edict_t *ed )
	{
		UserMessages()->MessageBegin( msg_dest, msg_type, pOrigin, ed );
	}


	void MessageEnd( void )
	{
		UserMessages()->MessageEnd();
	}


	void WriteByte( int iValue )
	{
		UserMessages()->WriteByte( iValue );
	}


	void WriteChar( int iValue )
	{
		UserMessages()->WriteChar( iValue );
	}


	void WriteShort( int iValue )
	{
		UserMessages()->WriteShort( iValue );
	}


	void WriteLong( int iValue )
	{
		UserMessages()->WriteLong( iValue );
	}


	void WriteAngle( float flValue )
	{
		UserMessages()->WriteAngle( flValue );
	}


	void WriteCoord( float flValue )
	{
		UserMessages()->WriteCoord( flValue );
	}


	void WriteString( const char *sz )
	{
		UserMessages()->WriteString( sz );
	}


	void WriteEntity( int iValue )
	{
		UserMessages()->WriteEntity( iValue );
	}


	void CVarRegister( cvar_t *pCvar )
	{
		g_pGoldSRCCVars->Register( pCvar );
	}


	float CVarGetFloat( const char *szVarName )
	{
		return g_pGoldSRCCVars->GetFloat( szVarName );
	}


	const char *CVarGetString( const char *szVarName )
	{
		return g_pGoldSRCCVars->GetString( szVarName );
	}


	void CVarSetFloat( const char *szVarName, float flValue )
	{
		g_pGoldSRCCVars->SetFloat( szVarName, flValue );
	}


	void CVarSetString( const char *szVarName, const char *szValue )
	{
		g_pGoldSRCCVars->SetString( szVarName, szValue );
	}


	void AlertMessage( ALERT_TYPE atype, char *szFmt, ... )
	{
		DevMsg( "<G> " );
		
		char szMessage[128];
		
		va_list args;
		va_start( args, szFmt );
		Q_vsnprintf( szMessage, 128, szFmt, args );
		va_end( args );
		
		switch ( atype )
		{
		case at_console:
			Msg( szMessage );
			break;
		case at_aiconsole:
			DevMsg( szMessage );
			break;
		case at_warning:
			Warning( szMessage );
			break;
		case at_error:
			Warning( "[E] " );
			Warning( szMessage );
			break;
		default:
			break;
		}
	}


	void EngineFprintf( void *pFile, char *szFmd, ... )
	{
		LOG_STUB();
	}


	void *PvAllocEntPrivateData( edict_t *pEdict, int32 cb )
	{
		return g_pGoldSRCEdict->AllocPrivateData( pEdict, cb );
	}


	void *PvEntPrivateData( edict_t *pEdict )
	{
		return g_pGoldSRCEdict->GetPrivateData( pEdict );
	}


	void FreeEntPrivateData( edict_t *pEdict )
	{
		g_pGoldSRCEdict->FreePrivateData( pEdict );
	}


	const char *SzFromIndex( int iString )
	{
		return (const char *)( iString + (int)szStringBase );
	}


	int AllocString( const char *szValue )
	{
		// Don't allocate empty string
		if ( !szValue || *szValue == '\0' )
			return 0;

		return (int)( stringPool.AllocString( szValue ) - (int)szStringBase );
	}


	entvars_t *GetVarsOfEnt( edict_t *pEdict )
	{
		LOG_STUB();
		return NULL;
	}


	edict_t *PEntityOfEntOffset( int iEntOffset )
	{
		return g_pGoldSRCEdict->FromOffset( iEntOffset );
	}


	int EntOffsetOfPEntity( const edict_t *pEdict )
	{
		return g_pGoldSRCEdict->ToOffset( pEdict );
	}


	int IndexOfEdict( const edict_t *pEdict )
	{
		return g_pGoldSRCEdict->ToIndex( pEdict );
	}


	edict_t *PEntityOfEntIndex( int iEntIndex )
	{
		return g_pGoldSRCEdict->FromIndex( iEntIndex );
	}


	edict_t *FindEntityByVars( entvars_t *pvars )
	{
		LOG_STUB();
		return NULL;
	}


	void *GetModelPtr( edict_t *pEdict )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( pEdict );
		if ( pEnt ) 
		{
			return pEnt->GetModelPtr();
		}

		return 0;
	}


	int RegUserMsg( const char *pszName, int iSize )
	{
		return UserMessages()->Register( pszName, iSize );
	}


	void AnimationAutomove( const edict_t *pEdict, float flTime )
	{
		LOG_STUB();
	}


	void GetBonePosition( const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
	{
		CBaseEntity *pEnt = g_pGoldSRCEdict->GetSourceEntity( (edict_t *)pEdict );
		if ( pEnt ) 
		{
			pEnt->GetBonePosition( iBone, rgflOrigin, rgflAngles );
		}
	}


	uint32 FunctionFromName( const char *pName )
	{
		LOG_STUB();
		return 0;
	}


	const char *NameForFunction( uint32 function )
	{
		//LOG_STUB();
		return "";	// SHUT UP
	}


	void ClientPrintf( edict_t *pEdict, PRINT_TYPE ptype, const char *szMsg )
	{
		LOG_STUB();
	}


	void ServerPrint( const char *szMsg )
	{
		LOG_STUB();
	}


	const char *Cmd_Args( void )
	{
		LOG_STUB();
		return NULL;
	}


	const char *Cmd_Argv( int argc )
	{
		LOG_STUB();
		return NULL;
	}
	

	int Cmd_Argc( void )
	{
		LOG_STUB();
		return 0;
	}


	void GetAttachment( const edict_t *pEdict, int iAttachment, float *rgflOrigin, float *rgflAngles )
	{
		//LOG_STUB();
	}


	void CRC32_Init( CRC32_t *pulCRC )
	{
		LOG_STUB();
	}


	void CRC32_ProcessBuffer( CRC32_t *pulCRC, void *p, int len )
	{
		LOG_STUB();
	}


	void CRC32_ProcessByte( CRC32_t *pulCRC, unsigned char ch )
	{
		LOG_STUB();
	}


	CRC32_t CRC32_Final( CRC32_t pulCRC )
	{
		LOG_STUB();
		return 0;
	}


	int32 RandomLong( int32 lLow, int32 lHigh )
	{
		return RandomInt( lLow, lHigh );
	}


	float RandomFloat( float flLow, float flHigh )
	{
		return ::RandomFloat( flLow, flHigh );
	}


	void SetView( const edict_t *pClient, const edict_t *pViewent )
	{
		LOG_STUB();
	}


	float Time( void )
	{
		LOG_STUB();
		return 0.0f;
	}


	void CrosshairAngle( const edict_t *pClient, float pitch, float yaw )
	{
		LOG_STUB();
	}


	byte *LoadFileForMe( char *filename, int *pLength )
	{
		FileHandle_t hFile = filesystem->Open( filename, "r", "GAME" );
		if ( !hFile )
			return NULL;

		*pLength = filesystem->Size( hFile );

		byte *filebytes = (byte*)malloc( *pLength );

		filesystem->Read( filebytes, *pLength, hFile );
		filesystem->Close( hFile );

		return filebytes;
	}


	void FreeFile( void *buffer )
	{
		free( buffer );
	}


	void EndSection( const char *pszSectionName )
	{
		LOG_STUB();
	}


	int CompareFileTime( char *filename1, char *filename2, int *iCompare )
	{
		LOG_STUB();
		return 0;
	}


	void GetGameDir( char *szGetGameDir )
	{
		char searchPaths[MAX_PATH];
		filesystem->GetSearchPath( "GAME", false, searchPaths, MAX_PATH );

		char *pPath = strtok( searchPaths, ";" );
		Q_strcpy( szGetGameDir, pPath );
	}


	void Cvar_RegisterVariable( cvar_t *variable )
	{
		g_pGoldSRCCVars->RegisterVariable( variable );
	}


	void FadeClientVolume( const edict_t *pEdict, int fadePercent, int fadeOutSecond, int holdTime, int fadeInSeconds )
	{
		LOG_STUB();
	}


	void SetClientMaxspeed( const edict_t *pEdict, float fNewMaxspeed )
	{
		LOG_STUB();
	}


	edict_t *CreateFakeClient( const char *netname )
	{
		LOG_STUB();
		return NULL;
	}


	void RunPlayerMove( edict_t *fakeclient, const float *viewangles, float forwardmove, float sidemove, float upmove, unsigned short buttons, byte impulse, byte msec )
	{
		LOG_STUB();
	}


	int NumberOfEntities( void )
	{
		LOG_STUB();
		return 0;
	}


	char *GetInfoKeyBuffer( edict_t *e )
	{
		LOG_STUB();
		return NULL;
	}


	char *InfoKeyValue( char *infobuffer, char *key )
	{
		LOG_STUB();
		return NULL;
	}


	void SetKeyValue( char *infobuffer, char *key, char *value )
	{
		LOG_STUB();
	}


	void SetClientKeyValue( int clientIndex, char *infobuffer, char *key, char *value )
	{
		LOG_STUB();
	}


	int IsMapValid( char *filename )
	{
		LOG_STUB();
		return 0;
	}


	void StaticDecal( const float *origin, int decalIndex, int  entityIndex, int modelIndex )
	{
		LOG_STUB();
	}


	int PrecacheGeneric( char *s )
	{
		LOG_STUB();
		return 0;
	}


	int GetPlayerUserId( edict_t *e )
	{
		LOG_STUB();
		return 0;
	}


	void BuildSoundMsg( edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch, int msg_dest, int msg_type, const float *pOrigin, edict_t *ed )
	{
		LOG_STUB();
	}

	
	int IsDedicatedServer( void )
	{
		LOG_STUB();
		return 0;
	}


	cvar_t *CVarGetPointer( const char *szVarName )
	{
		return g_pGoldSRCCVars->GetPointer( szVarName );
	}


	unsigned int GetPlayerWONId( edict_t *e )
	{
		LOG_STUB();
		return 0;
	}


	void Info_RemoveKey( char *s, const char *key )
	{
		LOG_STUB();
	}


	const char *GetPhysicsKeyValue( const edict_t *pClient, const char *key )
	{
		LOG_STUB();
		return NULL;
	}


	void SetPhysicsKeyValue( const edict_t *pClient, const char *key, const char *value )
	{
		LOG_STUB();
	}


	const char *GetPhysicsInfoString( const edict_t *pClient )
	{
		LOG_STUB();
		return NULL;
	}


	unsigned short PrecacheEvent( int type, const char *psz )
	{
		LOG_STUB();
		return 0;
	}


	void PlaybackEvent( int flags, const edict_t *pInvoker, unsigned short eventindex, float delay, float *origin, float *angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2 )
	{
		LOG_STUB();
	}


	unsigned char *SetFatPVS( float *org )
	{
		LOG_STUB();
		return NULL;
	}


	unsigned char *SetFatPAS( float *org )
	{
		LOG_STUB();
		return NULL;
	}


	int CheckVisibility( const edict_t *entity, unsigned char *pset )
	{
		LOG_STUB();
		return 0;
	}


	void DeltaSetField( delta_s *pFields, const char *fieldname )
	{
		LOG_STUB();
	}


	void DeltaUnsetField( delta_s *pFields, const char *fieldname )
	{
		LOG_STUB();
	}


	void DeltaAddEncoder( char *name, void ( *conditionalencode )( delta_s *pFields, const unsigned char *from, const unsigned char *to ) )
	{
		LOG_STUB();
	}


	int GetCurrentPlayer( void )
	{
		LOG_STUB();
		return 0;
	}


	int CanSkipPlayer( const edict_t *player )
	{
		LOG_STUB();
		return 0;
	}


	int DeltaFindField( delta_s *pFields, const char *fieldname )
	{
		LOG_STUB();
		return 0;
	}


	void DeltaSetFieldByIndex( delta_s *pFields, int fieldNumber )
	{
		LOG_STUB();
	}


	void DeltaUnsetFieldByIndex( delta_s *pFields, int fieldNumber )
	{
		LOG_STUB();
	}


	void SetGroupMask( int mask, int op )
	{
		LOG_STUB();
	}


	int CreateInstancedBaseline( int classname, entity_state_t *baseline )
	{
		LOG_STUB();
		return 0;
	}


	void Cvar_DirectSet( cvar_t *var, char *value )
	{
		g_pGoldSRCCVars->DirectSet( var, value );
	}


	void ForceUnmodified( FORCE_TYPE type, float *mins, float *maxs, const char *filename )
	{
		LOG_STUB();
	}


	void GetPlayerStats( const edict_t *pClient, int *ping, int *packet_loss )
	{
		LOG_STUB();
	}


	void AddServerCommand( char *cmd_name, void ( *function )( void ) )
	{
		LOG_STUB();
	}


	qboolean Voice_GetClientListening( int iReceiver, int iSender )
	{
		LOG_STUB();
		return 0;
	}


	qboolean Voice_SetClientListening( int iReceiver, int iSender, qboolean bListen )
	{
		LOG_STUB();
		return 0;
	}


	const char *GetPlayerAuthId( edict_t *e )
	{
		LOG_STUB();
		return NULL;
	}


	sequenceEntry_s *SequenceGet( const char *fileName, const char *entryName )
	{
		LOG_STUB();
		return NULL;
	}


	sentenceEntry_s *SequencePickSentence( const char *groupName, int pickMethod, int *picked )
	{
		LOG_STUB();
		return NULL;
	}


	int GetFileSize( char *filename )
	{
		LOG_STUB();
		return 0;
	}


	unsigned int GetApproxWavePlayLen( const char *filepath )
	{
		LOG_STUB();
		return 0;
	}


	int IsCareerMatch( void )
	{
		LOG_STUB();
		return 0;
	}


	int GetLocalizedStringLength( const char *label )
	{
		LOG_STUB();
		return 0;
	}


	void RegisterTutorMessageShown( int mid )
	{
		LOG_STUB();
	}


	int GetTimesTutorMessageShown( int mid )
	{
		LOG_STUB();
		return 0;
	}


	void ProcessTutorMessageDecayBuffer( int *buffer, int bufferLength )
	{
		LOG_STUB();
	}


	void ConstructTutorMessageDecayBuffer( int *buffer, int bufferLength )
	{
		LOG_STUB();
	}


	void ResetTutorMessageDecayData( void )
	{
		LOG_STUB();
	}


	void QueryClientCvarValue( const edict_t *player, const char *cvarName )
	{
		LOG_STUB();
	}


	void QueryClientCvarValue2( const edict_t *player, const char *cvarName, int requestID )
	{
		LOG_STUB();
	}


	int CheckParm( const char *pchCmdLineToken, char **ppnext )
	{
		LOG_STUB();
		return 0;
	}
}


GoldSRC::enginefuncs_t g_GoldSRCEngineFuncs = {
	&GoldSRC::PrecacheModel,
	&GoldSRC::PrecacheSound,
	&GoldSRC::SetModel,
	&GoldSRC::ModelIndex,
	&GoldSRC::ModelFrames,
	&GoldSRC::SetSize,
	&GoldSRC::ChangeLevel,
	&GoldSRC::GetSpawnParms,
	&GoldSRC::SaveSpawnParms,
	&GoldSRC::VecToYaw,
	&GoldSRC::VecToAngles,
	&GoldSRC::MoveToOrigin,
	&GoldSRC::ChangeYaw,
	&GoldSRC::ChangePitch,
	&GoldSRC::FindEntityByString,
	&GoldSRC::GetEntityIllum,
	&GoldSRC::FindEntityInSphere,
	&GoldSRC::FindClientInPVS,
	&GoldSRC::EntitiesInPVS,
	&GoldSRC::MakeVectors,
	&GoldSRC::AngleVectors,
	&GoldSRC::CreateEntity,
	&GoldSRC::RemoveEntity,
	&GoldSRC::CreateNamedEntity,
	&GoldSRC::MakeStatic,
	&GoldSRC::EntIsOnFloor,
	&GoldSRC::DropToFloor,
	&GoldSRC::WalkMove,
	&GoldSRC::SetOrigin,
	&GoldSRC::EmitSound,
	&GoldSRC::EmitAmbientSound,
	&GoldSRC::TraceLine,
	&GoldSRC::TraceToss,
	&GoldSRC::TraceMonsterHull,
	&GoldSRC::TraceHull,
	&GoldSRC::TraceModel,
	&GoldSRC::TraceTexture,
	&GoldSRC::TraceSphere,
	&GoldSRC::GetAimVector,
	&GoldSRC::ServerCommand,
	&GoldSRC::ServerExecute,
	&GoldSRC::ClientCommand,
	&GoldSRC::ParticleEffect,
	&GoldSRC::LightStyle,
	&GoldSRC::DecalIndex,
	&GoldSRC::PointContents,
	&GoldSRC::MessageBegin,
	&GoldSRC::MessageEnd,
	&GoldSRC::WriteByte,
	&GoldSRC::WriteChar,
	&GoldSRC::WriteShort,
	&GoldSRC::WriteLong,
	&GoldSRC::WriteAngle,
	&GoldSRC::WriteCoord,
	&GoldSRC::WriteString,
	&GoldSRC::WriteEntity,
	&GoldSRC::CVarRegister,
	&GoldSRC::CVarGetFloat,
	&GoldSRC::CVarGetString,
	&GoldSRC::CVarSetFloat,
	&GoldSRC::CVarSetString,
	&GoldSRC::AlertMessage,
	&GoldSRC::EngineFprintf,
	&GoldSRC::PvAllocEntPrivateData,
	&GoldSRC::PvEntPrivateData,
	&GoldSRC::FreeEntPrivateData,
	&GoldSRC::SzFromIndex,
	&GoldSRC::AllocString,
	&GoldSRC::GetVarsOfEnt,
	&GoldSRC::PEntityOfEntOffset,
	&GoldSRC::EntOffsetOfPEntity,
	&GoldSRC::IndexOfEdict,
	&GoldSRC::PEntityOfEntIndex,
	&GoldSRC::FindEntityByVars,
	&GoldSRC::GetModelPtr,
	&GoldSRC::RegUserMsg,
	&GoldSRC::AnimationAutomove,
	&GoldSRC::GetBonePosition,
	&GoldSRC::FunctionFromName,
	&GoldSRC::NameForFunction,
	&GoldSRC::ClientPrintf,
	&GoldSRC::ServerPrint,
	&GoldSRC::Cmd_Args,
	&GoldSRC::Cmd_Argv,
	&GoldSRC::Cmd_Argc,
	&GoldSRC::GetAttachment,
	&GoldSRC::CRC32_Init,
	&GoldSRC::CRC32_ProcessBuffer,
	&GoldSRC::CRC32_ProcessByte,
	&GoldSRC::CRC32_Final,
	&GoldSRC::RandomLong,
	&GoldSRC::RandomFloat,
	&GoldSRC::SetView,
	&GoldSRC::Time,
	&GoldSRC::CrosshairAngle,
	&GoldSRC::LoadFileForMe,
	&GoldSRC::FreeFile,
	&GoldSRC::EndSection,
	&GoldSRC::CompareFileTime,
	&GoldSRC::GetGameDir,
	&GoldSRC::Cvar_RegisterVariable,
	&GoldSRC::FadeClientVolume,
	&GoldSRC::SetClientMaxspeed,
	&GoldSRC::CreateFakeClient,
	&GoldSRC::RunPlayerMove,
	&GoldSRC::NumberOfEntities,
	&GoldSRC::GetInfoKeyBuffer,
	&GoldSRC::InfoKeyValue,
	&GoldSRC::SetKeyValue,
	&GoldSRC::SetClientKeyValue,
	&GoldSRC::IsMapValid,
	&GoldSRC::StaticDecal,
	&GoldSRC::PrecacheGeneric,
	&GoldSRC::GetPlayerUserId,
	&GoldSRC::BuildSoundMsg,
	&GoldSRC::IsDedicatedServer,
	&GoldSRC::CVarGetPointer,
	&GoldSRC::GetPlayerWONId,
	&GoldSRC::Info_RemoveKey,
	&GoldSRC::GetPhysicsKeyValue,
	&GoldSRC::SetPhysicsKeyValue,
	&GoldSRC::GetPhysicsInfoString,
	&GoldSRC::PrecacheEvent,
	&GoldSRC::PlaybackEvent,
	&GoldSRC::SetFatPVS,
	&GoldSRC::SetFatPAS,
	&GoldSRC::CheckVisibility,
	&GoldSRC::DeltaSetField,
	&GoldSRC::DeltaUnsetField,
	&GoldSRC::DeltaAddEncoder,
	&GoldSRC::GetCurrentPlayer,
	&GoldSRC::CanSkipPlayer,
	&GoldSRC::DeltaFindField,
	&GoldSRC::DeltaSetFieldByIndex,
	&GoldSRC::DeltaUnsetFieldByIndex,
	&GoldSRC::SetGroupMask,
	&GoldSRC::CreateInstancedBaseline,
	&GoldSRC::Cvar_DirectSet,
	&GoldSRC::ForceUnmodified,
	&GoldSRC::GetPlayerStats,
	&GoldSRC::AddServerCommand,
	&GoldSRC::Voice_GetClientListening,
	&GoldSRC::Voice_SetClientListening,
	&GoldSRC::GetPlayerAuthId,
	&GoldSRC::SequenceGet,
	&GoldSRC::SequencePickSentence,
	&GoldSRC::GetFileSize,
	&GoldSRC::GetApproxWavePlayLen,
	&GoldSRC::IsCareerMatch,
	&GoldSRC::GetLocalizedStringLength,
	&GoldSRC::RegisterTutorMessageShown,
	&GoldSRC::GetTimesTutorMessageShown,
	&GoldSRC::ProcessTutorMessageDecayBuffer,
	&GoldSRC::ConstructTutorMessageDecayBuffer,
	&GoldSRC::ResetTutorMessageDecayData,
	&GoldSRC::QueryClientCvarValue,
	&GoldSRC::QueryClientCvarValue2,
	&GoldSRC::CheckParm,
};

GoldSRC::enginefuncs_t *g_pGoldSRCEngineFuncs = &g_GoldSRCEngineFuncs;
