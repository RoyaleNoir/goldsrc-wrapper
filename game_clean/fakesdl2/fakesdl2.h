#ifndef FAKESDL2_H
#define FAKESDL2_H

#ifdef _WIN32
#pragma once
#endif

extern "C"
{
	#ifndef DECLSPEC
	#if defined(__WIN32__)
	#define DECLSPEC	__declspec(dllexport)
	#else
	#define DECLSPEC
	#endif
	#endif


	#ifndef SDLCALL
	#if defined(__WIN32__) && !defined(__GNUC__)
	#define SDLCALL __cdecl
	#else
	#define SDLCALL
	#endif
	#endif 

	struct SDL_GameController;

	extern DECLSPEC Uint32 SDLCALL SDL_GetRelativeMouseState( int *x, int *y );

	extern DECLSPEC int SDLCALL SDL_NumJoysticks( void );
	extern DECLSPEC void SDLCALL SDL_JoystickUpdate( void );

	extern DECLSPEC SDL_bool SDLCALL SDL_IsGameController( int joystick_index );
	extern DECLSPEC SDL_GameController *SDLCALL SDL_GameControllerOpen( int joystick_index );
	extern DECLSPEC const char *SDLCALL SDL_GameControllerName( SDL_GameController *gamecontroller );
	extern DECLSPEC Sint16 SDLCALL SDL_GameControllerGetAxis( SDL_GameController *gamecontroller, SDL_GameControllerAxis axis );
	extern DECLSPEC Uint8 SDLCALL SDL_GameControllerGetButton( SDL_GameController *gamecontroller, SDL_GameControllerButton button );
	extern DECLSPEC void SDLCALL SDL_GameControllerClose( SDL_GameController *gamecontroller );
}

#endif // FAKESDL2_H