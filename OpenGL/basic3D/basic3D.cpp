
// Directive de compilation 'pragma' 
// ici force le linkage avec la lib specifiee en parametre
#if _WIN32
#define FREEGLUT_LIB_PRAGMAS	1
#pragma comment (lib, "freeglut.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#endif

#include <iostream>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "../common/GLShader.h"

GLShader BasicProgram;
GLuint VBO;
GLuint IBO;
GLuint VAO;

// format X,Y,Z, ?, ?, ?, ?, ? = 8 floats par vertex
#include "DragonData.h"

int numVertices = _countof(DragonVertices);
int numIndices = _countof(DragonIndices);

void Initialize()
{
	glewInit();

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
	//BasicProgram.Destroy();
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

	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//auto positionLocation = glGetAttribLocation(programID, "a_Position");
	//glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	//glEnableVertexAttribArray(positionLocation);

	float timeInSeconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	auto timeLocation = glGetUniformLocation(programID, "u_Time");
	glUniform1f(timeLocation, timeInSeconds);

	// idem mais avec un Index Buffer (IBO)
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);

	//glDisableVertexAttribArray(positionLocation);
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