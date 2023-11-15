#include "cbase.h"
#include "goldsrc_sentences.h"
#include "filesystem.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IFileSystem *filesystem;


static CSentencesManager s_SentencesManager;

CSentencesManager *SentencesManager()
{
	return &s_SentencesManager;
}


CSentencesManager::CSentencesManager()
{
}


CSentencesManager::~CSentencesManager()
{
}


void CSentencesManager::LoadSentences( const char *szPath )
{
	FileHandle_t hFile = filesystem->Open( szPath, "r", "GAME" );
	if ( !hFile )
		return;

	char buffer[512] = { 0 };
	while( filesystem->ReadLine( buffer, 511, hFile ) != NULL )
	{
		int i = 0;

		// Skip whitespace
		while( buffer[i] && buffer[i] == ' ' )
			i++;

		// May have been a blank line
		if ( !buffer[i] )
			continue;

		// Skip comments etc
		if ( buffer[i] == '/' || !isalpha( buffer[i] ) )
			continue;

		// Read in sentence name
	}

	filesystem->Close( hFile );
}


void CSentencesManager::UnloadSentences()
{
}
