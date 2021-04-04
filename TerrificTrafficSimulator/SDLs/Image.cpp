#include "Image.h"


using namespace SDLHelper;

bool Image::loadImg(const char * path, SDL_Renderer * renderer)
{
	if (path == NULL || renderer == NULL)
		return false;
	SDL_Surface * temp;
	temp = IMG_Load(path);
	if (temp == NULL)
	{
		errLogger("Can't Load Image");
		return false;
	}
	SDL_SetColorKey(temp, SDL_TRUE, SDL_MapRGB(temp->format, 0, 0xFF, 0xFF));
	img = SDL_CreateTextureFromSurface(renderer, temp);
	w = temp->w;
	h = temp->h;

	cleanup(temp);
	if (img == NULL)
	{
		errLogger("Can't Create Texture");
		return false;
	}

	return true;
}

Image::Image(const char * path, SDL_Renderer * renderer)
{
	if (path != 0 && renderer != 0)
	{
		this->renderer = renderer;

		size_t len = strlen(path);
		this->path = new char[strlen(path) + 1];
		memset(this->path, 0, len + 1);
		strncpy(this->path, path, len);

		valid = loadImg(path, renderer);
	}
	else
		valid = false;
}

Image::Image(const Image & obj)
{
	renderer = obj.renderer;

	size_t len = strlen(obj.path);
	path = new char[len + 1];
	memset(path, 0, len + 1);
	strncpy(path, obj.path, len);

	valid = loadImg(path, obj.renderer);
}

Image::~Image()
{
	if (path != 0)
		delete[] path;
	if (img != 0)
		cleanup(img);
}

void Image::render(int x, int y, double angle, SDL_Point * center, SDL_Rect * clip) const
{
	if (!valid)
	{
		printf("Image is not vaild: %s", path);
		return;
	}

	SDL_Rect renderQuad = { x, y, w, h };

	if (clip != NULL)
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, img, clip = 0, &renderQuad, angle, center, SDL_FLIP_NONE);
}

void Image::SetAlpha(Uint8 alpha)
{
	if (alpha >= 255)
		SDL_SetTextureAlphaMod(img, 255);
	else if (alpha <= 0)
		SDL_SetTextureAlphaMod(img, 0);
	else
		SDL_SetTextureAlphaMod(img, alpha);
}

void Image::SetRenderer(SDL_Renderer * renderer)
{
	if (renderer != 0)
		this->renderer = renderer;
}

SDL_Renderer * Image::GetRenderer() const
{
	return renderer;
}

bool Image::isValid() const
{
	return valid;
}

int Image::W() const
{
	return w;
}

int Image::H() const
{
	return h;
}
