#include "cbase.h"
#include "goldsrc_mapentities.h"
#include "goldsrc_baseentity.h"
#include "goldsrc_eiface.h"
#include "goldsrc_edict.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


extern IFileSystem *filesystem;


#define MAPKEY_MAXLENGTH 2048


static const char *ParseMapToken( const char *data, char *newToken )
{
	static bool s_BuildReverseMap = true;
	static bool s_BraceCharacters[256];
	static const char *s_BraceChars = "{}()\'";

	int             c;
	int             len;
		
	len = 0;
	newToken[0] = 0;
	
	if (!data)
		return NULL;

	// build the new table if we have to
	if ( s_BuildReverseMap )
	{
		s_BuildReverseMap = false; 

		Q_memset( s_BraceCharacters, 0, sizeof(s_BraceCharacters) );

		for ( const char *c = s_BraceChars; *c; c++ )
		{
			s_BraceCharacters[*c] = true;
		}
	}
		
	// skip whitespace
	skipwhite:
	while ( (c = *data) <= ' ')
	{
		if (c == 0)
			return NULL;                    // end of file;
		data++;
	}
	
	// skip // comments
	if (c=='/' && data[1] == '/')
	{
		while (*data && *data != '\n')
			data++;
		goto skipwhite;
	}
	

	// handle quoted strings specially
	if (c == '\"')
	{
		data++;
		while ( len < MAPKEY_MAXLENGTH )
		{
			c = *data++;
			if (c=='\"' || !c)
			{
				newToken[len] = 0;
				return data;
			}
			newToken[len] = c;
			len++;
		}

		if ( len >= MAPKEY_MAXLENGTH )
		{
			len--;
			newToken[len] = 0;
		}
	}

	// parse single characters
	if ( s_BraceCharacters[c]/*c=='{' || c=='}'|| c==')'|| c=='(' || c=='\''*/ )
	{
		newToken[len] = c;
		len++;
		newToken[len] = 0;
		return data+1;
	}

	// parse a regular word
	do
	{
		newToken[len] = c;
		data++;
		len++;
		c = *data;
		if ( s_BraceCharacters[c] /*c=='{' || c=='}'|| c==')'|| c=='(' || c=='\''*/ )
			break;

		if ( len >= MAPKEY_MAXLENGTH )
		{
			len--;
			newToken[len] = 0;
		}

	} while (c>32);
	
	newToken[len] = 0;
	return data;
}


static const char *SkipToNextEntity( const char *pMapData, char *szTokenBuffer )
{
	if ( !pMapData )
		return NULL;

	int openBraceCount = 1;
	while ( pMapData != NULL )
	{
		pMapData = ParseMapToken( pMapData, szTokenBuffer );

		if ( V_stricmp( szTokenBuffer, "{" ) == 0 )
		{
			openBraceCount++;
		}
		else if ( V_stricmp( szTokenBuffer, "}" ) == 0 )
		{
			if ( --openBraceCount == 0 )
			{
				return pMapData;
			}
		}
	}

	return NULL;
}


bool GetMapEntityValue( const char *pMapEntity, const char *szKey, char *pValue )
{
	char szToken[MAPKEY_MAXLENGTH];
	const char *pCursor = pMapEntity;

	while ( pCursor )
	{
		pCursor = ParseMapToken( pCursor, szToken );
		if ( szToken[0] == '}' )
			break;

		if ( !V_strcmp( szToken, szKey ) )
		{
			pCursor = ParseMapToken( pCursor, szToken );
			Q_strncpy( pValue, szToken, MAPKEY_MAXLENGTH );
			return true;
		}

		pCursor = ParseMapToken( pCursor, szToken );
	}

	return false;
}


void ParseEntityKeyValues( GoldSRC::edict_t *pEdict, const char *pMapData, char *szClassName )
{
	char szToken[MAPKEY_MAXLENGTH];
	const char *pCursor = pMapData;

	char szKey[MAPKEY_MAXLENGTH];
	char szValue[MAPKEY_MAXLENGTH];

	GoldSRC::KeyValueData kv;

	bool bIsMM = !Q_strncmp( szClassName, "multi_manager", MAPKEY_MAXLENGTH );

	while ( pCursor )
	{
		pCursor = ParseMapToken( pCursor, szToken );
		if ( szToken[0] == '}' )
			break;

		V_strncpy( szKey, szToken, MAPKEY_MAXLENGTH );

		pCursor = ParseMapToken( pCursor, szToken );
		V_strncpy( szValue, szToken, MAPKEY_MAXLENGTH );

		if ( bIsMM && !Q_strncmp( szKey, "OnTrigger", MAPKEY_MAXLENGTH ) )
		{
			int firstComma = MAPKEY_MAXLENGTH;

			// Copy over the key until a comma is hit
			for ( int i = 0; i < MAPKEY_MAXLENGTH - 1; i++ )
			{
				if ( !szValue[i] || szValue[i] == ',' )
				{
					szKey[i] = '\0';
					firstComma = i;
					break;
				}

				szKey[i] = szValue[i];
			}

			// Find the second comma
			int secondComma = MAPKEY_MAXLENGTH;
			for ( int i = firstComma + 1; i < MAPKEY_MAXLENGTH - 1; i++ )
			{
				if ( !szValue[i] || szValue[i] == ',' )
				{
					secondComma = i;
					break;
				}
			}

			// Find the third comma
			int thirdComma = MAPKEY_MAXLENGTH;
			for ( int i = secondComma + 1; i < MAPKEY_MAXLENGTH - 1; i++ )
			{
				if ( !szValue[i] || szValue[i] == ',' )
				{
					thirdComma = i;
					break;
				}
			}

			// Copy over the delay
			int j = 0;
			for ( int i = thirdComma + 1; i < MAPKEY_MAXLENGTH - 1; i++ )
			{
				if ( !szValue[i] || szValue[i] == ',' )
				{
					szValue[j] = '\0';
					break;
				}

				szValue[j] = szValue[i];
				j++;
			}
		}

		kv.szClassName = szClassName;
		kv.szKeyName = szKey;
		kv.szValue = szValue;
		kv.fHandled = FALSE;

		g_pGoldSRCEntityInterface->pfnKeyValue( pEdict, &kv );

		if ( !kv.fHandled )
		{
			DevMsg( "Unknown key in %s : %s\n", szClassName, szKey );
		}
	}

	return;
}


const char *ParseMapEntity( const char *pMapEntity, char *szTokenBuffer )
{
	char szClassName[MAPKEY_MAXLENGTH];

	if ( GetMapEntityValue( pMapEntity, "classname", szClassName ) )
	{
		int forceEdict = -1;

		if ( !Q_strncmp( szClassName, "worldspawn", MAPKEY_MAXLENGTH ) )
		{
			forceEdict = 0;
		}

		CBaseEntity *pEntity = g_pGoldSRCEdict->CreateEntityByName( GoldSRC::AllocString( szClassName ), forceEdict );
		if ( !pEntity )
			return pMapEntity;

		GoldSRC::edict_t *pEdict = pEntity->GoldSrcEdict();
		if ( !pEdict )
			return pMapEntity;

		ParseEntityKeyValues( pEdict, pMapEntity, szClassName );
		pEntity->Spawn();
	}

	return pMapEntity;
}


struct GoldSRCLump
{
	int offset, length;
};


struct GoldSRCHeader
{
	int version;
	GoldSRCLump lumps[15];
};


char *TryLoadGoldSRCEntityLump( const char *pMapName )
{
	char szFileName[MAX_PATH];
	Q_snprintf( szFileName, MAX_PATH, "maps/%s.bsp_g", pMapName );

	FileHandle_t hFile = filesystem->Open( szFileName, "rb", "GAME" );
	if ( !hFile )
		return NULL;

	GoldSRCHeader header;
	filesystem->Read( &header, sizeof( header ), hFile );

	if ( header.version != 30 )
	{
		filesystem->Close( hFile );
		return NULL;
	}

	int entitesOffset = header.lumps[0].offset;
	int entitiesSize = header.lumps[0].length;

	char *entitiesLump = (char*)malloc( entitiesSize );

	filesystem->Seek( hFile, entitesOffset, FILESYSTEM_SEEK_HEAD );
	filesystem->Read( entitiesLump, entitiesSize, hFile );

	filesystem->Close( hFile );
	return entitiesLump;
}


void ParseMapEntities( const char *pMapName, const char *pMapEntities )
{
	char szTokenBuffer[MAPKEY_MAXLENGTH];

	// Try load the goldsrc BSP
	//char *pGoldSRCEntities = TryLoadGoldSRCEntityLump( pMapName );

	// If it succeeded, just load those entities instead
	//if ( pGoldSRCEntities )
	//	pMapEntities = pGoldSRCEntities;

	for ( ; pMapEntities != NULL; pMapEntities = SkipToNextEntity( pMapEntities, szTokenBuffer ) )
	{
		char token[MAPKEY_MAXLENGTH];
		pMapEntities = ParseMapToken( pMapEntities, token );

		if ( !pMapEntities )
			break;

		if ( token[0] != '{' )
		{
			continue;
		}

		// Parse entity
		ParseMapEntity( pMapEntities, szTokenBuffer );
	}

	// free the entity lump
	//if ( pGoldSRCEntities )
	//	free( pGoldSRCEntities );
}
