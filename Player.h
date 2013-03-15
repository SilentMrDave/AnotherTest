#ifndef _Player_H_
#define _Player_H_
 
#include "Entity.h"
 
class Player : public Entity
{
    public:
        Player();
 
        bool OnLoad(char* File, int Width, int Height, int MaxFrames);
 
        void OnLoop();
 
        void OnRender(SDL_Surface* Surf_Display);
 
        void OnCleanup();
 
        void OnAnimate();
 
        bool OnCollision(Entity* Entity);

		bool Collides(int oX, int oY, int oW, int oH);
};
 
#endif