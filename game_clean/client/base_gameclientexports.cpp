#include "cbase.h"
#include "base_gameclientexports.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


bool CBaseGameClientExports::IsPlayerGameVoiceMuted( int playerIndex )
{
	LOG_STUB();
	return false;
}


void CBaseGameClientExports::MutePlayerGameVoice( int playerIndex )
{
	LOG_STUB();
}


void CBaseGameClientExports::UnmutePlayerGameVoice( int playerIndex )
{
	LOG_STUB();
}


void CBaseGameClientExports::OnGameUIActivated()
{
	LOG_STUB();
}


void CBaseGameClientExports::OnGameUIHidden()
{
	LOG_STUB();
}


void CBaseGameClientExports::CreateAchievementsPanel( vgui::Panel *pParent )
{
	LOG_STUB();
}


void CBaseGameClientExports::DisplayAchievementPanel()
{
	LOG_STUB();
}


void CBaseGameClientExports::ShutdownAchievementPanel()
{
	LOG_STUB();
}


int CBaseGameClientExports::GetAchievementsPanelMinWidth( void ) const
{
	LOG_STUB();

	return 0;
}


const char *CBaseGameClientExports::GetHolidayString()
{
	LOG_STUB();

	return nullptr;
}
