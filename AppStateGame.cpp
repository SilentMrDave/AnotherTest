#include "AppStateGame.h"
 
AppStateGame AppStateGame::Instance;
 
AppStateGame::AppStateGame()
{

}
 
void AppStateGame::OnKeyDown(SDLKey sym, SDLMod mod, Uint16 unicode)
{
switch(sym) {
        case SDLK_LEFT:
		{
			(Players + CurrentPlayer)->MoveLeft = true;
            break;
        }
 
        case SDLK_RIGHT:
		{
			(Players + CurrentPlayer)->MoveRight = true;
            break;
        }

		case SDLK_SPACE:
		{
			(Players + CurrentPlayer)->Jump();
		   break;
		}
		
 
        default: {
        }
    }
}
 
void AppStateGame::OnKeyUp(SDLKey sym, SDLMod mod, Uint16 unicode)
{
    switch(sym)
	{
        case SDLK_LEFT:
		{
			(Players + CurrentPlayer)->MoveLeft = false;
            break;
        }
 
        case SDLK_RIGHT:
		{
			(Players + CurrentPlayer)->MoveRight = false;
            break;
        }

		case SDLK_q:
		{
			SwitchToPlayer(NextPlayer());
			break;
		}
		case SDLK_a:
		{
			SwitchToPlayer(PreviousPlayer());
			break;
		}
		case SDLK_f:
		{
			follow = !follow;
			if (!follow)
			{
				(Players + CurrentPlayer - 1)->MoveRight = false;
				(Players + CurrentPlayer - 1)->MoveLeft = false;
			}
			break;
		}
		case SDLK_g:
		{
			if (gravity == 1)
			{
				gravity *= -1;
			}
			else
			{
				gravity = 1;
			}
			break;
		}
		case SDLK_e:
		{
			carry = !carry;
			break;
		}
		case SDLK_n:
		{
			/*PlayerAmount++;
			Player* temp = new Player[PlayerAmount];
			std::copy(temp, temp + PlayerAmount - 1, Players);
			delete [] Players;
			Players = temp;
			Entity::EntityList.resize(Entity::EntityList.size() + 1);
			Entity::EntityList.push_back((Players + PlayerAmount - 1));
			(Players + PlayerAmount - 1)->X = (Players + CurrentPlayer)->X + 30;
			(Players + PlayerAmount - 1)->Y = (Players + CurrentPlayer)->Y;*/
			break;
		}
        default:
		{
        }
    }
}
 
void AppStateGame::OnActivate()
{
    if(Area::AreaControl.OnLoad("maps/1.area") == false)
	{
        return;
    }
	// Create and initialize our Player array
	CurrentPlayer = 0;
	PlayerAmount = 3;
	Players = new Player[PlayerAmount];
	for (int i = 0; i < PlayerAmount; i++)
	{
		if((Players + i)->OnLoad("gfx/stickman.png", 64, 64, 6) == false)
		{
			return;
		}
	}
	if(Box.OnLoad("gfx/Crate.png", 32, 32, 0) == false)
	{
		return;
	}
	Box.Flags = ENTITY_FLAG_GHOST & ENTITY_FLAG_GRAVITY;
	(Players + 1)->X = 100;
	(Players + 1)->Y = 50;
	(Players + 2)->X = 300;
	(Players + 2)->Y = 50;
	Players->Y = 50;
	Box.X = 200;
	Box.Y = 50;
	follow = true;
	carry = false;
	for (int i = 0; i < PlayerAmount; i++)
	{
		Entity::EntityList.push_back((Players + i));
	}
	Entity::EntityList.push_back(&Box);
 
    Camera::CameraControl.TargetMode = TARGET_MODE_CENTER;
    Camera::CameraControl.SetTarget(&(Players + CurrentPlayer)->X, &(Players + CurrentPlayer)->Y);
}
 
void AppStateGame::OnDeactivate()
{
    Area::AreaControl.OnCleanup();
 
    for(int i = 0;i < Entity::EntityList.size();i++)
	{
        if(!Entity::EntityList[i]) continue;
 
        Entity::EntityList[i]->OnCleanup();
    }
 
    Entity::EntityList.clear();
}
 
void AppStateGame::OnLoop()
{
    for(int i = 0;i < Entity::EntityList.size();i++)
	{
        if(!Entity::EntityList[i]) continue;
 
        Entity::EntityList[i]->OnLoop();
    }
 
    //Collision Events
    for(int i = 0;i < EntityCol::EntityColList.size();i++)
	{
        Entity* EntityA = EntityCol::EntityColList[i].EntityA;
        Entity* EntityB = EntityCol::EntityColList[i].EntityB;
 
        if(EntityA == NULL || EntityB == NULL) continue;
 
        if(EntityA->OnCollision(EntityB)) {
            EntityB->OnCollision(EntityA);
        }
    }
	if(follow)
	{
		(Players + CurrentPlayer - 1)->Follow((Players + CurrentPlayer));
	}
	if(carry)
	{
		(Players + CurrentPlayer)->Carry((Players + CurrentPlayer - 1));
	}
	else
	{
		(Players + CurrentPlayer)->emptyHands = true;
	}
    EntityCol::EntityColList.clear();
}
 
void AppStateGame::OnRender(SDL_Surface* Surf_Display)
{
    SDL_Rect Rect;
    Rect.x = 0;
    Rect.y = 0;
    Rect.w = WWIDTH;
    Rect.h = WHEIGHT;
 
    SDL_FillRect(Surf_Display, &Rect, 0xffffff);
 
    Area::AreaControl.OnRender(Surf_Display, -Camera::CameraControl.GetX(), -Camera::CameraControl.GetY());
 
    for(int i = 0;i < Entity::EntityList.size();i++)
	{
        if(!Entity::EntityList[i]) continue;
 
        Entity::EntityList[i]->OnRender(Surf_Display);
    }
}
 
Player* AppStateGame::PreviousPlayer()
{
	(Players + CurrentPlayer)->MoveRight = false;
	(Players + CurrentPlayer)->MoveLeft = false;
	if ( CurrentPlayer - 1 < 0)
	{
		CurrentPlayer = PlayerAmount - 1;
		return (Players + PlayerAmount - 1);
	}
	else
	{
		CurrentPlayer--;
		return (Players + CurrentPlayer);
	}
}

Player* AppStateGame::NextPlayer()
{
	(Players + CurrentPlayer)->MoveRight = false;
	(Players + CurrentPlayer)->MoveLeft = false;
	if (CurrentPlayer < PlayerAmount - 1)
	{
		CurrentPlayer++;
		return (Players + CurrentPlayer);
	}
	else
	{
		CurrentPlayer = 0;
		return (Players);
	}
}

void AppStateGame::SwitchToPlayer(Player* Player)
{
	Camera::CameraControl.SetTarget(&Player->X, &Player ->Y);
}

AppStateGame* AppStateGame::GetInstance()
{
    return &Instance;
}