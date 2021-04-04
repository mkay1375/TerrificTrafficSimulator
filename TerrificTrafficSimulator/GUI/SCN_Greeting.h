#pragma once
#include "Scene.h"
class SCN_Greeting : public Scene
{
	Mix_Chunk * engine = 0;
	Image * logo;
	float imageOpacity = 0;
	
public:
	bool OnInit();
	void OnEvent(SDL_Event * e);
	void OnLoop(const float & dt);
	void OnRender();
	void OnCleanup();

	SCN_Greeting(SDL_Renderer * mainRen, _Scene * currScene);
	~SCN_Greeting();
};

