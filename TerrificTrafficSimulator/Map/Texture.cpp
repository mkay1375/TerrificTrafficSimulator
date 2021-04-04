#include "Texture.h"

Texture::Texture(char name, Image img)
{
	this->name = name;
	this->img = new Image(img);
	valid = this->img->isValid();
}

Texture::Texture(const Texture & texture)
{
	name = texture.name;
	img = new Image(*(texture.img));
	valid = this->img->isValid();
}

Texture::~Texture()
{
	if (img != 0)
		delete img;
}
