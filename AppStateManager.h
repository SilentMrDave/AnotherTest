#ifndef __AppStateManager_H__
#define __AppStateManager_H__
 
#include "AppState.h"
 
enum
{
    // Add your Other App States Here
    APPSTATE_NONE,
    APPSTATE_INTRO,
    APPSTATE_GAME
};
 
class AppStateManager
{
    private:
        static AppState* ActiveAppState;
 
    public:
        static void OnEvent(SDL_Event* Event);
 
        static void OnLoop();
 
        static void OnRender(SDL_Surface* Surf_Display);
 
    public:
        static void SetActiveAppState(int AppStateID);
 
        static AppState* GetActiveAppState();
};
 
#endif