#include "globalization.h"

int main(int argc, char* args[])
{
	SDL_Window * win;
	SDL_Renderer * ren;
	SDL_CreateWindowAndRenderer(500, 500, SDL_WINDOW_SHOWN, &win, &ren);

	Image img("files\\cars\\car1\\car.png", ren);
	int angle = 0;
	while (true)
	{
		SDL_RenderClear(ren);
		img.render(100, 100, angle);
		angle++;
		SDL_RenderPresent(ren);
		SDL_Delay(10);
	}
}