#include "SDLMain.h"

bool SDLMain::VeryFirstInit()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return ErrorLogger("SDL_INIT_VIDEO or SDL_INIT_AUDIO Failed");

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	if (fullscreen)
		mainWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_FULLSCREEN);
	else
		mainWindow = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (mainWindow == NULL)
		return ErrorLogger("Window Creation Failed");

	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetLogicalSize(mainRenderer, width, height);
	if (mainRenderer == NULL)
		return ErrorLogger("Main Renderer failed");
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);

	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG)
		return ErrorLogger("IMG_INIT_PNG");

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) < 0)
	{
		SDL_Log("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
		return false;
	}
	return true;
}

void SDLMain::Exit()
{
	Running = false;
}

bool SDLMain::ErrorLogger(const char * message)
{
	SDL_Log("ERROR: %s, SDL Report: %s", message, SDL_GetError());
	return false;
}


SDLMain::SDLMain(const char * windowTitle, int windowWidth, int windowHeight, bool fullscreen)
{
	title = new char[SDL_strlen(windowTitle)];
	SDL_strlcpy(title, windowTitle, 100);
	width = windowWidth;
	height = windowHeight;
	this->fullscreen = fullscreen;
}

SDLMain::~SDLMain()
{
	SDL_DestroyRenderer(mainRenderer);
	SDL_DestroyWindow(mainWindow);
	exit(0);
}

int SDLMain::Run()
{
	if (!(VeryFirstInit() && OnInit()))
		return -1;

	Running = true;

	SDL_Event Event;

	while (Running)
	{
		static int oldtime;
		dt = SDL_GetTicks() - oldtime;
		oldtime = SDL_GetTicks();
		if (dt > 50)
			dt = 50;
		dt /= 1000.0;

		while (SDL_PollEvent(&Event))
		{
			OnEvent(&Event);
		}

		OnLoop();
		OnRender();
	}

	OnCleanup();

	return 0;
}
