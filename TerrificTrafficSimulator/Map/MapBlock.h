#pragma once
#include "../globalization.h"

class MapBlock
{
protected:
	int BlockSize = 120;

	BlockType type = BLK_NOTSET;
	Image * img = 0; // It won't copy and save the image in object; It's just an address.
	mutable Image * trl_up_green = 0; // It must be created and freed;
	double imageAngle;
	int x = 0, y = 0;

	int roadpxs[4] = { 0 }; // Movable Pixels in roads (X or Y)

	mutable SquareTLStatus STLS; // traffic light status
	mutable float begTime = 0;
	int LightDuration = 10; // seconds
	void InitTrafficLight();
	void ChangeLight(const float & dt) const;
	void DrawTRLs(SDL_Rect * clip) const;

public:
	MapBlock();
	MapBlock(BlockType btype, Image * bimg, double imgAngle, int x, int y);
	MapBlock(const MapBlock & obj);
	~MapBlock();

	BlockType getBlocktype() const;
	SquareTLStatus TrLightStatus() const;
	void Update(const float & dt);
	int AvailabeXorY(bool line, Direction dir) const;
	int GetBlockX() const;
	int GetBlockY() const;


	void Draw(SDL_Rect * clip, int x0 = 0, int y0 = 0, float scale = 1.0) const;

	void PrintType() const; // debugging
};

