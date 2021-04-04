#pragma once
#include "globalization.h"

template<typename T>
class CoordinateSystem {
private:
	T *** mainsurface;
	int width = 0, height = 0;

	int xc, yc; // for performance purposes

public:
	CoordinateSystem(int width, int height)
	{
		if (width > 0 && height > 0)
		{
			this->width = width; this->height = height;

			mainsurface = new T**[width]; // xc
			for (xc = 0; xc < width; xc++)
				mainsurface[xc] = new T*[height]; // yc

			for (xc = 0; xc < width; xc++)
				for (yc = 0; yc < height; yc++)
					mainsurface[xc][yc] = 0;
		}
		else
		{
			printf("Caution! Coordinate system is not valid!");
			system("pause");
		}
	}
	~CoordinateSystem() {}

	float DistanceBetweenPts(const float & x1, const float & y1, const float & x2, const float & y2)
	{
		return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	}

	void SetRectOnSurface(T * ref, const float & CenterX, const float & CenterY, const int & width, const int & height, const float & angle)
	{
		static int diff; static float radian;
		diff = abs(height - width);
		radian = angle * Pi / 180.0;
		static int WIDTH; static int HEIGHT;
		WIDTH = (width + abs(diff * sin(radian)));
		HEIGHT = (width + abs(diff * cos(radian)));
		for (xc = CenterX - WIDTH / 2; (xc < CenterX + WIDTH / 2) && (xc < this->width); xc++)
			for (yc = CenterY - HEIGHT / 2; (yc < CenterY + HEIGHT / 2) && (yc < this->height); yc++)
				if (xc >= 0 && yc >= 0)
					mainsurface[xc][yc] = ref;
		/*static int X;
		static int Y;
		static int r;
		for (xc = CenterX - width / 2; (xc < CenterX + width / 2) && (xc < this->width); xc++)
			for (yc = CenterY - height / 2; (yc < CenterY + height / 2) && (yc < this->height); yc++)
			{
				r = DistanceBetweenPts(CenterX, CenterY, xc, yc);
				X = CenterX + r * cos(angle * Pi / 180.0);
				Y = CenterX + r * sin(angle * Pi / 180.0);
				if (X >= 0 && Y >= 0 && X <= width && Y <= height)
				{
					mainsurface[X][Y] = ref;
				}
			}*/
	}

	void ClearRectOnSurface(const float & CenterX, const float & CenterY, const int & width, const int & height, const float & angle)
	{
		static int diff; static float radian;
		diff = abs(height - width);
		radian = angle * Pi / 180.0;
		static int WIDTH; static int HEIGHT;
		WIDTH = (width + abs(diff * sin(radian)));
		HEIGHT = (width + abs(diff * cos(radian)));
		for (xc = CenterX - WIDTH / 2; (xc < CenterX + WIDTH / 2) && (xc < this->width); xc++)
			for (yc = CenterY - HEIGHT / 2; (yc < CenterY + HEIGHT / 2) && (yc < this->height); yc++)
				if (xc >= 0 && yc >= 0)
					mainsurface[xc][yc] = 0;
		/*static int X;
		static int Y;
		static int r;
		for (xc = CenterX - width / 2; (xc < CenterX + width / 2) && (xc < this->width); xc++)
			for (yc = CenterY - height / 2; (yc < CenterY + height / 2) && (yc < this->height); yc++)
			{
				r = DistanceBetweenPts(CenterX, CenterY, xc, yc);
				X = CenterX + r * cos(angle * Pi / 180.0);
				Y = CenterX + r * sin(angle * Pi / 180.0);
				if (X >= 0 && Y >= 0 && X <= width && Y <= height)
				{
					mainsurface[X][Y] = 0;
				}
			}*/
	}

	void Draw(SDL_Renderer * ren) // For debugging purposes
	{
		static Image reddot("files\\debug\\reddot.png", ren);
		for (xc = 0; xc < width; xc++)
			for (yc = 0; yc < height; yc++)
				if (mainsurface[xc][yc] != 0)
					reddot.render(xc, yc);
	}

	T * GetThing(const int & x, const int & y)
	{
		if ((0 <= x && x < width) && (0 <= y && y < height))
			return mainsurface[x][y];
		else
			return 0;
	}
};
