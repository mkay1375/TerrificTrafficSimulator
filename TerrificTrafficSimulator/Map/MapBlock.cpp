#include "MapBlock.h"


void MapBlock::InitTrafficLight()
{
	STLS = (SquareTLStatus)(rand() % 2);
}

void MapBlock::ChangeLight(const float & dt) const
{
	if (!this)
		return;
	if (begTime > LightDuration)
	{
		begTime = 0;
		if (STLS == SQR_HOR_GREEN)
			STLS = SQR_VER_GREEN;
		else
			STLS = SQR_HOR_GREEN;
	}
	else
		begTime += dt;
}

void MapBlock::DrawTRLs(SDL_Rect * clip) const
{
	if (type < BLK_3SQUARE0)
		return;

	if (!trl_up_green)
		trl_up_green = new Image("files/trlights/green up.png", img->GetRenderer());

	switch (STLS)
	{
	case SQR_HOR_GREEN:
	{
		if (type != BLK_3SQUARE1 && type != BLK_3SQUARE0)
			trl_up_green->render(x, y, 90);
		if (type != BLK_3SQUARE3 && type != BLK_3SQUARE2)
			trl_up_green->render(x, y, 270);
	}
	break;
	case SQR_VER_GREEN:
	{
		if (type != BLK_3SQUARE0 && type != BLK_3SQUARE3)
			trl_up_green->render(x, y, 0);
		if (type != BLK_3SQUARE2 && type != BLK_3SQUARE1)
			trl_up_green->render(x, y, 180);
	}
	break;
	default:
		break;
	}

}


MapBlock::MapBlock()
{


}

MapBlock::MapBlock(BlockType btype, Image * bimg, double imgAngle, int x, int y) : x(x), y(y)
{
	begTime = (rand() % LightDuration * 1000.0) / 1000.0;

	roadpxs[0] = 22;
	roadpxs[1] = 46;
	roadpxs[2] = 73;
	roadpxs[3] = 97;

	type = btype;
	img = bimg;
	imageAngle = imgAngle;
	InitTrafficLight();
}

MapBlock::MapBlock(const MapBlock & obj)
{
	printf("copy called\n");
	begTime = obj.begTime;

	roadpxs[0] = 22;
	roadpxs[1] = 46;
	roadpxs[2] = 73;
	roadpxs[3] = 97;

	type = obj.type;
	img = obj.img;
	imageAngle = obj.imageAngle;
}


MapBlock::~MapBlock()
{
	if (trl_up_green)
		delete trl_up_green;
}

BlockType MapBlock::getBlocktype() const
{
	if (!this)
		return BLK_NOTSET;
	return type;
}


SquareTLStatus MapBlock::TrLightStatus() const
{
	return STLS;
}

void MapBlock::Update(const float & dt)
{
	ChangeLight(dt);
}


int MapBlock::AvailabeXorY(bool line, Direction dir) const
{

	switch (dir)
	{
	case DIR_UP:
		return roadpxs[3 - line] + x;
		break;
	case DIR_LEFT:
		return roadpxs[line] + y;
		break;
	case DIR_DOWN:
		return roadpxs[line] + x;
		break;
	case DIR_RIGHT:
		return roadpxs[3 - line] + y;
		break;
	default:
		break;
	}
}

int MapBlock::GetBlockX() const
{
	return x;
}

int MapBlock::GetBlockY() const
{
	return y;
}

void MapBlock::Draw(SDL_Rect * clip, int x0, int y0, float scale) const
{
	if (img)
	{
		if (!ANDROID)
		{
			if (imageAngle == 90)
				img->render(x0 + this->x * scale - 1, y0 + this->y * scale, imageAngle, 0, clip);
			else if (imageAngle == 180)
				img->render(x0 + this->x * scale - 1, y0 + this->y * scale - 1, imageAngle, 0, clip);
			else if (imageAngle == 270)
				img->render(x0 + this->x * scale, y0 + this->y * scale - 1, imageAngle, 0, clip);
			else
				img->render(x0 + this->x * scale, y0 + this->y * scale, imageAngle, 0, clip);
		}
		else
			img->render(x0 + this->x * scale, y0 + this->y * scale, imageAngle, 0, clip);
	}
	if (scale == 1)
		DrawTRLs(clip);
}
void MapBlock::PrintType() const
{
	if (this == 0)
		return;
	switch (type)
	{
	case BLK_NOTSET:
		printf("NOTSET\n");
		break;
	case BLK_BACKGROUND:
		printf("BLK_BACKGROUND\n");
		break;
	case BLK_TEXTURE:
		printf("BLK_TEXTURE\n");
		break;
	case BLK_VROAD:
		printf("BLK_VROAD\n");
		break;
	case BLK_HROAD:
		printf("BLK_HROAD\n");
		break;
	case BLK_4SQUARE:
		printf("BLK_4SQUARE\n");
		break;
	case BLK_3SQUARE0:
		printf("BLK_3SQUARE0\n");
		break;
	case BLK_3SQUARE1:
		printf("BLK_3SQUARE1\n");
		break;
	case BLK_3SQUARE2:
		printf("BLK_3SQUARE2\n");
		break;
	case BLK_3SQUARE3:
		printf("BLK_3SQUARE3\n");
		break;
	case BLK_2SQUARE0:
		printf("BLK_2SQUARE0\n");
		break;
	case BLK_2SQUARE1:
		printf("BLK_2SQUARE1\n");
		break;
	case BLK_2SQUARE2:
		printf("BLK_2SQUARE2\n");
		break;
	case BLK_2SQUARE3:
		printf("BLK_2SQUARE3\n");
		break;
	default:
		break;
	}
}
