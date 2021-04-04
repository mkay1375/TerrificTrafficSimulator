#include "TTS.h"



bool TTS::OnInit()
{
	fillcolor = new Image("files/other/fillcolor.png", mainRenderer);
	scenes[0] = new SCN_Greeting(mainRenderer, &currScene);
	scenes[0]->OnInit();
	scenes[2] = new SCN_Simulator(mainRenderer, new Map("files/maps/2nd map", mainRenderer));
	scenes[2]->OnInit();
	scenes[0]->SetBegTime(SDL_GetTicks());

	return true;
}

void TTS::OnEvent(SDL_Event * e)
{
	if (e->type == SDL_QUIT)
		Exit();
	scenes[currScene]->OnEvent(e);
}

void TTS::OnLoop()
{
	scenes[currScene]->OnLoop(dt);
}

void TTS::OnRender()
{
#pragma region BEGIN
	SDL_SetRenderDrawColor(mainRenderer, 0, 0, 0, 255);
	SDL_RenderClear(mainRenderer);
	fillcolor->render(0, 0);
#pragma endregion

	scenes[currScene]->OnRender();

#pragma region END
	SDL_RenderPresent(mainRenderer);
#pragma endregion
}

void TTS::OnCleanup()
{
	delete fillcolor;
}

TTS::TTS() : SDLMain("Teriffic Traffic Simulator", WIN_WIDTH, WIN_HEIGHT)
{
}


TTS::~TTS()
{
}
