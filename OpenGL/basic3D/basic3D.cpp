
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
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
#include "common/mat4.h"

#define PI 3.14159265f

GLShader BasicProgram;
GLuint VBO;
GLuint IBO;
GLuint VAO;

// format X,Y,Z, ?, ?, ?, ?, ? = 8 floats par vertex
#include "DragonData.h"

int numVertices = _countof(DragonVertices);
int numIndices = _countof(DragonIndices);

float moveX = 0.f;
float moveY = 0.f;
float moveZ = 0.f;

float deltaTime = 0.f;
float oldTimeSinceStart = 0.f;
float speed = 5.f;

auto rad(float angle) -> float
{
	float rad = angle * (PI / 180.f);
	return rad;
}

void KeyboardDown(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'w' :
		case 'z' :
			moveY += speed * deltaTime;
			break;
		case 's' :
			moveY -= speed * deltaTime;
			break;
		case 'q' :
		case 'a' :
			moveX -= speed * deltaTime;
			break;
		case 'd' :
			moveX += speed * deltaTime;
			break;
		default: break;
	}
}

void MouseWheel(int button, int dir, int x, int y)
{
	if (dir > 0)
		moveZ += speed * deltaTime;
	else if (dir < 0)
		moveZ -= speed * deltaTime;
}

void Initialize()
{
	glewInit();

	glutKeyboardFunc(KeyboardDown);
	glutMouseWheelFunc(MouseWheel);

	// extension .v, .vs, .vert, *.glsl ou autre
	BasicProgram.LoadShader(GL_VERTEX_SHADER, "basic3D.vs");
	// extension .f, .fs, .frag, *.glsl ou autre
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "basic3D.fs");
	BasicProgram.Create();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);			// <- ATTENTION, un VAO enregistre les parametres suivants :
									//	glBindBuffer(GL_(ELEMENT)_ARRAY_BUFFER)
									//	glEnable/DisableVertexAttribArray()
									//	glVertexAttribPointer()

	glGenBuffers(1, &VBO);
	// notez la relation en _ARRAY_ et DrawArrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER
		, numVertices*sizeof(float), DragonVertices
		, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	// notez la relation en _ELEMENT_ARRAY_ et DrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		,  numIndices* sizeof(uint16_t), DragonIndices
		, GL_STATIC_DRAW);
	
	auto programID = BasicProgram.Get();
	auto positionLocation = glGetAttribLocation(programID, "a_Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);

	//toujours reinitialiser le VAO par defaut (0) avant de bind un autre buffer
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Shutdown()
{
	BasicProgram.Destroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Update() {
	glutPostRedisplay();
}

void Render() {

	glClearColor(0.f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH);
	BasicProgram.Bind();
	auto programID = BasicProgram.Get();
	// sans inline la ligne precedente donnerait ceci:
	// temp = CALL MEMBER FUNCTION GET of GLShader
	// programID = temp
	// avec inline : programID = BasicProgram._Program 

	glBindVertexArray(VAO);

	float timeSinceStart = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	
	auto worldLocation = glGetUniformLocation(programID, "worldMatrix");
	maths::Mat4 identity = maths::Mat4::identity;
	maths::Mat4 worldMatrix = identity.Translate(0.f, 0.f, -10.f) * identity.RotateY(timeSinceStart) * identity.Scale(0.1f);
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldMatrix.val);

	auto projectionLocation = glGetUniformLocation(programID, "projectionMatrix");
	maths::Mat4 projectionMatrix = identity.Perspective(1280.f, 720.f, rad(45.f), 0.1f, 1000.f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.val);
	
	auto viewLocation = glGetUniformLocation(programID, "viewMatrix");
	maths::Mat4 viewMatrix = identity.Translate(moveX, moveY, moveZ);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix.val);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);

	glBindVertexArray(0);

	glutSwapBuffers();	
}

void Resize(int, int ) {}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitWindowSize(1280, 720);
	glutInitWindowPosition(100, 100);
	// defini les buffers du framebuffer 
	// GLUT_RGBA :  color buffer 32 bits
	// GLUT_DOUBLE : double buffering
	// GLUT_DEPTH : ajoute un buffer pour le depth test
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	glutCreateWindow("basic 3D");
	
	Initialize();
	
	glutReshapeFunc(Resize);
	glutIdleFunc(Update);
	glutDisplayFunc(Render);
#if FREEGLUT
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE
			, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
#endif
	glutMainLoop();

	Shutdown();

	return 1;
}