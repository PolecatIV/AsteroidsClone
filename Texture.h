#pragma once
#include <string>
#include <glew.h>
#include "stb_image.h"

class Texture
{
public:
	Texture(const std::string& fileName);
	void Bind(unsigned int unit);
	~Texture();

private:
	GLuint _texture;
};

