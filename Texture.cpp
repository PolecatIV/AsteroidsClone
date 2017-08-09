#include "Texture.h"
#include <iostream>



Texture::Texture(const std::string& fileName)
{
	int width, height, numComponents;
	unsigned char*_textureData= stbi_load(fileName.c_str(), &width, &height, &numComponents, 4);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, _textureData);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(_textureData);

}


Texture::~Texture()
{
	//SDL_FreeSurface(_textureData);
}

void Texture::Bind(unsigned int unit)
{
	//assert(unit unit >= 0 && unit <= 31);
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_2D, _texture);
}