#pragma once
#include "globalization.h"
#include "Car.h"
#include "Map\Map.h"
#include "CoordinateSystem.hpp"
#include "CarHandler.h"

class Simulator
{
private:

#pragma region BASICS
	SDL_Renderer * mainRen;

	bool pause = false;
	bool speedup = false;
	bool speeddown = false;

	void LoadSampleCars();
#pragma endregion Every thing necessary is here!


	std::vector<Image> carImgs;
	std::vector<Car> sampleCars;
	CarHandler * carhdlr;
	Map * map;
	int mapwidth, mapheight;
	CoordinateSystem<Car> * cs;
	
	Simulator();
public:
	Simulator(SDL_Renderer * mainRen, Map * map);
	~Simulator();

	bool OnInit();
	void OnEvent(SDL_Event * e);
	void OnLoop(const float & dt);
	void OnRender();
	void OnCleanup();

};

