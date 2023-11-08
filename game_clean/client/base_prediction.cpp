#include "cbase.h"
#include "base_prediction.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


void CBasePrediction::Init( void )
{
}


void CBasePrediction::Shutdown( void )
{
}


void CBasePrediction::Update( int startframe, bool validframe, int incoming_acknowledged, int outgoing_command )
{
}


void CBasePrediction::PreEntityPacketReceived( int commands_acknowledged, int current_world_update_packet )
{
}


void CBasePrediction::PostEntityPacketReceived( void )
{
}


void CBasePrediction::PostNetworkDataReceived( int commands_acknowledged )
{
}


void CBasePrediction::OnReceivedUncompressedPacket( void )
{
}


void CBasePrediction::GetViewOrigin( Vector &org )
{
}


void CBasePrediction::SetViewOrigin( Vector &org )
{
}


void CBasePrediction::GetViewAngles( QAngle &ang )
{
}


void CBasePrediction::SetViewAngles( QAngle &ang )
{
}


void CBasePrediction::GetLocalViewAngles( QAngle &ang )
{
}


void CBasePrediction::SetLocalViewAngles( QAngle &ang )
{
}
