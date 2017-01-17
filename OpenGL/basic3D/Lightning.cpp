//=============================================================================
// Sample Application: Lighting (Per Fragment Phong)
//=============================================================================

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <time.h>

#include "Lightning.h"

auto Lightning::OnResize(int w, int h) -> void
{
	if (h == 0) h = 1;
	float ratio = 1.0f * (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glViewport(0, 0, w, h);

	gluPerspective(45, ratio, 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0f, 0.0f, 4.0f,
		0.0, 0.0, -1.0,
		0.0f, 1.0f, 0.0f);
}
      
auto Lightning::LightPoint(void) -> void
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);

	// Light model parameters:
	// -------------------------------------------

	GLfloat lmKa[] = { 0.0, 0.0, 0.0, 0.0 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmKa);

	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1.0);
	glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, 0.0);

	// -------------------------------------------
	// Spotlight Attenuation

	GLfloat spot_direction[] = { 1.0, -1.0, -1.0 };
	GLint spot_exponent = 30;
	GLint spot_cutoff = 180;

	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLighti(GL_LIGHT0, GL_SPOT_EXPONENT, spot_exponent);
	glLighti(GL_LIGHT0, GL_SPOT_CUTOFF, spot_cutoff);

	GLfloat Kc = 1.0;
	GLfloat Kl = 0.0;
	GLfloat Kq = 0.0;

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, Kc);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, Kl);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, Kq);


	// ------------------------------------------- 
	// Lighting parameters:

	//GLfloat light_pos[] = {0.0f, 5.0f, 5.0f, 1.0f};
	GLfloat light_pos[] = { 0.0f, 5.0f, 0.0f, 1.0f };
	GLfloat light_Ka[] = { 1.0f, 0.5f, 0.5f, 1.0f };
	GLfloat light_Kd[] = { 1.0f, 0.1f, 0.1f, 1.0f };
	GLfloat light_Ks[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_Ka);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_Kd);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_Ks);

	// -------------------------------------------
	// Material parameters:

	GLfloat material_Ka[] = { 0.5f, 0.0f, 0.0f, 1.0f };
	GLfloat material_Kd[] = { 0.4f, 0.4f, 0.5f, 1.0f };
	GLfloat material_Ks[] = { 0.8f, 0.8f, 0.0f, 1.0f };
	GLfloat material_Ke[] = { 0.1f, 0.0f, 0.0f, 0.0f };
	GLfloat material_Se = 20.0f;

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material_Ka);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material_Kd);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material_Ks);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, material_Ke);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material_Se);
}
    
