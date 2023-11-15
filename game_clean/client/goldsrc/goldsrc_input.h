#ifndef GOLDSRC_INPUT_H
#define GOLDSRC_INPUT_H

#ifdef _WIN32
#pragma once
#endif

#include "goldsrc_playermove_shared.h"

#define NUM_STORED_CMDLISTS 32
#define MAX_CMDLIST_SIZE 16

/* 
 * NOTE: A major difference between GoldSrc and Source is that GoldSrc sends a
 * user message for every input frame on the client, while Source only does it
 * once per server tick. To emulate the old behavior, this class will send a
 * list of user commands which will be executed in sequence by the server.
 */

struct CUserCmdList
{
	GoldSRC::usercmd_t m_Cmds[MAX_CMDLIST_SIZE];
	int m_nNumCmds;
	int m_nSeqNum;
};


class CGoldSRCInput
{
public:
	CGoldSRCInput();
	~CGoldSRCInput();

	void Init();
	void ClearAllLists();

	void ActivateMouse();
	void DeactivateMouse();
	void AccumulateMouse();
	void ClearMouseState();

	void CreateMove( int sequence_number, float input_sample_frametime, bool active );
	void ExtraMouseSample( float frametime, bool active );
	void CreateMoveInternal( GoldSRC::usercmd_t *pCmd, float frametime, bool active );

	bool WriteUsercmdDeltaToBuffer( bf_write *buf, int from, int to, bool isnewcommand );

private:
	void MouseMove( float frametime, GoldSRC::usercmd_t *pCmd );

	CUserCmdList *GetCmdList( int seqNum );
	bool WriteUserCmdDelta( bf_write *buf, const GoldSRC::usercmd_t *pFrom, const GoldSRC::usercmd_t *pTo );

	CUserCmdList m_CmdLists[NUM_STORED_CMDLISTS];
	int m_nListsForSeqNums[NUM_STORED_CMDLISTS];
	int m_nListToWrite;

	bool m_bMouseActive;

	float m_flAccumulatedMouseX;
	float m_flAccumulatedMouseY;
	float m_flPrevMouseX;
	float m_flPrevMouseY;

	int m_nLastSampleFrame;
	float m_flTotalSampleTime;
};

extern CGoldSRCInput *g_pGoldSRCInput;

#endif // GOLDSRC_INPUT_H