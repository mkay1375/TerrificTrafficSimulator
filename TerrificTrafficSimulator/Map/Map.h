#pragma once
#include "../globalization.h"
#include "MapBlock.h"
#include "Texture.h"

class Map
{
	friend class CarHandler;
	char name[100];

	SDL_Renderer * ren = 0;

	Image * background = 0;
	Image * road = 0;
	Image * square4ent = 0; // -|-
	Image * square3ent = 0; //		|
						    //	  -----
	Image * square2ent = 0; //			L
	std::vector<Texture> textures; // Apart form textures(images) defined above.
	bool valid;

	const int BLOCK_SIZE = 120; // px
	const int BLROWS = 6;
	const int BLCOLUMNS = 11;
	MapBlock blocks[6][11];
	std::vector<MapBlock *> entrances;
	char cmap[6][11 + 2]; // map was blocks of 6 rows and 11 columns
	

	bool loadImages(char * path);
	bool loadBlocks(char * path);

public:
	Map(char * path, SDL_Renderer * ren);
	Map(const Map & map);
	~Map();

	int GetNumberofRows() const;
	int GetNumberofColumns() const;
	int GetBlockSize() const;
	const MapBlock * GetBlock(int x, int y) const;
	MapBlock * GetRandomEntrance() const;

	bool IsValid() const;
	void Draw(int x = 0, int y = 0, float scale = 1.0) const;
	void Update(const float & dt);
};

