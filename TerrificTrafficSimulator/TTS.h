#pragma once
#include "SDLMain.h"
#include "Simulator.h"
#include "GUI/Scene.h"
#include "GUI/SCN_Greeting.h"
#include "GUI/SCN_Simulator.h"


class TTS : public SDLMain
{
private:
	_Scene currScene = SCN_GREETING;
	Scene * scenes[3];
	Image * fillcolor;

	bool OnInit();
	void OnEvent(SDL_Event * e);
	void OnLoop();
	void OnRender();
	void OnCleanup();
public:
	TTS();
	~TTS();
};

