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
			/*if (isFirstPlayer)
			{
				Player1.MoveLeft = true;
			}
			else
			{
				Player2.MoveLeft = true;
			}*/
			(Players + CurrentPlayer)->MoveLeft = true;
            break;
        }
 
        case SDLK_RIGHT:
		{
			/*if (isFirstPlayer)
			{
				Player1.MoveRight = true;
			}
			else
			{
				Player2.MoveRight = true;
			}*/
			(Players + CurrentPlayer)->MoveRight = true;
            break;
        }

		case SDLK_SPACE:
		{
			/*if (isFirstPlayer)
			{
				Player1.Jump();
			}
			else
			{
				Player2.Jump();
			}*/
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
			/*if (isFirstPlayer)
			{
				Player1.MoveLeft = false;
			}
			else
			{
				Player2.MoveLeft = false;
			}*/
			(Players + CurrentPlayer)->MoveLeft = false;
            break;
        }
 
        case SDLK_RIGHT:
		{
			/*if (isFirstPlayer)
			{
				Player1.MoveRight = false;
			}
			else
			{
				Player2.MoveRight = false;
			}*/
			(Players + CurrentPlayer)->MoveRight = false;
            break;
        }

		case SDLK_q:
		{
			if (CurrentPlayer < PlayerAmount - 1)
			{
				CurrentPlayer++;
			}
			else
			{
				CurrentPlayer = 0;
			}
			Camera::CameraControl.SetTarget(&(Players + CurrentPlayer)->X, &(Players + CurrentPlayer)->Y);
			if ( CurrentPlayer - 1 < 0)
			{
				(Players + PlayerAmount - 1)->MoveRight = false;
				(Players + PlayerAmount - 1)->MoveLeft = false;
			}
			/*if ( CurrentPlayer + 1 > PlayerAmount - 1)
			{
				(Players)->MoveRight = false;
				(Players)->MoveLeft = false;
			}*/
			else
			{
				(Players + CurrentPlayer - 1)->MoveRight = false;
				(Players + CurrentPlayer - 1)->MoveLeft = false;
			}
			/*if (isFirstPlayer)
			{
				isFirstPlayer = false;
				Camera::CameraControl.SetTarget(&Player2.X, &Player2.Y);
				Player1.MoveRight = false;
				Player1.MoveLeft = false;
			}
			else
			{
				isFirstPlayer = true;
				Camera::CameraControl.SetTarget(&Player1.X, &Player1.Y);
				Player2.MoveRight = false;
				Player2.MoveLeft = false;
			}*/
			break;
		}
		case SDLK_f:
		{
			follow = !follow;
			if (!follow)
			{
				Player2.MoveRight = false;
				Player2.MoveLeft = false;
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
			(Players + CurrentPlayer)->Carry((Players + CurrentPlayer - 1));
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
	CurrentPlayer = 0;
	PlayerAmount = 3;
	Players = new Player[PlayerAmount];
	for (int i = 0; i < PlayerAmount; i++)
	{
		if((Players + i)->OnLoad("gfx/yoshi.png", 64, 64, 8) == false)
		{
			return;
		}
	}
 
    /*if(Player2.OnLoad("gfx/yoshi.png", 64, 64, 8) == false)
	{
        return;
    }*/
	if(Box.OnLoad("gfx/Crate.png", 32, 32, 0) == false)
	{
		return;
	}
    /*Player2.X = 100;
	Player2.Y = 50;
	Player1.Y = 50;*/
	
	(Players + 1)->X = 100;
	(Players + 1)->Y = 50;
	(Players + 2)->X = 300;
	(Players + 2)->Y = 50;
	Players->Y = 50;
	Box.X = 200;
	Box.Y = 50;
	isFirstPlayer = true;
	follow = true;
	for (int i = 0; i < PlayerAmount; i++)
	{
		Entity::EntityList.push_back((Players + i));
	}
    Entity::EntityList.push_back(&Player1);
    Entity::EntityList.push_back(&Player2);
	Entity::EntityList.push_back(&Box);
 
    Camera::CameraControl.TargetMode = TARGET_MODE_CENTER;
    Camera::CameraControl.SetTarget(&(Players + CurrentPlayer)->X, &(Players + CurrentPlayer)->Y);
}
 
void AppStateGame::OnDeactivate() {
    Area::AreaControl.OnCleanup();
 
    for(int i = 0;i < Entity::EntityList.size();i++) {
        if(!Entity::EntityList[i]) continue;
 
        Entity::EntityList[i]->OnCleanup();
    }
 
    Entity::EntityList.clear();
}
 
void AppStateGame::OnLoop() {
    for(int i = 0;i < Entity::EntityList.size();i++) {
        if(!Entity::EntityList[i]) continue;
 
        Entity::EntityList[i]->OnLoop();
    }
 
    //Collision Events
    for(int i = 0;i < EntityCol::EntityColList.size();i++) {
        Entity* EntityA = EntityCol::EntityColList[i].EntityA;
        Entity* EntityB = EntityCol::EntityColList[i].EntityB;
 
        if(EntityA == NULL || EntityB == NULL) continue;
 
        if(EntityA->OnCollision(EntityB)) {
            EntityB->OnCollision(EntityA);
        }
    }
	if(follow)
	{
		if(isFirstPlayer)
		{
			Player2.Follow(&Player1);
		}
	}
    EntityCol::EntityColList.clear();
}
 
void AppStateGame::OnRender(SDL_Surface* Surf_Display) {
    SDL_Rect Rect;
    Rect.x = 0;
    Rect.y = 0;
    Rect.w = WWIDTH;
    Rect.h = WHEIGHT;
 
    SDL_FillRect(Surf_Display, &Rect, 0);
 
    Area::AreaControl.OnRender(Surf_Display, -Camera::CameraControl.GetX(), -Camera::CameraControl.GetY());
 
    for(int i = 0;i < Entity::EntityList.size();i++) {
        if(!Entity::EntityList[i]) continue;
 
        Entity::EntityList[i]->OnRender(Surf_Display);
    }
}
 
AppStateGame* AppStateGame::GetInstance() {
    return &Instance;
}