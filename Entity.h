#ifndef _Entity_H_
#define _Entity_H_
 
#include <vector>
 
#include "Area.h"
#include "Animation.h"
#include "Camera.h"
#include "FPS.h"
#include "Surface.h"
#include <cmath>
 
enum
{
    ENTITY_TYPE_GENERIC = 0,
 
    ENTITY_TYPE_PLAYER
};
 
enum
{
    ENTITY_FLAG_NONE     = 0,
 
    ENTITY_FLAG_GRAVITY    = 0x00000001,
    ENTITY_FLAG_GHOST    = 0x00000002,
    ENTITY_FLAG_MAPONLY    = 0x00000004
};
 
class Entity
{
    public:
        static std::vector<Entity*>    EntityList;
 
    protected:
        Animation      Anim_Control;
 
        SDL_Surface*    Surf_Entity;
 
    public:
        float           X;
        float           Y;
 
        int             Width;
        int             Height;
 
        bool        MoveLeft;
        bool        MoveRight;

		float		gravity;
 
    public:
        int			Type;
 
        bool        Dead;
        int			Flags;
		int			tempFlags;
		bool		emptyHands;
 
    protected:
        float       SpeedX;
        float       SpeedY;
 
        float       AccelX;
        float       AccelY;
 
    public:
        float       MaxSpeedX;
        float       MaxSpeedY;
 
    protected:
        int          CurrentFrameCol;
        int          CurrentFrameRow;
 
    protected:
        int        Col_X;
        int        Col_Y;
        int        Col_Width;
        int        Col_Height;
 
    public:
        Entity();
 
        virtual ~Entity();
 
    public:
        virtual bool OnLoad(char* File, int Width, int Height, int MaxFrames);

        virtual void OnLoop();
 
        virtual void OnRender(SDL_Surface* Surf_Display);
 
        virtual void OnCleanup();
 
        virtual void OnAnimate();
 
        virtual bool OnCollision(Entity* Entity);
 
    public:
        void    OnMove(float MoveX, float MoveY);
		// Move Entity MoveX/MoveY amount of pixels per frame
        void    StopMove();
		// Decreases acceleration of an Entity until it stops
		bool    Jump();
		
		bool	Carry(Entity* Entity);

		bool	Follow(Entity* Entity);

	protected:
		bool    CanJump;

		bool	isCarrying;
		
 
    public:
        bool    Collides(int oX, int oY, int oW, int oH);
 
    private:
        bool     PosValid(int NewX, int NewY);
 
        bool     PosValidTile(Tile* Tile);
 
        bool     PosValidEntity(Entity* Entity, int NewX, int NewY);

	public:
		static bool CheckCollision(Entity* entityA, Entity* entityB);
		static bool GetAlphaXY(Entity* entity, int x, int y);
		static SDL_Rect Intersection(const SDL_Rect& boundsA, const SDL_Rect& boundsB);
		SDL_Rect NormalizeBounds(const SDL_Rect& rect);
		SDL_Rect GetFrameBounds();
		SDL_Rect GetBounds();
};
 
class EntityCol
{
    public:
        static std::vector<EntityCol>    EntityColList;
 
    public:
        Entity* EntityA;
        Entity* EntityB;
 
    public:
        EntityCol();
};
 
#endif