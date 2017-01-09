#if _WIN32
#define FREEGLUT_LIB_PRAGMAS 1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#endif


#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"
//#include "glext.h"

//PFNGLGETSTRINGIPROC glGetStringi = nullptr;

static const float g_Triangle[] = {
	-0.8f, 0.8f, 1.0f, 0.0f, 0.0f,
	0.0f, -0.8f, 0.0f, 1.0f, 0.0f,
	0.8f, 0.8f, 0.0f, 0.0f, 1.0f
};

auto Initialize() -> void
{
	glewInit();
	//wglGetProcAddress() version specialisee de getProcAddress() qui recupere
	//un pointeur de fonction dans la dll du driver OpenGL
	//glGetStringi = (PFNGLGETSTRINGIPROC)wglGetProcAddress("glGetStringi");

	std::cout << "Version des pilotes : " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Fabricant : " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Carte graphique : " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "extensions supportees : " << std::endl;
	//std::cout << glGetString(GL_EXTENSIONS) << std::endl;
	int numExt;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
	for (int idx = 0; idx < numExt; ++idx)
		std::cout << "extension[" << idx << "] = " << glGetStringi(GL_EXTENSIONS, idx) << std::endl;
}

auto Update() -> void
{
	glutPostRedisplay();
}

auto Render() -> void
{
	glViewport(40, 40, 1200, 640);
	glScissor(40, 40, 1200, 640);
	glEnable(GL_SCISSOR_TEST);

	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClearColor(rand()/(float)RAND_MAX, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT); // | GL_DEPTH_BUFFFER_BIT

	glEnable(GL_CULL_FACE);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, 5 * sizeof(float), g_Triangle);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(3, 3, GL_FLOAT, false, 5 * sizeof(float), &g_Triangle[2]);
	//glEnableVertexAttribArray(3);
	glVertexAttrib3f(3, 1.0f, 1.0f, 0.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(0);

	/*glBegin(GL_TRIANGLES);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex2f(0.0f, -0.8f);
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	glVertex2f(-0.4f, 0.0f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex2f(-0.8f, -0.8f);

	glEnd();

	glBegin(GL_TRIANGLE_STRIP);

	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex2f(0.0f, 0.8f);
	glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(0.8f, 0.8f);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	glVertex2f(0.8f, 0.0f);

	glEnd();*/

	glColorMask(false, true, true, true);

	glutSwapBuffers();
}

auto Resize(int, int) -> void
{

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);

	//definit les buffers du framebuffer
	//GL_RGBA : color buffer 32 bits
	//GL_DOUBLE : double buffering
	//GL_DEPTH : ajoute un buffer pour le depth test
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );

	glutCreateWindow("basic GL");
	Initialize();

	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);

#ifdef FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	glutMainLoop();

	//Terminate();

	return 1;
}