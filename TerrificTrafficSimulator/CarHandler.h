#pragma once
#include "globalization.h"
#include "Car.h"
#include "Map\Map.h"
#include "CoordinateSystem.hpp"

struct Entrance {
	MapBlock * entrance;
	Car * car[2];
};

class CarHandler
{
	std::vector<Car *> cars;
	std::vector<Car> * sampleCars;
	Map * map;
	int mapwidth, mapheight;
	CoordinateSystem<Car> * cs;
	int iMaxAllowedCars;
	int iMinAllowedCars;
	int iRequiredCars;
	int iCurrentCars = 0;

	int OffArea = 50;

	std::vector<Entrance> entrances;
	void AddCar();
	void RemoveCar(int i);
	void EntranceUpdate();
public:
	CarHandler(int iMaxAllowedCars, int iMinAllowedCars, int iRequiredCars, Map * map, std::vector<Car> * sampleCars, CoordinateSystem<Car> * cs);
	~CarHandler();

	void Update(float & dt);
	void Draw();
	void SetRequiredCars(int Required);
	int GetRequiredCars();
};

