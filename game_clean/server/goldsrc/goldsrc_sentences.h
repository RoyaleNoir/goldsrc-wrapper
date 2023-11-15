#ifndef GOLDSRC_SENTENCES_H
#define GOLDSRC_SENTENCES_H

#ifdef _WIN32
#pragma once
#endif

class CSentencesManager
{
public:
	CSentencesManager();
	~CSentencesManager();

	void LoadSentences( const char *szPath );
	void UnloadSentences();
};

extern CSentencesManager *SentencesManager();

#endif // GOLDSRC_SENTENCES_H