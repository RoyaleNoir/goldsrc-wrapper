#ifndef BASE_GAMECLIENTEXPORTS_H
#define BASE_GAMECLIENTEXPORTS_H

#ifdef _WIN32
#pragma once
#endif

#include <vgui/IPanel.h>
#include "game/client/IGameClientExports.h"

class CBaseGameClientExports : public IGameClientExports
{
public:
#ifndef _XBOX
	virtual bool	IsPlayerGameVoiceMuted( int playerIndex );
	virtual void	MutePlayerGameVoice( int playerIndex );
	virtual void	UnmutePlayerGameVoice( int playerIndex );

	virtual void	OnGameUIActivated();
	virtual void	OnGameUIHidden();
#endif

	virtual void	CreateAchievementsPanel( vgui::Panel *pParent );
	virtual void	DisplayAchievementPanel();
	virtual void	ShutdownAchievementPanel();
	virtual int		GetAchievementsPanelMinWidth( void ) const;

	virtual const char *GetHolidayString();
};

extern CBaseGameClientExports *g_pGameClientExports;

#endif // BASE_CLIENTDLL_H