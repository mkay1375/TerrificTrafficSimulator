#pragma once
#include "../globalization.h"

class Texture
{
public:
	char name;
	Image * img = 0;
	bool valid; 

	Texture(char name, Image img);
	Texture(const Texture & texture);
	~Texture();
};

