#if _WIN32
#define FREEGLUT_LIB_PRAGMAS 1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#endif


#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "../common/GLShader.h"

GLShader	basicProgram;
GLuint		VBO;
GLuint		IBO;

const float g_Triangle[] = {
	-0.8f, 0.8f, 1.0f, 0.0f, 0.0f,
	-0.8f, -0.8f, 0.0f, 1.0f, 0.0f,
	0.8f, 0.8f, 0.0f, 0.0f, 1.0f,
	0.8f, -0.8f, 1.0f, 1.0f, 1.0f
};

uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };

auto Initialize() -> void
{
	glewInit();

	glGenBuffers(1, &VBO);
	//	notez la relation entre _ARRAY_ et DrawArrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4  * 5 * sizeof(float), g_Triangle, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	//	notez la relation entre _ARRAY_ et DrawArrays
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(uint16_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//extension .v, .vs, .vert ou autre
	basicProgram.LoadShader(GL_VERTEX_SHADER, "basic.vs");
	//extension .f, .fs, .frag ou autre
	basicProgram.LoadShader(GL_FRAGMENT_SHADER, "basic.fs");
	basicProgram.Create();
}

auto Shutdown() -> void
{
	//basicProgram.Destroy();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

auto Update() -> void
{
	glutPostRedisplay();
}

auto Render() -> void
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	basicProgram.Bind();
	uint32_t programId = basicProgram.Get();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	auto positionLocation = glGetAttribLocation(programId, "a_Position");

	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 5 * sizeof(float), 0/*g_Triangle*/);
	glEnableVertexAttribArray(positionLocation);

	auto colorLocation = glGetAttribLocation(programId, "a_Color");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float))/*&g_Triangle[2]*/);
	glEnableVertexAttribArray(colorLocation);

	float timeInSeconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	auto timeLocation = glGetUniformLocation(programId, "u_Time");
	glUniform1f(timeLocation, timeInSeconds);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(positionLocation);
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

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE );

	glutCreateWindow("Window");
	Initialize();

	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);

#ifdef FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	glutMainLoop();

	Shutdown();

	return 1;
}