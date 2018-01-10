#include <stdio.h>
#include <fstream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Texture.h"
#include "Timer.h"
#include "Config.h"
#include "Tile.h"

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;
Timer fpsTimer;

Tile *mapTiles[TILES_MAP_COUNT];
Texture *spriteSheet = new Texture();
SDL_Rect clips[TILES_SPRITESHEET];


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
					/*if (musicIsPlaying)
					{
						musicIsPlaying = false;
						Mix_PauseMusic();
					}	
					else
					
						musicIsPlaying = true;
						Mix_PlayMusic(musicTest, -1);
					}*/
					break; 
				
				case SDLK_SPACE:
					//Mix_PlayChannel(-1, sfxTest, 0);
					break;
				}
			}
		}
		
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);

		for (int t = 0; t < TILES_MAP_COUNT; t++)
		{
			mapTiles[t]->Draw();
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

	//if (!spriteSheet->LoadFromFile(gRenderer, "spriteSheet.png")) return false;
	if (!spriteSheet->LoadFromFile(gRenderer, "spriteSheet16x16.png")) return false;


	/*Texture testTex;
	if (!testTex.LoadFromFile(gRenderer, "test.png")) return false;

	Mix_Music* musicTest = nullptr;
	musicTest = Mix_LoadMUS("testMusic.mp3");
	if (musicTest == NULL)
	{
		printf("Failed to load mp3! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	bool musicIsPlaying = false;

	Mix_Chunk* sfxTest;
	sfxTest = Mix_LoadWAV("testSfx.wav");
	if (sfxTest == NULL)
	{
		printf("Failed to load wav! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}*/


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

	spriteSheet->Free();
	delete spriteSheet;

	/*
	Mix_FreeMusic(musicTest);
	musicTest = NULL;

	Mix_FreeChunk(sfxTest);
	sfxTest = NULL;
	*/

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
	clips[TILE_PLAYER2] = { TILE_WIDTH * 4, 0, TILE_WIDTH, TILE_HEIGHT };

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
			SDL_Rect *box = new SDL_Rect { x, y, TILE_WIDTH, TILE_HEIGHT };
			tile = new Tile(box, tileType, id++, spriteSheet, &clips[tileType]);
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
