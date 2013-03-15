#ifndef __AppStateIntro_H__
#define __AppStateIntro_H__
 
#include "AppState.h"
#include "Surface.h"

class AppStateIntro : public AppState
{
    private:
        static AppStateIntro Instance;
 
        SDL_Surface* Surf_Logo;
 
        int StartTime;
 
    private:
        AppStateIntro();
 
    public:
        void OnActivate();
 
        void OnDeactivate();
 
        void OnLoop();
 
        void OnRender(SDL_Surface* Surf_Display);
 
    public:
        static AppStateIntro* GetInstance();
};
 
#endif