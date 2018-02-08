#include <stdio.h>
#include <fstream>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Timer.h"
#include "Config.h"
#include "Tile.h"
#include "GameObject.h"
#include "Player.h"
#include "Enemy.h"
#include "GameObjectManager.h"
#include "Messenger.h"
#include "WindowsClock.h"

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
Timer fpsTimer;

Tile *mapTiles[TILES_MAP_COUNT];
Texture *spriteSheetTex = new Texture();
SDL_Rect clips[TILES_SPRITESHEET];

Texture *player1Tex = new Texture();
Player *player1 = NULL;
Texture *bulletTex = new Texture();

Texture *enemyTex = new Texture();

vector<GameObject*> gameObjects;
vector<GameObject*>::iterator goIter;

int framesCount = 0;

bool Init();
bool Load();
void Shutdown();
void CalcFPS();
void LoadClips();
bool LoadMap(std::string filename);


int main(int argc, char* args[])
{
	if (!Init())
	{
		Shutdown();
		return 0;
	}

	if (!Load())
	{
		Shutdown();
		return 0;
	}
	
	bool quit = false;
	SDL_Event evt;

	fpsTimer.start();
	Uint32 before = SDL_GetTicks();
	Uint32 now = 0;

	while (!quit)
	{
		now = SDL_GetTicks() - before;
		before = SDL_GetTicks();
		float secs = now / 1000.0f;

		CalcFPS();

		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT || (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE))
				quit = true;
			else if (evt.type == SDL_KEYDOWN)
			{
				switch (evt.key.keysym.sym)
				{
				case SDLK_RETURN:
					
					break;
				case SDLK_SPACE:
					
					break;
				}
			}
			player1->HandleInput(&evt);
		}
		
		player1->Update(secs, mapTiles, &gameObjects);

		if (gameObjects.size() > 0)
		{
			for (goIter = gameObjects.begin(); goIter != gameObjects.end(); ++goIter)
			{
				if ((*goIter) != NULL && (*goIter)->IsActive())
				{
					if (dynamic_cast<Bullet*>(*goIter) != nullptr)
					{
						(dynamic_cast<Bullet*>(*goIter))->Update(secs, mapTiles, &gameObjects);
					}
					else if (dynamic_cast<Enemy*>(*goIter) != nullptr)
					{
						(dynamic_cast<Enemy*>(*goIter))->Update(secs, mapTiles, &gameObjects);
					}
				}
			}
		}

		Msger->SendDelayedMsg(); //send all the queued messages ready to be sent

		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		for (int t = 0; t < TILES_MAP_COUNT; t++)
		{
			mapTiles[t]->Draw();
		}

		player1->Draw(secs);

		if (gameObjects.size() > 0)
		{
			for (goIter = gameObjects.begin(); goIter != gameObjects.end(); ++goIter)
			{
				if ((*goIter) != NULL && (*goIter)->IsActive())
				{
					(*goIter)->Draw(secs);
				}
			}
		}

		SDL_RenderPresent(gRenderer);
		++framesCount;
	}
	Shutdown();
	return 0;
}

bool Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	gWindow = SDL_CreateWindow("BSM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (gWindow == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (gRenderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags)&imgFlags))
	{
		printf("SDL_image could not initialize! SDL_Image Error: %s\n", IMG_GetError());
		return false;
	}
	if (TTF_Init() == -1)
	{
		printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

bool Load()
{
	gFont = TTF_OpenFont("arial.ttf", 18);
	if (gFont == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	if (!spriteSheetTex->LoadFromFile(gRenderer, "spriteSheet16x16.png")) return false;

	if (!player1Tex->LoadFromFile(gRenderer, "player1.png")) return false;

	if (!bulletTex->LoadFromFile(gRenderer, "bullet1.png")) return false;

	if (!enemyTex->LoadFromFile(gRenderer, "enemy.png")) return false;

	LoadClips();
	
	if (!LoadMap("map_01.map"))
	{
		printf("Failed to load map_01.map!");
		return false;
	}
	

	return true;
}

void Shutdown()
{
	TTF_CloseFont(gFont);
	gFont = NULL;

	spriteSheetTex->Free();
	delete spriteSheetTex;
	spriteSheetTex = NULL;

	player1Tex->Free();
	delete player1Tex;
	player1Tex = NULL;

	bulletTex->Free();
	delete bulletTex;
	bulletTex = NULL;

	enemyTex->Free();
	delete enemyTex;
	enemyTex = NULL;

	if (gameObjects.size() > 0)
	{
		for (goIter = gameObjects.begin(); goIter != gameObjects.end(); ++goIter)
		{
			if ((*goIter) != NULL)
			{
				delete (*goIter);
			}
		}
	}

	SDL_DestroyRenderer(gRenderer);
	gRenderer = NULL;
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;

	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void CalcFPS()
{
	float avgFPS = framesCount / (fpsTimer.getTicks() / 1000.f);
	if (avgFPS > 2000000)
		avgFPS = 0;

	std::string fps;
	fps = "FPS " + std::to_string(avgFPS);
	SDL_SetWindowTitle(gWindow, fps.c_str());
}

void LoadClips()
{
	clips[TILE_NULL] = { 0, 0, TILE_WIDTH, TILE_HEIGHT };
	clips[TILE_WALL] = { TILE_WIDTH, 0, TILE_WIDTH, TILE_HEIGHT };
	clips[TILE_ENEMY] = { TILE_WIDTH*2, 0, TILE_WIDTH, TILE_HEIGHT };
	clips[TILE_PLAYER1] = { TILE_WIDTH*3, 0, TILE_WIDTH, TILE_HEIGHT };
	clips[TILE_PLAYER2] = { TILE_WIDTH*4, 0, TILE_WIDTH, TILE_HEIGHT };
}

bool LoadMap(std::string filename)
{
	int x = 0;
	int y = 0;
	int id = 0;

	std::ifstream map(filename);

	if (!map)
	{
		printf("Failed to load map file!\n");
		return false;
	}

	for (int t = 0; t < TILES_MAP_COUNT; t++)
	{
		int tileType = -1;
		Tile* tile;

		map >> tileType;

		if (map.fail())
		{
			printf("Failed to load map!\n");
			return false;
		}

		if (tileType == TILE_NULL || tileType == TILE_WALL || tileType == TILE_ENEMY || tileType == TILE_PLAYER1 || tileType == TILE_PLAYER2)
		{
			if (tileType == TILE_PLAYER1)
			{
				tileType = TILE_NULL;
				player1 = new Player(Vector2D(x,y), player1Tex, bulletTex);
				GoManager->AddGameObject(player1);
			}
			else if (tileType == TILE_ENEMY)
			{
				tileType = TILE_NULL;
				Enemy* en = new Enemy(Vector2D(x, y), enemyTex);
				gameObjects.push_back(en);
				GoManager->AddGameObject(en);
			}

			bool isCollidable = false;
			if (tileType == TILE_WALL)
			{
				isCollidable = true;
			}

			

			SDL_Rect *box = new SDL_Rect { x, y, TILE_WIDTH, TILE_HEIGHT };
			tile = new Tile(box, tileType, id++, spriteSheetTex, &clips[tileType], isCollidable);
			mapTiles[t] = tile;
		}
		else
		{
			printf("Failed to load fileType! [%s]\n", tileType);
			map.close();
			return false;
		}

		x += TILE_WIDTH;
		if (x >= SCREEN_WIDTH)
		{
			x = 0;
			y += TILE_HEIGHT;
		}
	}

	map.close();
	return true;
}
