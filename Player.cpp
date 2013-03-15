#include "Player.h"
 
Player::Player()
{
	Type = ENTITY_TYPE_PLAYER;
}
 
bool Player::OnLoad(char* File, int Width, int Height, int MaxFrames)
{
    if(Entity::OnLoad(File, Width, Height, MaxFrames) == false)
	{
        return false;
    }
 
    return true;
}
 
void Player::OnLoop()
{
    Entity::OnLoop();
}
 
void Player::OnRender(SDL_Surface* Surf_Display)
{
    Entity::OnRender(Surf_Display);
}
 
void Player::OnCleanup()
{
    Entity::OnCleanup();
}
 
void Player::OnAnimate()
{
    if(SpeedX != 0)
	{
        Anim_Control.MaxFrames = 8;
    }
	else
	{
        Anim_Control.MaxFrames = 0;
    }
 
    Entity::OnAnimate();
}
 
bool Player::OnCollision(Entity* Entity)
{
    //Jump();
	
    return true;
}

bool Player::Collides(int oX, int oY, int oW, int oH)
{
	 int left1, left2;
    int right1, right2;
    int top1, top2;
    int bottom1, bottom2;
 
    int tX = (int)X + Col_X;
    int tY = (int)Y + Col_Y;
 
    left1 = tX;
    left2 = oX;
 
    right1 = left1 + Width - 1 - Col_Width;
    right2 = oX + oW - 1;
 
    top1 = tY;
    top2 = oY;
 
    bottom1 = top1 + Height - 1 - Col_Height;
    bottom2 = oY + oH - 1;
 
 
    if (bottom1 < top2) return false;
    if (top1 > bottom2) return false;
 
    if (right1 < left2) return false;
    if (left1 > right2) return false;
 
	/*if (Entity->Type == ENTITY_TYPE_PLAYER)
	{
		return false;
	}*/
    return false;
}