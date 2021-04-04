#pragma once
#include "globalization.h"
#include "Map\Map.h"
#include "CoordinateSystem.hpp"

class Car
{
private:
	char name[51];
	long int id;
	Image * img;
	Map * map;
	int mapblocksize = 0;
	int mapwidth = 0;
	int mapheight = 0;
	const MapBlock * currBlock = 0;

	CoordinateSystem<Car> * CS = 0;

	float maxAcceleration;
	float maxDeceleration;
	float maxVelocity;
	float velocity;
	float x, y;
	Direction dir;
	float angle;
	bool lane;

	const MapBlock * FindNextBlock();
	float max_s_0_car = 2.5;
	float min_s_0_car = 1.0;
	float s_0_car;
	Car * FindNextCar();
	float ToNextCarCoef();
	bool isSquareBusy(const MapBlock * blk);
	float ToNextSquareCoef();
	float Acceleration();
	void setVelocity(float v);

	void MoveStraight(float & step);
	bool movingstraight = false;
	bool turningLeft = false;
	bool turningRight = false;
	float turningRadius;
	int cx; // Center of Rotation in turns
	int cy; // Center of Rotation in turns
	void TurnLeft(float & dt, const MapBlock * block);
	void TurnRight(float & dt, const MapBlock * block);
	// Notice! The direction will be changed (in turns) after the turn.
public:
	Car(char * name, float maxAcceleraiton, float maxDeceleration, float maxVelocity, Image & img, Map & map, CoordinateSystem<Car> & CS); // For creating car samples
	Car(const Car & car, float x0, float y0, float v0, Direction dir0, bool lane0);
	Car(const Car & sampleCar, MapBlock * entrance, bool lane);
	~Car();

	void Move(float & dt);
	void Draw();
	long int GetID();
	int GetX();
	int GetY();
	Direction GetDir();
};

