#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "../World/World.h"
#include "../Messaging/Messenger.h"
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include "../Utils/Timer.h"
#include "../Scripting/LuaParameters.h"


#define Game SDLGame::GetInstance()

class SDLGame
{
private:
	SDLGame(){}
	SDLGame(const SDLGame&);
	SDLGame& operator=(const SDLGame&);
	bool Initialize();
	void Shutdown();

	SDL_Rect*		camera_;
	SDL_Window*		window_;
	SDL_Renderer*	renderer_;
	TTF_Font*		font_;
	World*			world_;

public:
	static SDLGame* GetInstance();
	int				Run();
	SDL_Renderer*	GetRenderer()const { return renderer_; }


};

