#include "SCN_Simulator.h"


bool SCN_Simulator::OnInit()
{
	music = Mix_LoadMUS("files/sounds/music.ogg");
	fillcolor = new Image("files/other/fillcolor.png", mainRen);
	return (sim = new Simulator(mainRen, map))->OnInit();
}

void SCN_Simulator::OnEvent(SDL_Event * e)
{
	sim->OnEvent(e);
}

void SCN_Simulator::OnLoop(const float & dt)
{
	if (!Mix_PlayingMusic())
		Mix_PlayMusic(music, 0);
	sim->OnLoop(dt);
	if (op > 0)
		op -= dt * 100;
}

void SCN_Simulator::OnRender()
{
	sim->OnRender();
	if (op > 0)
	{
		fillcolor->SetAlpha(op);
		fillcolor->render(0, 0);
	}
}

void SCN_Simulator::OnCleanup()
{
	sim->OnCleanup();
	delete fillcolor;
}

SCN_Simulator::SCN_Simulator(SDL_Renderer * mainRen, Map * map)
{
	this->mainRen = mainRen;
	this->map = map;
}

SCN_Simulator::~SCN_Simulator()
{
}
