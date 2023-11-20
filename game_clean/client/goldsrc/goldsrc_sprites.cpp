#include "cbase.h"
#include "goldsrc_sprites.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IFileSystem* filesystem;


static CSpriteManager s_SpriteManager;
CSpriteManager* SpriteManager()
{
	return &s_SpriteManager;
}


CSpriteManager::CSpriteManager()
{
}

CSpriteManager::~CSpriteManager()
{
	m_SpriteLists.PurgeAndDeleteElements();
}


static const char *ReadToken( const char *szInput, char *szToken, int maxLength )
{
	if ( !szInput || !szToken || maxLength == 0 )
		return NULL;

	const char *p = szInput;
	
	// Skip the spaces
	for ( ; *p && isspace( *p ); p++ )
	{
	}

	if ( !*p )
		return NULL;
	
	int i = 0;
	for ( ; *p && !isspace( *p ); p++ )
	{
		szToken[i++] = *p;
		if ( i >= maxLength - 1 )
		{
			break;
		}
	}

	if ( p == szInput )
		return NULL;

	szToken[i] = '\0';
	return p;
}


GoldSRC::client_sprite_t *CSpriteManager::GetList( const char *szName, int *pCount )
{
	FileHandle_t hFile = filesystem->Open( szName, "r" );
	if ( !hFile )
	{
		*pCount = 0;
		return NULL;
	}

	GoldSRC::client_sprite_t *pList = NULL;

	char szLine[256];
	char szToken[64];
	int i = 0;
	while ( filesystem->ReadLine( szLine, 256, hFile ) != NULL )
	{
		const char *pCursor = szLine;

		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
			continue;

		// Skip comment
		if ( !szToken[0] || szToken[0] == '/' )
			continue;

		// First line case
		if ( !pList )
		{
			*pCount = atoi( szToken );
			if ( *pCount == 0 )
			{
				Warning( "Couldn't read sprite count for %s\n", szName );
				Assert( 0 );
				filesystem->Close( hFile );
				return false;
			}

			pList = new GoldSRC::client_sprite_t[*pCount];
			continue;
		}

		GoldSRC::client_sprite_t *pSprite = &pList[i];

		// Copy over name
		Q_strncpy( pSprite->szName, szToken, 64 );

		// Read resolution
		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
		{
			Warning( "Couldn't read resolution for sprite %s\n", pSprite->szName );
			delete[] pList;
			filesystem->Close( hFile );
			return NULL;
		}

		pSprite->iRes = atoi( szToken );

		// Read texture
		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
		{
			Warning( "Couldn't read texture name for sprite %s\n", pSprite->szName );
			delete[] pList;
			filesystem->Close( hFile );
			return NULL;
		}
		Q_strncpy( pSprite->szSprite, szToken, 64 );

		int x, y, w, h;

		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
		{
			Warning( "Couldn't read x for sprite %s\n", pSprite->szName );
			delete[] pList;
			filesystem->Close( hFile );
			return NULL;
		}

		x = atoi( szToken );

		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
		{
			Warning( "Couldn't read y for sprite %s\n", pSprite->szName );
			delete[] pList;
			filesystem->Close( hFile );
			return NULL;
		}

		y = atoi( szToken );

		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
		{
			Warning( "Couldn't read w for sprite %s\n", pSprite->szName );
			delete[] pList;
			filesystem->Close( hFile );
			return NULL;
		}

		w = atoi( szToken );

		pCursor = ReadToken( pCursor, szToken, 64 );
		if ( !pCursor )
		{
			Warning( "Couldn't read h for sprite %s\n", pSprite->szName );
			delete[] pList;
			filesystem->Close( hFile );
			return NULL;
		}

		h = atoi( szToken );
		

		pSprite->rc.left = x;
		pSprite->rc.top = y;
		pSprite->rc.right = x + w;
		pSprite->rc.bottom = y + h;
	}

	m_SpriteLists.AddToTail( pList );
	filesystem->Close( hFile );
	return pList;
}
