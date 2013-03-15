#ifndef __AppStateGame_H__
#define __AppStateGame_H__

static float gravity;

#include "AppState.h"
 
#include "Area.h"
#include "Camera.h"
#include "Entity.h"
#include "Surface.h"
 
#include "Player.h"
 
class AppStateGame : public AppState
{
    private:
        static AppStateGame Instance;
 

		Entity		Box;

		Player*		Players;
		int			PlayerAmount;
		int			CurrentPlayer;
		//Will become Entity specific variable
		bool		follow;
		
 
    private:
        AppStateGame();
 
    public:
        void OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode);
 
        void OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode);
 
    public:
        void OnActivate();
 
        void OnDeactivate();
 
        void OnLoop();
 
        void OnRender(SDL_Surface* Surf_Display);
 
    public:
        static AppStateGame* GetInstance();
};
 
#endif