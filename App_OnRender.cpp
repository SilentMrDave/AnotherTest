#include "App.h"
 
void App::OnRender()
{
    AppStateManager::OnRender(Surf_Display);
 
    SDL_Flip(Surf_Display);
}