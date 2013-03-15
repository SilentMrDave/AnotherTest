#include "App.h"
 
void App::OnCleanup()
{
    AppStateManager::SetActiveAppState(APPSTATE_NONE);
 
    SDL_FreeSurface(Surf_Display);
    SDL_Quit();
}