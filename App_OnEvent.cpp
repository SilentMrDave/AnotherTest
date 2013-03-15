#include "App.h"
 
void App::OnEvent(SDL_Event* Event)
{
    Event::OnEvent(Event);
 
    AppStateManager::OnEvent(Event);
}
 
void App::OnExit()
{
    Running = false;
}