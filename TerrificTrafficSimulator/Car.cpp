#include "Car.h"

const MapBlock * Car::FindNextBlock()
{
	switch (dir)
	{
	case DIR_UP:
		return map->GetBlock(x, y - mapblocksize);
		break;
	case DIR_RIGHT:
		return map->GetBlock(x + mapblocksize, y);
		break;
	case DIR_DOWN:
		return map->GetBlock(x, y + mapblocksize);
		break;
	case DIR_LEFT:
		return map->GetBlock(x - mapblocksize, y);
		break;
	default:
		break;
	}
}

Car * Car::FindNextCar()
{
	static Car * nextCar; nextCar = 0;
	static float X;	X = x;
	static float Y;	Y = y;

	static float radian;
	static float tempAngle; tempAngle = angle;
	static int i;
	i = 0;


	static Image de("files/debug/redsquare.png", img->GetRenderer());

	//static const MapBlock * nextblk;
	//nextblk = FindNextBlock();

	while (
		-50 <= X && X < mapwidth + 50 &&
		-50 <= Y && Y < mapheight + 50 &&
		i < 60
		)
	{
		// 'angle < dir * 90 + 60' : this part is for preventing checking the next car in circular path when the turn is almost done.
		if (turningRight && tempAngle < dir * 90 + 90)// ||
			//(nextblk && BLK_2SQUARE0 <= nextblk->getBlocktype() && nextblk->getBlocktype() <= BLK_2SQUARE3 && (abs(nextblk->GetBlockX() - x) < 20 || abs(nextblk->GetBlockY() - y) < 20)))
		{
			tempAngle += 90.0 / 60.0;
			radian = ((tempAngle)* Pi) / 180.0;
			X = cx - cos(radian) * (turningRadius);
			Y = cy - sin(radian) * (turningRadius);
			if ((nextCar = CS->GetThing(X, Y)))
				return nextCar;
		}
		else if (turningLeft && tempAngle > dir * 90 - 90)
		{
			tempAngle -= 90.0 / 60.0;
			radian = ((tempAngle)* Pi) / 180.0;
			X = cx + cos(radian) * turningRadius;
			Y = cy + sin(radian) * turningRadius;
			if ((nextCar = CS->GetThing(X, Y)))
				return nextCar;
		}
		else
		{
			radian = ((tempAngle - 90) * Pi) / 180.0;
			if (i > 10)
			{
				X += cos(radian) * 5;
				Y += sin(radian) * 5;
			}
			else
			{
				X += cos(radian) * 1;
				Y += sin(radian) * 1;
			}
			if ((nextCar = CS->GetThing(X, Y)))
				return nextCar;
			if ((nextCar = CS->GetThing(X - abs(sin(radian) * img->W() / 2 - 1), Y - abs(cos(radian) * img->W() / 2 - 1))))
				return nextCar;
			if ((nextCar = CS->GetThing(X + abs(sin(radian) * img->W() / 2 - 1), Y + abs(cos(radian) * img->W() / 2 - 1))))
				return nextCar;
		}


		//de.render(X, Y);
		i++;
	}
	return nextCar;
}

float Car::ToNextCarCoef()
{
	static float s_alpha; // Distance to next car;
	static float s_0; s_0 = s_0_car;
	static float T = 1.0;
	static float v_alphaMinus1;
	static float delta_v;
	static Car * nextCar;

	s_alpha = 0;
	nextCar = 0;

	nextCar = FindNextCar();
	if (nextCar)// && dir == nextCar->dir && lane == nextCar->lane)
	{
		s_alpha = sqrt(pow(x - nextCar->GetX(), 2) + pow(y - nextCar->GetY(), 2));
		s_alpha = s_alpha - img->H() / 2 - nextCar->img->H() / 2;
	}
	else
		return 0;

	s_alpha /= 10.0;

	if (!nextCar)
		return 0;
	if (s_alpha < 0.0)
		s_alpha = 0;

	if (dir == nextCar->dir && lane != nextCar->lane)
		return 0;

	static const MapBlock * nextblk;
	nextblk = FindNextBlock();
	if ((nextblk && BLK_2SQUARE0 <= nextblk->getBlocktype() && nextblk->getBlocktype() <= BLK_2SQUARE3) &&
		dir != nextCar->dir)
		return 0;
	/*if ((turningRight && currBlock &&
		(dir + 6 == currBlock->getBlocktype() || (currBlock->getBlocktype() == BLK_2SQUARE0 && dir == DIR_LEFT))
		&& BLK_2SQUARE0 <= currBlock->getBlocktype() && currBlock->getBlocktype() <= BLK_2SQUARE3) &&
		dir != nextCar->dir)
		return 0;
*/
	v_alphaMinus1 = nextCar->velocity;

	//if (s_alpha <= 1 && v_alphaMinus1 != 0)
		//return 0;

	delta_v = velocity - v_alphaMinus1;
	return (s_0 + velocity * T + (velocity * (delta_v)) / (2 * sqrt(maxAcceleration * maxDeceleration))) / s_alpha;
}

bool Car::isSquareBusy(const MapBlock * blk)
{
	if (!blk)
		return false;
	if (blk->getBlocktype() < BLK_3SQUARE0)
		return false;
	if (blk->getBlocktype() == BLK_3SQUARE0 && dir == DIR_RIGHT)
		return false;
	if (blk->getBlocktype() == BLK_3SQUARE1 && dir == DIR_DOWN)
		return false;
	if (blk->getBlocktype() == BLK_3SQUARE2 && dir == DIR_LEFT)
		return false;
	if (blk->getBlocktype() == BLK_3SQUARE3 && dir == DIR_UP)
		return false;

	static int X;
	static int Y;
	static int BX;
	static int BY;
	static Car * car;
	car = 0;
	BX = blk->GetBlockX();
	BY = blk->GetBlockY();

	for (X = 10; X < mapblocksize; X += 15)
		for (Y = 10; Y < mapblocksize; Y += 15)
		{
			car = CS->GetThing(X + BX, Y + BY);
			if (car)
			{
				if (blk->getBlocktype() == BLK_4SQUARE && abs(car->dir - dir) % 2 != 0)
					return true;
				else if (
					blk->getBlocktype() == BLK_3SQUARE0 &&
					((dir == DIR_DOWN && car->dir == DIR_LEFT) || (dir == DIR_LEFT && car->dir == DIR_DOWN))
					)
					return true;
				else if (
					blk->getBlocktype() == BLK_3SQUARE1 &&
					((dir == DIR_LEFT && car->dir == DIR_UP) || (dir == DIR_UP && car->dir == DIR_LEFT))
					)
					return true;
				else if (
					blk->getBlocktype() == BLK_3SQUARE2 &&
					((dir == DIR_UP && car->dir == DIR_RIGHT) || (dir == DIR_RIGHT && car->dir == DIR_UP))
					)
					return true;
				else if (
					blk->getBlocktype() == BLK_3SQUARE3 &&
					((dir == DIR_RIGHT && car->dir == DIR_DOWN) || (dir == DIR_DOWN && car->dir == DIR_RIGHT))
					)
					return true;

			}
		}
	return false;
}

float Car::ToNextSquareCoef()
{
	static float s_alpha; // Distance to next Square;
	static float s_0 = 0.2;
	static float T = 0.5;
	static float v_alphaMinus1;
	static float delta_v;
	static float nextSquareXorY;
	static int tempCounter;

	s_alpha = 0;
	if (map->GetBlock(x, y)->getBlocktype() >= BLK_2SQUARE0)
		return 0;

	static const MapBlock * Square;

	switch (dir)
	{
	case DIR_UP:
		for (tempCounter = y; tempCounter > 0; tempCounter -= mapblocksize)
		{
			if ((Square = map->GetBlock(x, tempCounter))->getBlocktype() >= BLK_2SQUARE0)
			{
				s_alpha = y - img->H() / 2 - ((tempCounter / mapblocksize) * mapblocksize + mapblocksize);
				break;
			}
		}
		break;
	case DIR_RIGHT:
		for (tempCounter = x; tempCounter < mapwidth; tempCounter += mapblocksize)
		{
			if ((Square = map->GetBlock(tempCounter, y))->getBlocktype() >= BLK_2SQUARE0)
			{
				s_alpha = (tempCounter / mapblocksize) * mapblocksize - x - img->H() / 2;
				break;
			}
		}
		break;
	case DIR_DOWN:
		for (tempCounter = y; tempCounter < mapheight; tempCounter += mapblocksize)
		{
			if ((Square = map->GetBlock(x, tempCounter))->getBlocktype() >= BLK_2SQUARE0)
			{
				s_alpha = (tempCounter / mapblocksize) * mapblocksize - y - img->H() / 2;
				break;
			}
		}
		break;
	case DIR_LEFT:
		for (tempCounter = x; tempCounter > 0; tempCounter -= mapblocksize)
		{
			if ((Square = map->GetBlock(tempCounter, y))->getBlocktype() >= BLK_2SQUARE0)
			{
				s_alpha = x - img->H() / 2 - ((tempCounter / mapblocksize) * mapblocksize + mapblocksize);
				break;
			}
		}
		break;
	default:
		break;
	}
	s_alpha /= 10.0;

	if ((Square->getBlocktype() == BLK_3SQUARE0 && dir == DIR_RIGHT) ||
		(Square->getBlocktype() == BLK_3SQUARE1 && dir == DIR_DOWN) ||
		(Square->getBlocktype() == BLK_3SQUARE2 && dir == DIR_LEFT) ||
		(Square->getBlocktype() == BLK_3SQUARE3 && dir == DIR_UP))
		return 0;
	if (
		((Square->TrLightStatus() == dir || abs(Square->TrLightStatus() - dir) == 2) && !isSquareBusy(Square)) ||
		Square->getBlocktype() < BLK_3SQUARE0 || x <= 5 || x >= mapwidth - 5 || y <= 5 || y >= mapheight - 5 
		)
		v_alphaMinus1 = 7;
	else
		v_alphaMinus1 = 0;

	if (s_alpha <= 1 && v_alphaMinus1 != 0)
		return 0;

	delta_v = velocity - v_alphaMinus1;
	return (s_0 + velocity * T + (velocity * (delta_v)) / (2 * sqrt(maxAcceleration * maxDeceleration))) / s_alpha;
}

float Car::Acceleration()
{
	static float result;
	result = maxAcceleration * (1 - pow((velocity / maxVelocity), 4) - pow(fmaxf(ToNextCarCoef(), ToNextSquareCoef()), 2));
	//if (result > maxAcceleration)
		//return maxAcceleration;
	//if (result < -maxDeceleration)
		//return -maxDeceleration;
	return result;
}

void Car::setVelocity(float v)
{
	velocity = v;
	if (velocity < 0)
		velocity = 0;
	else if (velocity > maxVelocity)
		velocity = maxVelocity;
}

void Car::MoveStraight(float & step)
{
	if (step < 0)
		return;
	switch (dir)
	{
	case DIR_UP:
		y -= step;
		break;
	case DIR_RIGHT:
		x += step;
		break;
	case DIR_DOWN:
		y += step;
		break;
	case DIR_LEFT:
		x -= step;
		break;
	default:
		break;
	}
}

void Car::TurnLeft(float & dt, const MapBlock * block)
{
	static float t;
	static float r;
	if (!turningLeft)
	{
		switch (dir)
		{
		case DIR_UP:
			cx = block->GetBlockX();
			cy = block->GetBlockY() + mapblocksize;
			break;
		case DIR_RIGHT:
			cx = block->GetBlockX();
			cy = block->GetBlockY();
			break;
		case DIR_DOWN:
			cx = block->GetBlockX() + mapblocksize;
			cy = block->GetBlockY();
			break;
		case DIR_LEFT:
			cx = block->GetBlockX() + mapblocksize;
			cy = block->GetBlockY() + mapblocksize;
			break;
		default:
			break;
		}
		if (lane == 0)	// Lane one
			turningRadius = 97.0;
		else			// Lane rwo
			turningRadius = 73.0;
	}

	r = turningRadius;
	turningLeft = true;
	angle -= velocity * dt / r * 500;
	t = angle * Pi / 180.0;
	x = cx + cos(t) * r;
	y = cy + sin(t) * r;

	static const MapBlock * nxtblk;
	nxtblk = FindNextBlock();
	if (map->GetBlock(x, y)->getBlocktype() < BLK_2SQUARE0 && angle < dir * 90 - 90)//nxtblk && nxtblk->getBlocktype() < BLK_2SQUARE0)
	{
		x = round(x);
		y = round(y);

		switch (dir)
		{
		case DIR_UP:
			dir = DIR_LEFT;
			break;
		case DIR_RIGHT:
			dir = DIR_UP;
			break;
		case DIR_DOWN:
			dir = DIR_RIGHT;
			break;
		case DIR_LEFT:
			dir = DIR_DOWN;
			break;
		default:
			break;
		}

		switch (dir)
		{
		case DIR_LEFT:
			x = cx - 1;
			break;
		case DIR_UP:
			y = cy - 1;
			break;
		case DIR_RIGHT:
			x = cx + 1;
			break;
		case DIR_DOWN:
			y = cy + 1;
			break;
		default:
			break;
		}

		angle = dir * 90;
		turningLeft = false;
	}
}
void Car::TurnRight(float & dt, const MapBlock * block)
{
	static float t;
	static float r;
	if (!turningRight)
	{
		if (block->getBlocktype() < BLK_2SQUARE0)
			return;
		switch (dir)
		{
		case DIR_UP:
			cx = block->GetBlockX() + mapblocksize;
			cy = block->GetBlockY() + mapblocksize;
			break;
		case DIR_RIGHT:
			cx = block->GetBlockX();
			cy = block->GetBlockY() + mapblocksize;
			break;
		case DIR_DOWN:
			cx = block->GetBlockX();
			cy = block->GetBlockY();
			break;
		case DIR_LEFT:
			cx = block->GetBlockX() + mapblocksize;
			cy = block->GetBlockY();
			break;
		default:
			break;
		}

		if (lane == 0)	// Lane one
			turningRadius = 22.0;
		else			// Lane rwo
			turningRadius = 46.0;
	}
	r = turningRadius;
	turningRight = true;
	angle += velocity * dt / r * 500;
	t = angle * Pi / 180.0;
	x = cx - cos(t) * r;
	y = cy - sin(t) * r;

	static const MapBlock * nxtblk;
	nxtblk = FindNextBlock();
	if (map->GetBlock(x, y)->getBlocktype() < BLK_2SQUARE0 && angle > dir * 90 + 90)//nxtblk && nxtblk->getBlocktype() < BLK_2SQUARE0)
	{
		x = round(x);
		y = round(y);

		switch (dir)
		{
		case DIR_UP:
			dir = DIR_RIGHT;
			break;
		case DIR_RIGHT:
			dir = DIR_DOWN;
			break;
		case DIR_DOWN:
			dir = DIR_LEFT;
			break;
		case DIR_LEFT:
			dir = DIR_UP;
			break;
		default:
			break;
		}
		switch (dir)
		{
		case DIR_RIGHT:
			x = cx + 1;
			break;
		case DIR_DOWN:
			y = cy + 1;
			break;
		case DIR_LEFT:
			x = cx - 1;
			break;
		case DIR_UP:
			y = cy - 1;
			break;
		default:
			break;
		}

		angle = dir * 90;
		turningRight = false;
	}
}

Car::Car(char * name, float maxAcceleraiton, float maxDeceleration, float maxVelocity, Image & img, Map & map, CoordinateSystem<Car> & CS)
	: img(&img), map(&map), CS(&CS)
{

	strcpy(this->name, name);
	this->name[50] = '\0';
	this->maxAcceleration = maxAcceleraiton;
	this->maxDeceleration = maxDeceleration;
	this->maxVelocity = maxVelocity;
	this->mapblocksize = map.GetBlockSize();
	this->mapwidth = map.GetNumberofColumns() * mapblocksize;
	this->mapheight = map.GetNumberofRows() * mapblocksize;

}

static long int counter;

Car::Car(const Car & car, float x0, float y0, float v0, Direction dir0, bool lane0)
	: img(car.img), map(car.map), CS(car.CS)
{
	id = counter;
	counter++;

	strcpy(name, car.name);
	maxAcceleration = car.maxAcceleration;
	maxDeceleration = car.maxDeceleration;
	maxVelocity = car.maxVelocity;
	mapblocksize = car.mapblocksize;
	mapwidth = car.mapwidth;
	mapheight = car.mapheight;
	x = x0;
	y = y0;
	velocity = v0;
	dir = dir0;
	angle = dir * 90;
	lane = lane0;
}

Car::Car(const Car & sampleCar, MapBlock * entrance, bool lane) // Creating car with given entrance
	: img(sampleCar.img), map(sampleCar.map), CS(sampleCar.CS)
{
	id = counter;
	counter++;

	strcpy(name, sampleCar.name);
	maxAcceleration = sampleCar.maxAcceleration;
	maxDeceleration = sampleCar.maxDeceleration;
	maxVelocity = sampleCar.maxVelocity;
	mapblocksize = sampleCar.mapblocksize;
	mapwidth = sampleCar.mapwidth;
	mapheight = sampleCar.mapheight;
	this->lane = lane;

	velocity = rand() % (int)maxVelocity;
	s_0_car = rand() % (int)((max_s_0_car - min_s_0_car) * 1000.0) + min_s_0_car * 1000.0;
	s_0_car /= 1000.0;
	if (entrance->getBlocktype() == BLK_HROAD)
		if (entrance->GetBlockX() == 0)
			dir = DIR_RIGHT;
		else
			dir = DIR_LEFT;
	else
		if (entrance->GetBlockY() == 0)
			dir = DIR_DOWN;
		else
			dir = DIR_UP;

	angle = dir * 90;
	switch (dir)
	{
	case DIR_UP:
		y = mapheight + 40;
		x = entrance->AvailabeXorY(lane, dir);
		break;
	case DIR_LEFT:
		y = entrance->AvailabeXorY(lane, dir);
		x = mapwidth + 40;
		break;
	case DIR_DOWN:
		y = -40;
		x = entrance->AvailabeXorY(lane, dir);
		break;
	case DIR_RIGHT:
		y = entrance->AvailabeXorY(lane, dir);
		x = -40;
		break;
	default:
		break;
	}
}

Car::~Car()
{
}

void Car::Move(float & dt)
{
	CS->ClearRectOnSurface(x, y, img->W(), img->H(), angle);
	static int moveStraightPossibilityIn3Sqrs = 80; // percent

	static int randnum;
	randnum = rand() % 100;

	static float acceleration;
	acceleration = Acceleration();
	setVelocity(acceleration * dt + velocity);
	// Each 10 px = 1 meter //

	static const MapBlock * block;
	block = map->GetBlock(x, y);
	currBlock = block;

	static float step;
	if (turningLeft)
		TurnLeft(dt, block);
	else if (turningRight)
		TurnRight(dt, block);
	else if (movingstraight && block->getBlocktype() >= BLK_3SQUARE0)
	{
		step = 10 * (velocity * dt + 0.5 * acceleration * dt * dt);
		MoveStraight(step);
	}
	else
	{
		movingstraight = false;
		static BlockType btype;
		btype = block->getBlocktype();

		step = 10 * (velocity * dt + 0.5 * acceleration * dt * dt);

		if (btype == BLK_VROAD)
			MoveStraight(step);
		else if (btype == BLK_HROAD)
			MoveStraight(step);
		else if (btype == BLK_2SQUARE0)
			if (dir == DIR_DOWN)
				TurnLeft(dt, block);
			else
				TurnRight(dt, block);
		else if (btype == BLK_2SQUARE1)
			if (dir == DIR_LEFT)
				TurnLeft(dt, block);
			else
				TurnRight(dt, block);
		else if (btype == BLK_2SQUARE2)
			if (dir == DIR_UP)
				TurnLeft(dt, block);
			else
				TurnRight(dt, block);
		else if (btype == BLK_2SQUARE3)
			if (dir == DIR_RIGHT)
				TurnLeft(dt, block);
			else
				TurnRight(dt, block);
		else if (btype == BLK_3SQUARE0)
		{
			if (dir == DIR_RIGHT)
			{
				//if (randnum > moveStraightPossibilityIn3Sqrs && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				//{
				movingstraight = true;
				MoveStraight(step);
				//}
			}
			else if (dir == DIR_DOWN)
			{
				//if (randnum > 50 && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				TurnRight(dt, block);
			}
			else if (dir == DIR_LEFT)
			{
				if (randnum > moveStraightPossibilityIn3Sqrs && lane == 0)
					TurnRight(dt, block);
				else
				{
					movingstraight = true;
					MoveStraight(step);
				}
			}
		}
		else if (btype == BLK_3SQUARE1)
		{
			if (dir == DIR_DOWN)
			{
				//if (randnum > moveStraightPossibilityIn3Sqrs && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				//{
				movingstraight = true;
				MoveStraight(step);
				//}
			}
			else if (dir == DIR_LEFT)
			{
				//if (randnum > 50 && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				TurnRight(dt, block);
			}
			else if (dir == DIR_UP)
			{
				if (randnum > moveStraightPossibilityIn3Sqrs && lane == 0)
					TurnRight(dt, block);
				else
				{
					movingstraight = true;
					MoveStraight(step);
				}
			}
		}
		else if (btype == BLK_3SQUARE2)
		{
			if (dir == DIR_LEFT)
			{
				//if (randnum > moveStraightPossibilityIn3Sqrs && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				//{
				movingstraight = true;
				MoveStraight(step);
				//}
			}
			else if (dir == DIR_UP)
			{
				//if (randnum > 50 && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				TurnRight(dt, block);
			}
			else if (dir == DIR_RIGHT)
			{
				if (randnum > moveStraightPossibilityIn3Sqrs && lane == 0)
					TurnRight(dt, block);
				else
				{
					movingstraight = true;
					MoveStraight(step);
				}
			}
		}
		else if (btype == BLK_3SQUARE3)
		{
			if (dir == DIR_UP)
			{
				//if (randnum > moveStraightPossibilityIn3Sqrs && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				//{
				movingstraight = true;
				MoveStraight(step);
				//}
			}
			else if (dir == DIR_RIGHT)
			{
				//if (randnum > 50 && lane == 1 && 0) // NEVER HAPPEN
				//	TurnLeft(dt, block);
				//else
				TurnRight(dt, block);
			}
			else if (dir == DIR_DOWN)
			{
				if (randnum > moveStraightPossibilityIn3Sqrs && lane == 0)
					TurnRight(dt, block);
				else
				{
					movingstraight = true;
					MoveStraight(step);
				}
			}
		}
		else if (btype == BLK_4SQUARE)
		{
			//if (randnum < 33 && lane == 1 && 0)
			//	TurnLeft(dt, block);
			//else
			if (randnum < 60 && lane == 0)
				TurnRight(dt, block);
			else
			{
				movingstraight = true;
				MoveStraight(step);
			}
		}
		else
			MoveStraight(step);
	}

	CS->SetRectOnSurface(this, x, y, img->W(), img->H(), angle);
}

void Car::Draw()
{
	img->render(round(x) - img->W() / 2, round(y) - img->H() / 2, angle);
}

long int Car::GetID()
{
	return id;
}

int Car::GetX()
{
	return x;
}

int Car::GetY()
{
	return y;
}

Direction Car::GetDir()
{
	return dir;
}

