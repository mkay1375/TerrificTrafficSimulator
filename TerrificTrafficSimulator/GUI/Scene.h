#pragma once
#include "../globalization.h"

enum _Scene {
	SCN_GREETING,
	SCN_SELECTINGMAP,
	SCN_SIMULATOR
};
class Scene
{
protected:
	SDL_Renderer * mainRen;
	_Scene * currScene;
	int begTime;
public:

	void SetBegTime(int time);
	virtual bool OnInit() = 0;
	virtual void OnEvent(SDL_Event * e) = 0;
	virtual void OnLoop(const float & dt) = 0;
	virtual void OnRender() = 0;
	virtual void OnCleanup() = 0;

	Scene();
	~Scene();
};

