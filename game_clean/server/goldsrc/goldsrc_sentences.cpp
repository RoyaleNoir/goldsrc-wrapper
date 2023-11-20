#include "cbase.h"
#include "goldsrc_sentences.h"
#include "filesystem.h"
#include "eiface.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

extern IFileSystem *filesystem;
extern IVEngineServer *engine;

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
	/*
	// TODO: Make this do more than just map ids

	FileHandle_t hFile = filesystem->Open( szPath, "r", "GAME" );
	if ( !hFile )
		return;

	int numsentences = 0;

	char buffer[512] = { 0 };
	char sentenceName[256] = { 0 };

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
		int j = i;
		while ( buffer[j] && buffer[j] != ' ' )
			j++;

		if ( !buffer[j] )
			continue;

		buffer[j] = '\0';

		Q_strncpy( sentenceName, &buffer[i], 256 );

		int sentenceID = numsentences++;
		int realSentenceID = engine->SentenceIndexFromName( sentenceName );

		m_realindex[sentenceID] = realSentenceID;

		if ( numsentences >= 1536 )
			break;
	}

	filesystem->Close( hFile );
	*/
}


void CSentencesManager::UnloadSentences()
{
}
