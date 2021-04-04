#include "SDLHelper.h"

namespace SDLHelper
{
	void cleanup(SDL_Window * win)
	{
		SDL_DestroyWindow(win);
	}
	void cleanup(SDL_Renderer * ren)
	{
		SDL_DestroyRenderer(ren);
	}
	void cleanup(SDL_Texture * tex)
	{
		SDL_DestroyTexture(tex);
	}
	void cleanup(SDL_Surface * surf)
	{
		SDL_FreeSurface(surf);
	}
	bool errLogger(char * text)
	{
		SDL_Log("ERROR: %s, SDL_Error: %s\n", text, SDL_GetError());
		return false;
	}


	SDL_Texture * loadPNG(char * path, SDL_Renderer * renderer)
	{
		if (path == 0 || renderer == 0)
			return 0;

		SDL_Texture * output;

		SDL_Surface * temp;
		temp = IMG_Load(path);
		if (temp == NULL)
		{
			cleanup(temp);
			errLogger("Can't Load Image");
			return 0;
		}

		output = SDL_CreateTextureFromSurface(renderer, temp);
		cleanup(temp);
		if (output == NULL)
		{
			errLogger("Can't Create Texture");
			return 0;
		}

		return output;
	}

	char * file_read(const char * filename) {
		SDL_RWops * rw = SDL_RWFromFile(filename, "rb");
		if (rw == NULL) return NULL;

		Sint64 res_size = SDL_RWsize(rw);
		char* res = (char *)malloc(res_size + 1);

		Sint64 nb_read_total = 0, nb_read = 1;
		char* buf = res;
		while (nb_read_total < res_size && nb_read != 0) {
			nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
		}
		SDL_RWclose(rw);
		if (nb_read_total != res_size) {
			free(res);
			return NULL;
		}

		res[nb_read_total] = '\0';
		return res;
	}
};

