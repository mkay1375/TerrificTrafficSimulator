#include "Map.h"
#include <string>
using namespace std;

bool Map::loadImages(char * path)
{
	string mappath = path;
	size_t nameBeg = mappath.rfind("/") + 1;
	size_t nameEnd = mappath.size();
	strcpy(name, mappath.substr(nameBeg, nameEnd).c_str()); // Setting NAME

	char * text = 0;
	text = SDLHelper::file_read((mappath + "/map.map").c_str());
	if (text == 0)
		return false;
	// map was a block of 6 row and 11 columns
	strcpy(cmap[0], strtok(text, "\n"));
	cmap[0][BLCOLUMNS + 1] = '\0';
	for (int i = 1; i < BLROWS && strcpy(cmap[i], strtok(0, "\n")); i++) // Loading File into memory
		cmap[i][BLCOLUMNS + 1] = '\0';

	// Loading Images:
	background = new Image((mappath + "/background.png").c_str(), ren);
	road = new Image((mappath + "/road.png").c_str(), ren);
	square2ent = new Image((mappath + "/sq2.png").c_str(), ren);
	square3ent = new Image((mappath + "/sq3.png").c_str(), ren);
	square4ent = new Image((mappath + "/sq4.png").c_str(), ren);

	// Loading Textures:
	for (int i = 0; i < BLROWS; i++)
	{
		for (int j = 0; j < BLCOLUMNS; j++)
		{
			if (cmap[i][j] != ' ' && cmap[i][j] != 'o' && cmap[i][j] != '|' && cmap[i][j] != '-')
			{
				bool is_this_char_exists = false;
				for (int bls = 0; bls < textures.size(); bls++)
				{
					if (textures[bls].name == cmap[i][j])
						is_this_char_exists = true;
				}
				if (!is_this_char_exists)
				{
					textures.push_back(
						Texture(
							cmap[i][j], Image(
							(mappath + "/" + cmap[i][j] + ".png").c_str(), ren
							)
						)
					);
				}
			}
		}
	}

	return true;
}

bool Map::loadBlocks(char * path)
{
	if (!loadImages(path))
		return false;

	// Loading backgrounds
	for (int i = 0; i < BLROWS; i++)
	{
		for (int j = 0; j < BLCOLUMNS; j++)
		{
			if (cmap[i][j] == ' ')
				blocks[i][j] = MapBlock(BLK_BACKGROUND, background, 0, j * BLOCK_SIZE, i * BLOCK_SIZE);
		}
	}

	// Loading Roads
	for (int i = 0; i < BLROWS; i++)
	{
		for (int j = 0; j < BLCOLUMNS; j++)
		{
			if (cmap[i][j] == '-')
			{
				blocks[i][j] = MapBlock(BLK_HROAD, road, 0, j * BLOCK_SIZE, i * BLOCK_SIZE);
				if (j == 0 || j == BLCOLUMNS - 1)
					entrances.push_back(&blocks[i][j]);
			}
			else if (cmap[i][j] == '|')
			{
				blocks[i][j] = MapBlock(BLK_VROAD, road, 90, j * BLOCK_SIZE, i * BLOCK_SIZE);
				if (i == 0 || i == BLROWS - 1)
					entrances.push_back(&blocks[i][j]);
			}
		}
	}

	// Loading Squares and turns
	for (int i = 0; i < BLROWS; i++)
	{
		for (int j = 0; j < BLCOLUMNS; j++)
		{
			if (cmap[i][j] == 'o')
			{
				char roadsAroundSquare[5] = { '0','0','0','0',0 };
				roadsAroundSquare[4] = '\0';

				// Checking Around clockwise from 12 o'clock
				// Y , X
				if (i > 0)
					if (cmap[i - 1][j] == '|')
						roadsAroundSquare[0] = '1';
				if (j + 1 < BLCOLUMNS)
					if (cmap[i][j + 1] == '-')
						roadsAroundSquare[1] = '1';
				if (i + 1 < BLROWS)
					if (cmap[i + 1][j] == '|')
						roadsAroundSquare[2] = '1';
				if (j > 0)
					if (cmap[i][j - 1] == '-')
						roadsAroundSquare[3] = '1';

				string arounds = roadsAroundSquare;

				// 4 way square
				if (arounds == "1111")
					blocks[i][j] = MapBlock(BLK_4SQUARE, square4ent, 0, j * BLOCK_SIZE, i * BLOCK_SIZE);
				// 3 way square
				else if (arounds == "1101")
					blocks[i][j] = MapBlock(BLK_3SQUARE0, square3ent, 0, j * BLOCK_SIZE, i * BLOCK_SIZE);
				else if (arounds == "1110")
					blocks[i][j] = MapBlock(BLK_3SQUARE1, square3ent, 90, j * BLOCK_SIZE, i * BLOCK_SIZE);
				else if (arounds == "0111")
					blocks[i][j] = MapBlock(BLK_3SQUARE2, square3ent, 180, j * BLOCK_SIZE, i * BLOCK_SIZE);
				else if (arounds == "1011")
					blocks[i][j] = MapBlock(BLK_3SQUARE3, square3ent, 270, j * BLOCK_SIZE, i * BLOCK_SIZE);
				// 2 way square (turn)
				else if (arounds == "1100")
					blocks[i][j] = MapBlock(BLK_2SQUARE0, square2ent, 0, j * BLOCK_SIZE, i * BLOCK_SIZE);
				else if (arounds == "0110")
					blocks[i][j] = MapBlock(BLK_2SQUARE1, square2ent, 90, j * BLOCK_SIZE, i * BLOCK_SIZE);
				else if (arounds == "0011")
					blocks[i][j] = MapBlock(BLK_2SQUARE2, square2ent, 180, j * BLOCK_SIZE, i * BLOCK_SIZE);
				else if (arounds == "1001")
					blocks[i][j] = MapBlock(BLK_2SQUARE3, square2ent, 270, j * BLOCK_SIZE, i * BLOCK_SIZE);
			}
		}
	}

	// Loading Textures;
	for (int i = 0; i < BLROWS; i++)
	{
		for (int j = 0; j < BLCOLUMNS; j++)
		{
			for (size_t txC = 0; txC < textures.size(); txC++)
			{
				if (cmap[i][j] == textures[txC].name && textures[txC].valid)
				{
					blocks[i][j] = MapBlock(BLK_TEXTURE, textures[txC].img, 0, j * BLOCK_SIZE, i * BLOCK_SIZE);
				}
			}
		}
	}

	return true;
}

Map::Map(char * path, SDL_Renderer * ren)
{
	this->ren = ren;
	valid = loadBlocks(path);
}

Map::Map(const Map & map)
{
	strcpy(name, map.name);
	ren = map.ren;

	background = new Image(*(map.background));
	road = new Image(*(map.road));
	square2ent = new Image(*(map.square2ent));
	square3ent = new Image(*(map.square3ent));
	square4ent = new Image(*(map.square4ent));
	for (size_t i = 0; i < map.textures.size(); i++)
		textures.push_back(Texture(map.textures[i]));
	for (size_t i = 0; i < BLROWS; i++)
		for (size_t j = 0; j < BLCOLUMNS; j++)
			blocks[i][j] = map.blocks[i][j];
}


Map::~Map()
{
	if (background != 0)
		delete background;
	if (road != 0)
		delete road;
	if (square2ent != 0)
		delete square2ent;
	if (square3ent != 0)
		delete square3ent;
	if (square4ent != 0)
		delete square4ent;
}

int Map::GetNumberofRows() const
{
	return BLROWS;
}

int Map::GetNumberofColumns() const
{
	return BLCOLUMNS;
}

int Map::GetBlockSize() const
{
	return BLOCK_SIZE;
}

const MapBlock * Map::GetBlock(int x, int y) const
{
	static int width = BLCOLUMNS * BLOCK_SIZE;
	static int height = BLROWS * BLOCK_SIZE;

	if (x > width || y > height || x < 0 || y < 0)
		return 0;

	return &blocks[y / 120][x / 120];
}

MapBlock * Map::GetRandomEntrance() const
{
	return entrances[rand() % entrances.size()];
}

bool Map::IsValid() const
{
	return valid;
}

void Map::Draw(int x, int y, float scale) const
{
	static float block_size;
	block_size = BLOCK_SIZE * scale;
	SDL_Rect clip = { 0,0,block_size,block_size };
	for (size_t i = 0; i < BLCOLUMNS; i++)
	{
		for (size_t j = 0; j < BLROWS; j++)
		{
			background->render(x + i * block_size, y + j * block_size, 0, 0, &clip);
		}
	}

	for (size_t i = 0; i < BLROWS; i++)
	{
		for (size_t j = 0; j < BLCOLUMNS; j++)
		{
			if (blocks[i][j].getBlocktype() != BLK_BACKGROUND)
				blocks[i][j].Draw(&clip, x, y, scale);
		}
	}
}

void Map::Update(const float & dt)
{
	for (size_t i = 0; i < BLROWS; i++)
	{
		for (size_t j = 0; j < BLCOLUMNS; j++)
		{
			if (blocks[i][j].getBlocktype() >= BLK_3SQUARE0)
				blocks[i][j].Update(dt);
		}
	}
}
