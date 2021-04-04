#pragma once
#include "Scene.h"
#include "../Simulator.h"

class SCN_Simulator : public Scene
{
	Simulator * sim;
	Map * map;
	Image * fillcolor;
	float op = 255;

	Mix_Music * music;
public:
	bool OnInit();
	void OnEvent(SDL_Event * e);
	void OnLoop(const float & dt);
	void OnRender();
	void OnCleanup();

	SCN_Simulator(SDL_Renderer * mainRen, Map * map);
	~SCN_Simulator();
};

