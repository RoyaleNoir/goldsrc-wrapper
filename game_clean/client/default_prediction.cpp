#include "cbase.h"
#include "base_prediction.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"



static CBasePrediction g_Prediction;
CBasePrediction *g_pPrediction = &g_Prediction;

EXPOSE_SINGLE_INTERFACE_GLOBALVAR( CBasePrediction, IPrediction, VCLIENT_PREDICTION_INTERFACE_VERSION, g_Prediction );
