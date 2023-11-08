#include "cbase.h"
#include "base_prediction.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"


void CBasePrediction::Init( void )
{
	LOG_STUB();
}


void CBasePrediction::Shutdown( void )
{
	LOG_STUB();
}


void CBasePrediction::Update( int startframe, bool validframe, int incoming_acknowledged, int outgoing_command )
{
	LOG_STUB();
}


void CBasePrediction::PreEntityPacketReceived( int commands_acknowledged, int current_world_update_packet )
{
	LOG_STUB();
}


void CBasePrediction::PostEntityPacketReceived( void )
{
	LOG_STUB();
}


void CBasePrediction::PostNetworkDataReceived( int commands_acknowledged )
{
	LOG_STUB();
}


void CBasePrediction::OnReceivedUncompressedPacket( void )
{
	LOG_STUB();
}


void CBasePrediction::GetViewOrigin( Vector &org )
{
	LOG_STUB();
}


void CBasePrediction::SetViewOrigin( Vector &org )
{
	LOG_STUB();
}


void CBasePrediction::GetViewAngles( QAngle &ang )
{
	LOG_STUB();
}


void CBasePrediction::SetViewAngles( QAngle &ang )
{
	LOG_STUB();
}


void CBasePrediction::GetLocalViewAngles( QAngle &ang )
{
	LOG_STUB();
}


void CBasePrediction::SetLocalViewAngles( QAngle &ang )
{
	//LOG_STUB();
}
