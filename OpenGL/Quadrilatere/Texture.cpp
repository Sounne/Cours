#include "Texture.h"

#include <fstream>

Texture::Texture()
{
}


Texture::~Texture()
{
}

auto Texture::LoadTexture(const char * path) -> GLuint
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	return texture_id;
}

