#include "cbase.h"
#include "base_gameclientexports.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


bool CBaseGameClientExports::IsPlayerGameVoiceMuted( int playerIndex )
{
	return false;
}


void CBaseGameClientExports::MutePlayerGameVoice( int playerIndex )
{
}


void CBaseGameClientExports::UnmutePlayerGameVoice( int playerIndex )
{
}


void CBaseGameClientExports::OnGameUIActivated()
{
}


void CBaseGameClientExports::OnGameUIHidden()
{
}


void CBaseGameClientExports::CreateAchievementsPanel( vgui::Panel *pParent )
{
}


void CBaseGameClientExports::DisplayAchievementPanel()
{
}


void CBaseGameClientExports::ShutdownAchievementPanel()
{
}


int CBaseGameClientExports::GetAchievementsPanelMinWidth( void ) const
{
	return 0;
}


const char *CBaseGameClientExports::GetHolidayString()
{
	return nullptr;
}
