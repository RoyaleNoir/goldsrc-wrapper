#ifndef GOLDSRC_USERMESSAGES_SHARED_H
#define GOLDSRC_USERMESSAGES_SHARED_H

#ifdef _WIN32
#pragma once
#endif

#ifdef CLIENT_DLL
typedef int ( *pfnUserMsgHook )( const char *pszName, int iSize, void *pbuf );
#include "utlhashtable.h"
#else
#include "goldsrc_edict.h"
#include "utlmap.h"
#endif


class CGoldSRCUserMessages
{
public:
	CGoldSRCUserMessages();
	~CGoldSRCUserMessages();

	void Init();
	void Shutdown();

#ifdef CLIENT_DLL
	bool InstallStringTableCallback( const char *szTableName );
	bool Dispatch( int type, bf_read &buf );

	int HookUserMsg( const char *szName, pfnUserMsgHook hook );
#else
	void CreateStringTable();

	int Register( const char *pszName, int size );
	bool GetMessageInfo( int type, char *szName, int nameLength, int &size );

	void MessageBegin( int dest, int type, const float *pOrigin, GoldSRC::edict_t *pEnt );
	void MessageEnd();

	void WriteByte( int iValue );
	void WriteChar( int iValue );
	void WriteShort( int iValue );
	void WriteLong( int iValue );
	void WriteAngle( float flValue );
	void WriteCoord( float flValue );
	void WriteString( const char* sz );
	void WriteEntity( int iValue );
#endif

	enum StandardMsgs
	{
		SVC_TEMPENTITY = 23,
		SVC_INTERMISSION = 30,
		SVC_CDTRACK = 32,
		SVC_WEAPONANIM = 35,
		SVC_ROOMTYPE = 37,
		SVC_DIRECTOR = 51,
		FIRST_MOD_USERMSG = 64,
	};

private:
	struct MsgDesc
	{
		const char *name;
		int size;
	};

#ifdef CLIENT_DLL
	CUtlHashtable<const char*, pfnUserMsgHook> m_Hooks;
#else
	CUtlMap<int, MsgDesc> m_RegisteredMessages;
	bf_write *m_buf;
#endif
};

extern CGoldSRCUserMessages *UserMessages();

#endif // GOLDSRC_USERMESSAGES_SHARED_H