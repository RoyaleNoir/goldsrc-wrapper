#include "fakesdl2.h"

extern "C"
{
    DECLSPEC Uint32 SDLCALL SDL_GetRelativeMouseState( int *x, int *y )
    {

    }


	DECLSPEC int SDLCALL SDL_NumJoysticks( void )
    {
        return 0;
    }

	DECLSPEC void SDLCALL SDL_JoystickUpdate( void )
    {

    }


	DECLSPEC SDL_bool SDLCALL SDL_IsGameController( int joystick_index )
    {
        return FALSE;
    }

	DECLSPEC SDL_GameController *SDLCALL SDL_GameControllerOpen( int joystick_index )
    {
        return NULL;
    }

	DECLSPEC const char *SDLCALL SDL_GameControllerName( SDL_GameController *gamecontroller )
    {
        return NULL;
    }

	DECLSPEC Sint16 SDLCALL SDL_GameControllerGetAxis( SDL_GameController *gamecontroller, SDL_GameControllerAxis axis )
    {
        return 0;
    }

	DECLSPEC Uint8 SDLCALL SDL_GameControllerGetButton( SDL_GameController *gamecontroller, SDL_GameControllerButton button )
    {
        return 0;
    }

	DECLSPEC void SDLCALL SDL_GameControllerClose( SDL_GameController *gamecontroller )
    {

    }
}
