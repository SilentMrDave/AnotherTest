#include "AppStateManager.h"
 
// Refer to your Other App States Here
#include "AppStateIntro.h"
#include "AppStateGame.h"
 
AppState* AppStateManager::ActiveAppState = 0;
 
void AppStateManager::OnEvent(SDL_Event* EventHolder)
{
    if(ActiveAppState) ActiveAppState->OnEvent(EventHolder);
}
 
void AppStateManager::OnLoop()
{
    if(ActiveAppState) ActiveAppState->OnLoop();
}
 
void AppStateManager::OnRender(SDL_Surface* Surf_Display)
{
    if(ActiveAppState) ActiveAppState->OnRender(Surf_Display);
}
 
void AppStateManager::SetActiveAppState(int AppStateID)
{
    if(ActiveAppState) ActiveAppState->OnDeactivate();
 
    // Also, add your App State Here so that the Manager can switch to it
    if(AppStateID == APPSTATE_NONE)        ActiveAppState = 0;
    if(AppStateID == APPSTATE_INTRO)    ActiveAppState = AppStateIntro::GetInstance();
    if(AppStateID == APPSTATE_GAME)        ActiveAppState = AppStateGame::GetInstance();
 
    if(ActiveAppState) ActiveAppState->OnActivate();
}
 
AppState* AppStateManager::GetActiveAppState()
{
    return ActiveAppState;
}