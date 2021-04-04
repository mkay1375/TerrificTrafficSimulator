#pragma once
#include <SDL.h>	// To install and add SDL2 to your project
					// right click on your solution in Solution Explorer
					// and open Manage NuGet Packages.
					// in Browse tab search for SDL2 and click install
					// If you want to use PNG images install SDL2_image too.
#include <SDL_image.h>
#include <SDL_mixer.h>

class SDLMain
{
private:
	bool Running;
protected:
	float dt; // the time between frames

	char * title;
	int width;
	int height;
	bool fullscreen;
	SDL_Window * mainWindow;
	SDL_Renderer * mainRenderer; // Note that it's available in OnInit funtion NOT in constructor.
	virtual bool VeryFirstInit();
	void Exit();

	virtual bool OnInit() = 0;
	virtual void OnEvent(SDL_Event * e) = 0;
	virtual void OnLoop() = 0;
	virtual void OnRender() = 0;
	virtual void OnCleanup() = 0;

	bool ErrorLogger(const char * message); // It always returns false
public:
	SDLMain(const char * windowTitle, int windowWidth, int windowHeight, bool fullscreen = false);
	virtual ~SDLMain();

	int Run();
};

