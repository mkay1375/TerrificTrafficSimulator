#pragma once
#include "../globalization.h"
namespace SDLHelper
{
	void cleanup(SDL_Window * win);
	void cleanup(SDL_Renderer * ren);
	void cleanup(SDL_Texture * tex);
	void cleanup(SDL_Surface * surf);

	bool errLogger(char * text);

	SDL_Texture * loadPNG(char * path, SDL_Renderer * renderer);

	char * file_read(const char * filename);
};

