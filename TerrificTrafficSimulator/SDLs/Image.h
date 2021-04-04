#pragma once
#include "../globalization.h"


class Image
{
private:
	SDL_Texture * img = 0;
	SDL_Renderer * renderer = 0;
	int w, h;
	char * path = 0;

	bool valid;
	bool loadImg(const char * path, SDL_Renderer * renderer);
public:
	Image(const char * path, SDL_Renderer * renderer);
	Image(const Image & obj);
	~Image();

	void render(int x, int y, double angle = 0.0, SDL_Point* center = NULL, SDL_Rect* clip = NULL) const;

	void SetAlpha(Uint8 alpha);

	void SetRenderer(SDL_Renderer * renderer);
	SDL_Renderer * GetRenderer() const;
	bool isValid() const;
	int W() const;
	int H() const;

};

