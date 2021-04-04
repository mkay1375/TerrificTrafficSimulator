#include "Simulator.h"

using namespace SDLHelper;

void Simulator::LoadSampleCars()
{
	char carspecpath[30];
	char carimgpath[30];
	carImgs.reserve(MAX_SAMPLECARS);
	sampleCars.reserve(MAX_SAMPLECARS);
	for (int i = 1; i < MAX_SAMPLECARS; i++)
	{
		sprintf(carspecpath, "files/cars/car%d.car", i);
		sprintf(carimgpath, "files/cars/car%d.png", i);
		char * text = 0;
		text = file_read(carspecpath);
		if (text == NULL)
			return;
		float a, d, v;
		sscanf(text, "maxAccel:%f;maxDeccel:%f;maxV:%f", &a, &d, &v);
		carImgs.push_back(Image(carimgpath, mainRen));
		sampleCars.push_back(Car("myCar", a, d, v, carImgs[i - 1], *map, *cs));
	}
}

bool Simulator::OnInit()
{
	mapwidth = map->GetBlockSize() * map->GetNumberofColumns();
	mapheight = map->GetBlockSize() * map->GetNumberofRows();
	cs = new CoordinateSystem<Car>(mapwidth, mapheight);

	LoadSampleCars();
	carhdlr = new CarHandler(200, 1, 50, map, &sampleCars, cs);

	return true;
}

void Simulator::OnEvent(SDL_Event * e)
{
	if (e->key.keysym.sym == SDLK_UP)
		carhdlr->SetRequiredCars(carhdlr->GetRequiredCars() + 1);
	else if (e->key.keysym.sym == SDLK_DOWN)
		carhdlr->SetRequiredCars(carhdlr->GetRequiredCars() - 1);
	else if (e->key.keysym.sym == SDLK_c)
		SDL_RenderClear(mainRen);
	else if (e->key.keysym.sym == SDLK_RIGHT)
		speedup = true;
	else if (e->key.keysym.sym == SDLK_LEFT)
		speedup = false;
	else if (e->key.keysym.sym == SDLK_o)
		pause = false;
	else if (e->key.keysym.sym == SDLK_p)
		pause = true;
	else if (e->key.keysym.sym == SDLK_l)
		speeddown = true;
	else if (e->key.keysym.sym == SDLK_k)
		speeddown = false;
}

void Simulator::OnLoop(const float & dt)
{
	static float _dt;
	_dt = dt;
	if (_dt > 0.050)
		_dt = 0.050;
	if (_dt == 0)
		_dt = 0.001;
	if (speeddown)
		_dt /= 2.0;
	if (speedup)
		_dt *= 2.0;
	if (pause)
		_dt = 0;

	carhdlr->Update(_dt);
	map->Update(_dt);
}

void Simulator::OnRender()
{
	map->Draw();
	carhdlr->Draw();
	//cs->Draw(mainRen);
}

void Simulator::OnCleanup()
{
	delete carhdlr;
	delete cs;
	sampleCars.clear();
}

Simulator::Simulator(SDL_Renderer * mainRen, Map * map)
{
	this->mainRen = mainRen;
	this->map = map;
}

Simulator::Simulator()
{

}

Simulator::~Simulator()
{
	
}



