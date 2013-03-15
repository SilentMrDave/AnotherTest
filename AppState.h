#ifndef __AppSTATE_H__
#define __AppSTATE_H__
 
#include "Event.h"
 
class AppState : public Event
{
    public:
        AppState();
 
    public:
        virtual void OnActivate() = 0;
 
        virtual void OnDeactivate() = 0;
 
        virtual void OnLoop() = 0;
 
        virtual void OnRender(SDL_Surface* Surf_Display) = 0;
};
 
#endif