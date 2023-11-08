#ifndef BASE_PREDICTION_H
#define BASE_PREDICTION_H

#ifdef _WIN32
#pragma once
#endif

#include "iprediction.h"

class CBasePrediction : public IPrediction
{
public:
	virtual void	Init( void );
	virtual void	Shutdown( void );

	virtual void	Update( int startframe, bool validframe, int incoming_acknowledged, int outgoing_command );

	virtual void	PreEntityPacketReceived( int commands_acknowledged, int current_world_update_packet );
	virtual void	PostEntityPacketReceived( void );
	virtual void	PostNetworkDataReceived( int commands_acknowledged );

	virtual void	OnReceivedUncompressedPacket( void );

	virtual void	GetViewOrigin( Vector &org );
	virtual void	SetViewOrigin( Vector &org );
	virtual void	GetViewAngles( QAngle &ang );
	virtual void	SetViewAngles( QAngle &ang );
	virtual void	GetLocalViewAngles( QAngle &ang );
	virtual void	SetLocalViewAngles( QAngle &ang );
};


extern CBasePrediction *g_pPrediction;

#endif // BASE_PREDICTION_H