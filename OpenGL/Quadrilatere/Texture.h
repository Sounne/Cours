#pragma once

#include "glew.h"
#include "freeglut.h"

struct Texture
{
	Texture();
	~Texture();

	auto LoadTexture(const char * path) -> GLuint;
};

