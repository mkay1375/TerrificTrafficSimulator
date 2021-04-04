#include "SCN_Greeting.h"



bool SCN_Greeting::OnInit()
{
	engine = Mix_LoadWAV("files/sounds/engine.wav");
	logo = new Image("files/other/logo.png", mainRen);
	logo->SetAlpha(0);
	
	return true;
}

void SCN_Greeting::OnEvent(SDL_Event * e)
{
}

void SCN_Greeting::OnLoop(const float & dt)
{
	static bool firsttime = true;
	if (firsttime)
	{
		Mix_PlayChannel(-1, engine, 0);
		firsttime = false;
	}
	static int TimeLine;
	TimeLine += dt * 1000;
	if (0 <= TimeLine && TimeLine <= 1500)
	{
		if (imageOpacity < 255)
			imageOpacity += dt * 255.0;
		if (imageOpacity > 255)
			imageOpacity = 255;
	}
	else if (1500 < TimeLine && TimeLine <= 7000)
	{
		return;
	}
	else if (7000 < TimeLine && TimeLine <= 9000)
	{
		if (imageOpacity > 0)
		imageOpacity -= dt * 122.5;
		if (imageOpacity < 0)
			imageOpacity = 0;
	}
	else
	{
		*currScene = SCN_SIMULATOR;
	}
}

void SCN_Greeting::OnRender()
{
	logo->SetAlpha(imageOpacity);
	logo->render(0, 0);
}



void SCN_Greeting::OnCleanup()
{
	delete logo;
	Mix_FreeChunk(engine);
}

SCN_Greeting::SCN_Greeting(SDL_Renderer * mainRen, _Scene * currScene)
{
	this->mainRen = mainRen;
	this->currScene = currScene;
}

SCN_Greeting::~SCN_Greeting()
{
}
