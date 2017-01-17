#pragma once

#include <iostream>
#include <time.h>

#include "GL/glew.h"
#include "GL/freeglut.h"

class Lightning
{
public:
	Lightning() = default;
	~Lightning() = default;

	auto OnResize(int w, int h) -> void;
	auto LightPoint(void) -> void;

private:
	GLuint ProgramObject;
	clock_t time0, time1;
	float timer010;  // timer counting 0->1->0
	bool bUp;
};