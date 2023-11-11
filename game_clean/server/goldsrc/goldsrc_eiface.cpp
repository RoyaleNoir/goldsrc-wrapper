#include "cbase.h"
#include "goldsrc_eiface.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_cvars.h"
#include "goldsrc_edict.h"
#include "goldsrc_globalvars.h"
#include "engine/IEngineTrace.h"
#include "engine/ivmodelinfo.h"
#include "utlhashtable.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IVEngineServer *engine;
extern IVModelInfo *modelinfo;
extern IEngineTrace *enginetrace;


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
			return -1;

		int index = engine->PrecacheModel( s );

		return index;
	}


	int PrecacheSound( char *s )
	{
		//LOG_STUB();
		return 0;
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
			return -1;

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


	void MoveToOrigin( edict_t *end, const float *pflGoal, float dist, int iMoveType )
	{
		LOG_STUB();
	}

	
	void ChangeYaw( edict_t *ent )
	{
		LOG_STUB();
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
		LOG_STUB();
		return NULL;
	}


	edict_t *FindClientInPVS( edict_t *pEdict )
	{
		LOG_STUB();
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
		LOG_STUB();
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


	void EmitSound( edict_t *entity, int channel, const char *sample, float volume, float attenuation, int fFlags, int pitch )
	{
		LOG_STUB();
	}


	void EmitAmbientSound( edict_t *entity, float *pos, const char *samp, float vol, float attenuation, int iFlags, int pitch )
	{
		LOG_STUB();
	}


	void TraceLine( const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr )
	{
		LOG_STUB();
	}

	
	void TraceToss( edict_t *pEnt, edict_t *pentToIgnore, TraceResult *ptr )
	{
		LOG_STUB();
	}


	int TraceMonsterHull( edict_t *pEdict, const float *v1, const float *v2, int fNoMonsters, edict_t *pentToSkip, TraceResult *ptr )
	{
		LOG_STUB();
		return 0;
	}

	
	void TraceHull( const float *v1, const float *v2, int fNoMonsters, int hullNumber, edict_t *pentToSkip, TraceResult *ptr )
	{
		LOG_STUB();
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
		LOG_STUB();
	}


	void ServerExecute( void )
	{
		LOG_STUB();
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
		LOG_STUB();
	}


	int DecalIndex( const char *name )
	{
		LOG_STUB();
		return 0;
	}


	int PointContents( const float *rgflVector )
	{
		LOG_STUB();
		return 0;
	}


	void MessageBegin( int msg_dest, int msg_type, const float *pOrigin, edict_t *ed )
	{
		LOG_STUB();
	}


	void MessageEnd( void )
	{
		LOG_STUB();
	}


	void WriteByte( int iValue )
	{
		LOG_STUB();
	}


	void WriteChar( int iValue )
	{
		LOG_STUB();
	}


	void WriteShort( int iValue )
	{
		LOG_STUB();
	}


	void WriteLong( int iValue )
	{
		LOG_STUB();
	}


	void WriteAngle( float flValue )
	{
		LOG_STUB();
	}


	void WriteCoord( float flValue )
	{
		LOG_STUB();
	}


	void WriteString( const char *sz )
	{
		LOG_STUB();
	}


	void WriteEntity( int iValue )
	{
		LOG_STUB();
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
		LOG_STUB();
		return NULL;
	}


	int RegUserMsg( const char *pszName, int iSize )
	{
		LOG_STUB();
		return 0;
	}


	void AnimationAutomove( const edict_t *pEdict, float flTime )
	{
		LOG_STUB();
	}


	void GetBonePosition( const edict_t *pEdict, int iBone, float *rgflOrigin, float *rgflAngles )
	{
		LOG_STUB();
	}


	uint32 FunctionFromName( const char *pName )
	{
		LOG_STUB();
		return 0;
	}


	const char *NameForFunction( uint32 function )
	{
		LOG_STUB();
		return NULL;
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
		LOG_STUB();
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
		LOG_STUB();
		return lLow;
	}


	float RandomFloat( float flLow, float flHigh )
	{
		LOG_STUB();
		return flLow;
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
		LOG_STUB();
		return NULL;
	}


	void FreeFile( void *buffer )
	{
		LOG_STUB();
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
		LOG_STUB();
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
