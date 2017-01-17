
#include <iostream>

#include "glew.h"
#include "freeglut.h"
#include "GLShader.h"
#include "mat4.h"
#include "Texture.h"

#define PI 3.14159265f

#include "DragonData.h"

int numVertices = _countof(DragonVertices);
int numIndices = _countof(DragonIndices);

float moveX = 0.f;
float moveY = 0.f;
float moveZ = 0.f;

float deltaTime = 0.f;
float oldTimeSinceStart = 0.f;
float speed = 5.f;

Texture texture;
GLShader BasicProgram;
GLuint VBO;
GLuint IBO;
GLuint VAO;

static const float Square[] = {
	-0.8f, 0.8f, 1.0f, 0.0f, 0.0f,
	-0.8f, -0.8f, 0.0f, 1.0f, 0.0f,
	0.8f, 0.8f, 0.0f, 0.0f, 1.0f,
	0.8f, -0.8f, 1.0f, 1.0f, 1.0f
};

uint16_t indices[] = { 0, 1, 2, 1, 3, 2 };

auto rad(float angle) -> float
{
	float rad = angle * (PI / 180.f);
	return rad;
}

auto Init() -> void
{
	glewInit();

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 4 * 5 * sizeof(float), Square, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(uint16_t), indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	BasicProgram.LoadShader(GL_VERTEX_SHADER, "exo.vs");
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "exo.fs");
	BasicProgram.Create();
}

void Shutdown()
{
	BasicProgram.Destroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Render() {

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	BasicProgram.Bind();
	uint32_t programId = BasicProgram.Get();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	auto positionLocation = glGetAttribLocation(programId, "a_Position");

	glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 5 * sizeof(float), 0/*g_Triangle*/);
	glEnableVertexAttribArray(positionLocation);

	auto colorLocation = glGetAttribLocation(programId, "a_Color");
	glVertexAttribPointer(colorLocation, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)(2 * sizeof(float))/*&g_Triangle[2]*/);
	glEnableVertexAttribArray(colorLocation);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, Square);


	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, nullptr);
	glDisableVertexAttribArray(colorLocation);
	glDisableVertexAttribArray(positionLocation);
	glutSwapBuffers(); 
}

void Update() { glutPostRedisplay(); }
void Resize(int, int) {}

int main(int ac, char ** av)
{
	glutInit(&ac, av);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(1280, 720);

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutCreateWindow("Window");
	Init();

	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);

#ifdef FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif

	glutMainLoop();

	Shutdown();

	return EXIT_SUCCESS;
}