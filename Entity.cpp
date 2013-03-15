#include "Entity.h"
#define Maximum(a, b) ((a > b) ? a : b)
#define Minimum(a, b) ((a < b) ? a : b) 

std::vector<Entity*>   Entity::EntityList;

Entity::Entity()
{
    Surf_Entity = NULL;
 
    X = 0;
    Y = 0;
 
    Width     = 0;
    Height     = 0;
 
    MoveLeft  = false;
    MoveRight = false;
	CanJump = false;
	isCarrying = false;
 
    Type =     ENTITY_TYPE_GENERIC;
 
    Dead = false;
    Flags = ENTITY_FLAG_GRAVITY;
 
    SpeedX = 0;
    SpeedY = 0;
 
    AccelX = 0;
    AccelY = 0;
 
    MaxSpeedX = 5;
    MaxSpeedY = 10;
 
    CurrentFrameCol = 0;
    CurrentFrameRow = 0;
 
    Col_X = 0;
    Col_Y = 0;
 
    Col_Width  = 0;
    Col_Height = 0;
}

Entity::~Entity()
{

}
 
bool Entity::OnLoad(char* File, int Width, int Height, int MaxFrames)
{
    if((Surf_Entity = Surface::OnLoad(File)) == NULL)
	{
        return false;
    }
 
    Surface::Transparent(Surf_Entity, 255, 0, 255);
 
    this->Width = Width;
    this->Height = Height;
 
    Anim_Control.MaxFrames = MaxFrames;
 
    return true;
}

bool Entity::OnLoad(char* File, int Width, int Height)
{
    if((Surf_Entity = Surface::OnLoad(File)) == NULL)
	{
        return false;
    }
 
    Surface::Transparent(Surf_Entity, 255, 0, 255);
 
    this->Width = Width;
    this->Height = Height;
 
    return true;
}
 
void Entity::OnLoop()
{
    //We're not Moving
    if(MoveLeft == false && MoveRight == false)
	{
        StopMove();
    }
 
    if(MoveLeft)
	{
        AccelX = -0.5;
    }
	else if(MoveRight)
	{
        AccelX = 0.5;
    }
	if(Flags & ENTITY_FLAG_GRAVITY)
	{
        AccelY = 0.75f;
    }
 
    SpeedX += AccelX * FPS::FPSControl.GetSpeedFactor();
    SpeedY += AccelY * FPS::FPSControl.GetSpeedFactor();
 
    if(SpeedX > MaxSpeedX)  SpeedX =  MaxSpeedX;
    if(SpeedX < -MaxSpeedX) SpeedX = -MaxSpeedX;
    if(SpeedY > MaxSpeedY)  SpeedY =  MaxSpeedY;
    if(SpeedY < -MaxSpeedY) SpeedY = -MaxSpeedY;
 
    OnAnimate();
    OnMove(SpeedX, SpeedY);
}
 
void Entity::OnRender(SDL_Surface* Surf_Display)
{
    if(Surf_Entity == NULL || Surf_Display == NULL) return;
 
    Surface::OnDraw(Surf_Display, Surf_Entity, X - Camera::CameraControl.GetX(), Y - Camera::CameraControl.GetY(), CurrentFrameCol * Width, (CurrentFrameRow + Anim_Control.GetCurrentFrame()) * Height, Width, Height);
	//Surface::OnDraw(Surf_Display, Surf_Entity, X - Camera::CameraControl.GetX(), Y - Camera::CameraControl.GetY(), (CurrentFrameCol + Anim_Control.GetCurrentFrame()) * Width, CurrentFrameRow * Height, Width, Height);
}

void Entity::OnAnimate()
{
    if(MoveLeft)
	{
        CurrentFrameCol = 0;
    }
	else if(MoveRight)
	{
        CurrentFrameCol = 1;
    }
 
    Anim_Control.OnAnimate();
}

void Entity::OnMove(float MoveX, float MoveY)
{
	CanJump = false;

    if(MoveX == 0 && MoveY == 0) return;
 
    double NewX = 0;
    double NewY = 0;
 
    MoveX *= FPS::FPSControl.GetSpeedFactor();
    MoveY *= FPS::FPSControl.GetSpeedFactor();
 
    if(MoveX != 0)
	{
        if(MoveX >= 0)     NewX =  FPS::FPSControl.GetSpeedFactor();
        else             NewX = -FPS::FPSControl.GetSpeedFactor();
    }
 
    if(MoveY != 0)
	{
        if(MoveY >= 0)     NewY =  FPS::FPSControl.GetSpeedFactor();
        else             NewY = -FPS::FPSControl.GetSpeedFactor();
    }
 
    while(true)
	{
        if(Flags & ENTITY_FLAG_GHOST)
		{
            PosValid((int)(X + NewX), (int)(Y + NewY)); //We don't care about collisions, but we need to send events to other entities
 
            X += NewX;
            Y += NewY;
        }
		else
		{
            if(PosValid((int)(X + NewX), (int)(Y)))
			{
                X += NewX;
            }
			else
			{
                SpeedX = 0;
            }
 
            if(PosValid((int)(X), (int)(Y + NewY)))
			{
				Y += NewY;
			}
			else
			{
				if(MoveY > 0)
				{
					CanJump = true;
				}
				SpeedY = 0;
			}
        }
 
        MoveX += -NewX;
        MoveY += -NewY;
 
        if(NewX > 0 && MoveX <= 0) NewX = 0;
        if(NewX < 0 && MoveX >= 0) NewX = 0;
 
        if(NewY > 0 && MoveY <= 0) NewY = 0;
        if(NewY < 0 && MoveY >= 0) NewY = 0;
 
        if(MoveX == 0) NewX = 0;
        if(MoveY == 0) NewY = 0;
 
        if(MoveX == 0 && MoveY     == 0)     break;
        if(NewX  == 0 && NewY     == 0)     break;
    }
}

void Entity::StopMove()
{
    if(SpeedX > 0)
	{
        AccelX = -1;
    }
 
    if(SpeedX < 0)
	{
        AccelX =  1;
    }
 
    if(SpeedX < 2.0f && SpeedX > -2.0f)
	{
        AccelX = 0;
        SpeedX = 0;
    }
}

bool Entity::Jump()
{
    if(CanJump == false) return false;
 
    SpeedY = -MaxSpeedY;
 
    return true;
}

bool Entity::Carry(Entity* Entity)
{
	int tempFlags = Entity->Flags;
	bool letGo = false;
	//if (PosValidEntity(Entity, Entity->X, Entity->Y))
	if (abs(Entity->X - this->X) > 40 && !isCarrying)
	{
		Entity->Flags = ENTITY_FLAG_MAPONLY;
		Entity->StopMove();
		Entity->MoveLeft = false;
		Entity->MoveRight = false;
		Entity->Y = this->Y - 10;
		if (this->MoveLeft)
		{
			Entity->X = this->X - 20;
		}
		if (this->MoveRight)
		{
			Entity->X = this->X + 20;
		}
		letGo = false;
		//return true;
	}
	else if (!letGo)
	{
		if (this->MoveLeft)
		{
			Entity->X = this->X - 20;
		}
		if (this->MoveRight)
		{
			Entity->X = this->X + 20;
		}
	}
	Entity->Flags = tempFlags;
	isCarrying = false;
	return false;
}

bool Entity::Follow(Entity* Entity)
{
	if (this->X < Entity->X + this->Width + 20)
	{
		this->MoveLeft = false;
		this->MoveRight = true;
	}
	else if (this->X > Entity->X - this->Width - 20)
	{
		this->MoveRight = false;
		this->MoveLeft = true;
	}
	else
	{
		this->StopMove();
	}
	return true;
}

bool Entity::Collides(int oX, int oY, int oW, int oH)
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
 
    return true;
}

bool Entity::PosValid(int NewX, int NewY)
{
    bool Return = true;
 
    int StartX     = (NewX + Col_X) / TILE_SIZE;
    int StartY     = (NewY + Col_Y) / TILE_SIZE;
 
    int EndX    = ((NewX + Col_X) + Width - Col_Width - 1)         / TILE_SIZE;
    int EndY    = ((NewY + Col_Y) + Height - Col_Height - 1)    / TILE_SIZE;
 
    for(int iY = StartY;iY <= EndY;iY++)
	{
        for(int iX = StartX;iX <= EndX;iX++)
		{
            Tile* Tile = Area::AreaControl.GetTile(iX * TILE_SIZE, iY * TILE_SIZE);
 
            if(PosValidTile(Tile) == false)
			{
                Return = false;
            }
        }
    }
 
    if(Flags & ENTITY_FLAG_MAPONLY)
	{
    }
	else
	{
        for(int i = 0;i < EntityList.size();i++)
		{
            if(PosValidEntity(EntityList[i], NewX, NewY) == false)
			{
                Return = false;
            }
        }
    }
 
    return Return;
}

bool Entity::PosValidEntity(Entity* Entity, int NewX, int NewY)
{
    if(this != Entity && Entity != NULL && Entity->Dead == false &&
        Entity->Flags ^ ENTITY_FLAG_MAPONLY &&
        Entity->Collides(NewX + Col_X, NewY + Col_Y, Width - Col_Width - 1, Height - Col_Height - 1) == true)
	{
 
        EntityCol EntityCol;
 
        EntityCol.EntityA = this;
        EntityCol.EntityB = Entity;
 
        EntityCol::EntityColList.push_back(EntityCol);
 
        return false;
    }
    return true;
}

bool Entity::PosValidTile(Tile* Tile)
{
    if(Tile == NULL)
	{
        return true;
    }
 
    if(Tile->TypeID == TILE_TYPE_BLOCK)
	{
        return false;
    }
 
    return true;
}

bool Entity::OnCollision(Entity* Entity)
{
	/*if(this->Type == ENTITY_TYPE_PLAYER && Entity->Type == ENTITY_TYPE_PLAYER)
    {
        return false;
    }*/
	return true;
}
// Pixel Collision starts
SDL_Rect Entity::GetBounds()
{
	SDL_Rect bounds;
	bounds.x = (Sint16)(X - Width);
	bounds.y = (Sint16)(Y - Height);
	bounds.w = (Sint16)(Width);
	bounds.h = (Sint16)(Height);

	return bounds;
}

SDL_Rect Entity::Intersection(const SDL_Rect& boundsA, const SDL_Rect& boundsB)
{
	int x1 = Maximum(boundsA.x, boundsB.x);
	int y1 = Maximum(boundsA.y, boundsB.y);
	int x2 = Minimum(boundsA.x + boundsA.w, boundsB.x + boundsB.w);
	int y2 = Minimum(boundsA.y + boundsA.h, boundsB.y + boundsB.h);
	
	int width = x2 - x1;
	int height = y2 - y1;
	
	if(width > 0 && height > 0)
	{
		SDL_Rect intersect = {x1, y1, width, height};
		return intersect;
	}
	else
	{
		SDL_Rect intersect = {0, 0, 0, 0};
		return intersect;
	}
}

SDL_Rect Entity::GetFrameBounds()
{
	SDL_Rect frameBounds;
	/*frameBounds.x = (Sint16)(Anim_Control % tilesX * Width);
	frameBounds.y = (Sint16)(Anim_Control / tilesX * Height);
	frameBounds.w = (Sint16)(Width);
	frameBounds.h = (Sint16)(Height);
	*/
	return frameBounds;
}

SDL_Rect Entity::NormalizeBounds(const SDL_Rect& rect)
{
	SDL_Rect normalized;
	normalized.x = rect.x - (Sint16)X + (Sint16)(X - Width) + GetFrameBounds().x;
	normalized.y = rect.y - (Sint16)Y + (Sint16)(Y - Height) + GetFrameBounds().y;
	normalized.w = rect.w;
	normalized.h = rect.h;
	
	return normalized;
}

bool Entity::CheckCollision(Entity* entityA, Entity* entityB)
{
	SDL_Rect collisionRect = Intersection(entityA->GetBounds(), entityB->GetBounds());

	if(collisionRect.w == 0 && collisionRect.h == 0)
		return false;

	SDL_Rect normalA = entityA->NormalizeBounds(collisionRect);
	SDL_Rect normalB = entityB->NormalizeBounds(collisionRect);

	for(int y = 0; y <= collisionRect.h; y++)
	{
		for(int x = 0; x <= collisionRect.w; x++)
		{
			if(GetAlphaXY(entityA, normalA.x + x, normalA.y + y) && GetAlphaXY(entityB, normalB.x + x, normalB.y + y))
				return true;
		}
	}

	return false;
}

bool Entity::GetAlphaXY(Entity* entity, int x, int y)
{
	int bpp = entity->Surf_Entity->format->BytesPerPixel; 
	Uint8* p = (Uint8*)entity->Surf_Entity->pixels + y * entity->Surf_Entity->pitch + x * bpp;
	Uint32 pixelColor;
	
	switch(bpp)
	{
		case(1):
			pixelColor = *p;
			break;
		case(2):
			pixelColor = *(Uint16*)p;
			break;
		case(3):
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
				pixelColor = p[0] << 16 | p[1] << 8 | p[2];
			else
				pixelColor = p[0] | p[1] << 8 | p[2] << 16;
			break;
		case(4):
			pixelColor = *(Uint32*)p;
			break;
	}
	
	Uint8 red, green, blue, alpha;
	SDL_GetRGBA(pixelColor, entity->Surf_Entity->format, &red, &green, &blue, &alpha);

	return alpha > 200;
}
// Pixel Collision ends

void Entity::OnCleanup()
{
    if(Surf_Entity)
	{
        SDL_FreeSurface(Surf_Entity);
    }
 
    Surf_Entity = NULL;
}