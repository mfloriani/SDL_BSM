#include "SDLGame.h"

SDLGame* SDLGame::GetInstance()
{
	static SDLGame instance;
	return &instance;
}

bool SDLGame::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	window_ = SDL_CreateWindow("BSM", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window_ == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer_ == NULL)
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

	font_ = TTF_OpenFont("arial.ttf", 18);
	if (font_ == NULL)
	{
		printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
		return false;
	}

	return true;
}

void SDLGame::Shutdown()
{
	delete camera_;
	delete world_;

	TTF_CloseFont(font_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int SDLGame::Run()
{
	if (!Initialize())
	{
		Shutdown();
		return 0;
	}

	camera_ = new SDL_Rect();
	camera_->x = 0;
	camera_->y = 0;
	camera_->w = SCREEN_WIDTH;
	camera_->h = SCREEN_HEIGHT;

	world_ = new World();
	if (!world_->Initialize()) return 0;

	bool quit = false;
	SDL_Event evt;
	
	Timer fpsTimer;
	fpsTimer.start();
	Uint32 before = SDL_GetTicks();
	Uint32 now = 0;
	int framesCount = 0;
	
	while (!quit)
	{
		//calculate the time that has passed since last frame
		now = SDL_GetTicks() - before;
		before = SDL_GetTicks();
		float secs = now / 1000.0f;

		//calculate the framerate
		float avgFPS = framesCount / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000) avgFPS = 0;

		//show the framerate 
		std::string fps;
		fps = "FPS " + std::to_string(avgFPS);
		SDL_SetWindowTitle(window_, fps.c_str());

		while (SDL_PollEvent(&evt) != 0)
		{
			if (evt.type == SDL_QUIT || (evt.type == SDL_KEYDOWN && evt.key.keysym.sym == SDLK_ESCAPE)) quit = true;

			world_->HandleInput(&evt);
		}

		world_->Update(secs);

		SDL_SetRenderDrawColor(renderer_, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer_);

		world_->Draw();
		
		SDL_RenderPresent(renderer_);
		++framesCount;
	}
	Shutdown();
	return 0;
}
