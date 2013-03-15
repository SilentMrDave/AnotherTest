#ifndef _App_H_
#define _App_H_
 
#include <SDL/SDL.h>
 
#include "Define.h"
 
#include "AppStateManager.h"
#include "FPS.h"
 
#include "Event.h"
#include "Surface.h"
 
class App : public Event
{
    private:
        bool            Running;
 
        SDL_Surface*    Surf_Display;
 
    public:
        App();
 
        int OnExecute();
 
    public:
        bool OnInit();
 
        void OnEvent(SDL_Event* Event);
 
            void OnExit();
 
        void OnLoop();
 
        void OnRender();
 
        void OnCleanup();
};
 
#endif