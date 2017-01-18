#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "IsartLib.lib")
#endif

#include <iostream>
#include "GL/glew.h"
#include "GL/freeglut.h"

#include "../common/GLShader.h"

#define PI 3.14159265f

const float quadVertice[] = {
	-0.8f, 0.8f, 0.f, 1.f, 0.f, 0.f,
	-0.8f, -0.8f, 0.f, 0.f, 1.f, 0.f,
	0.8f, 0.8f, 0.f, 0.f, 0.f, 1.f,
	0.8f, -0.8f, 0.f, 1.f, 1.f, 1.f
};

uint16_t quadIndices[] = {
	0, 1, 2, 1, 3, 2
};

GLShader program;
uint32_t program_id;

GLuint VAO;
GLuint VBO;
GLuint IBO;

auto rad(float angle) -> float
{
	return angle * (PI / 180.f);
}

auto Initialize() -> void 
{
	glewInit();

	program.LoadShader(GL_VERTEX_SHADER, "basic3D.vs");
	program.LoadShader(GL_FRAGMENT_SHADER, "basic3D.fs");
	program.Create();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		_countof(quadVertice) * sizeof(float), quadVertice,
		GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		_countof(quadIndices) * sizeof(uint16_t), quadIndices,
		GL_STATIC_DRAW);

	program_id = program.Get();

	glBindVertexArray(VAO);

	auto positionLocation = glGetAttribLocation(program_id, "a_Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8* sizeof(float), 0);


}

auto Shutdown() -> void {}

auto Render() -> void
{
	glEnable(GL_DEPTH);
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
}

auto Update() -> void 
{
	glutPostRedisplay();
}

auto Resize(int, int) -> void {}

auto main(int argc, char* argv[]) -> int
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("Exercise");

	Initialize();

	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);

#if FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	glutMainLoop();

	Shutdown();

	return EXIT_SUCCESS;
}

