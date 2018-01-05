#include <stdio.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;
TTF_Font *gFont = NULL;

int main(int argc, char* args[])
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
		printf("SDL_image cound not initialize! SDL_Image Error: %s\n", IMG_GetError());
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

	bool quit = false;
	SDL_Event evt;

	while (!quit)
	{
		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT || (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE))
				quit = true;
			else if (evt.type == SDL_KEYDOWN)
			{
				
			}
			
		}
		
		SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 0xFF);
		SDL_RenderClear(gRenderer);
		SDL_RenderPresent(gRenderer);	
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
