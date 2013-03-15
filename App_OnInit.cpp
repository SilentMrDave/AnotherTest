#include "App.h"
 
bool App::OnInit()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
        return false;
    }
 
    if((Surf_Display = SDL_SetVideoMode(WWIDTH, WHEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
	{
        return false;
    }
 
    SDL_EnableKeyRepeat(1, SDL_DEFAULT_REPEAT_INTERVAL / 3);
 
    AppStateManager::SetActiveAppState(APPSTATE_INTRO);
 
    return true;
}