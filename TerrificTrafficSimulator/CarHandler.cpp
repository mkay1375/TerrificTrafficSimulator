#include "CarHandler.h"


void CarHandler::AddCar()
{
	static int randEntI;
	randEntI = rand() % entrances.size();
	static int en_size;
	en_size = entrances.size();
	static int i;
	for (i = 0; i < en_size; i++)
	{
		static bool lane;
		lane = rand() % 2;
		if (entrances[randEntI].car[lane] != 0)
			lane = !lane;
		if (entrances[randEntI].car[lane] == 0)
		{
			cars.push_back(
				new Car(
				(*sampleCars)[rand() % (*sampleCars).size()],
					entrances[randEntI].entrance,
					lane
				)
			);
			entrances[randEntI].car[lane] = cars[iCurrentCars];
			iCurrentCars++;
			return;
		}
		else
		{
			randEntI++;
			if (en_size == randEntI)
				randEntI = 0;
		}
	}
	return;
}

void CarHandler::RemoveCar(int i)
{
	delete cars[i];
	cars.erase(cars.begin() + i);
	iCurrentCars--;
}

void CarHandler::EntranceUpdate()
{
	static const int Off = 30;
	static int en_size;
	en_size = entrances.size();
	static int i;
	static int j;
	for (i = 0; i < en_size; i++)
	{
		for (j = 0; j < 2; j++)
		{
			if (entrances[i].car[j])
			{
				if (entrances[i].entrance->GetBlockX() == 0 && entrances[i].entrance->getBlocktype() == BLK_HROAD && entrances[i].car[j]->GetX() > Off)
					entrances[i].car[j] = 0;
				else if (entrances[i].entrance->GetBlockY() == 0 && entrances[i].entrance->getBlocktype() == BLK_VROAD && entrances[i].car[j]->GetY() > Off)
					entrances[i].car[j] = 0;
				else if (entrances[i].entrance->GetBlockX() == mapwidth - map->GetBlockSize() && entrances[i].entrance->getBlocktype() == BLK_HROAD && entrances[i].car[j]->GetX() < mapwidth - Off)
					entrances[i].car[j] = 0;
				else if (entrances[i].entrance->GetBlockY() == mapheight - map->GetBlockSize() && entrances[i].entrance->getBlocktype() == BLK_VROAD && entrances[i].car[j]->GetY() < mapheight - Off)
					entrances[i].car[j] = 0;
			}
		}
	}
}

CarHandler::CarHandler(int iMaxAllowedCars, int iMinAllowedCars, int iRequiredCars, Map * map, std::vector<Car> * sampleCars, CoordinateSystem<Car> * cs)
	: map(map), sampleCars(sampleCars), cs(cs), iMaxAllowedCars(iMaxAllowedCars), iMinAllowedCars(iMinAllowedCars)
{
	mapwidth = map->GetBlockSize() * map->GetNumberofColumns();
	mapheight = map->GetBlockSize() * map->GetNumberofRows();

	if (iRequiredCars >= iMinAllowedCars && iRequiredCars <= iMaxAllowedCars)
		this->iRequiredCars = iRequiredCars;
	else
		this->iRequiredCars = iMinAllowedCars;

	static int i;
	for (i = 0; i < map->entrances.size(); i++)
	{
		Entrance temp;
		temp.entrance = map->entrances[i];
		temp.car[0] = 0;
		temp.car[1] = 0;
		entrances.push_back(temp);
	}

	cars.reserve(iMaxAllowedCars);
}

CarHandler::~CarHandler()
{
}

void CarHandler::Update(float & dt)
{
	//printf("cur : %d , req: %d\n", iCurrentCars, iRequiredCars);
	EntranceUpdate();
	if (iCurrentCars < iRequiredCars)
	{
		AddCar();
	}
	static int i;
	for (i = 0; i < cars.size(); i++)
	{
		if (cars[i]->GetX() < -OffArea || mapwidth + OffArea < cars[i]->GetX() || cars[i]->GetY() < -OffArea || mapheight + OffArea < cars[i]->GetY())
		{
			RemoveCar(i);
			i--;
		}
		else
		{
			if (cars[i])
				cars[i]->Move(dt);
		}
	}

}

void CarHandler::Draw()
{
	static int i;
	for (i = 0; i < cars.size(); i++)
	{
		cars[i]->Draw();
	}
}

void CarHandler::SetRequiredCars(int Required)
{
	if (Required >= iMinAllowedCars && Required <= iMaxAllowedCars)
	{
		iRequiredCars = Required;
		printf("req: %d\n", Required);
	}
}

int CarHandler::GetRequiredCars()
{
	return iRequiredCars;
}
