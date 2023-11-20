//========= Copyright © 1996-2002, Valve LLC, All rights reserved. ============
//
// Purpose: 
//
// $NoKeywords: $
//=============================================================================

#pragma once

#ifdef _WIN32
#define VGUIAPI __declspec( dllexport )
#else
#define VGUIAPI __attribute__ ((visibility("default")))
#endif
