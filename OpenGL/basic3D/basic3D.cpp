
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

// format X,Y,Z, ?, ?, ?, ?, ? = 8 floats par vertex
#include "DragonData.h"

int numVertices = _countof(DragonVertices);
int numIndices = _countof(DragonIndices);

void Initialize()
{
	glewInit();

	glGenBuffers(1, &VBO);
	// notez la relation en _ARRAY_ et DrawArrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER
		, numVertices*sizeof(float), DragonVertices
		, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &IBO);
	// notez la relation en _ELEMENT_ARRAY_ et DrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		,  numIndices* sizeof(uint16_t), DragonIndices
		, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/*std::cout << "Version des pilotes: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "Fabricant: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Carte graphique: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "extensions supportees: " << std::endl;
	//std::cout << "Version des pilotes" << glGetString(GL_EXTENSIONS) << std::endl;
	int numExt;
	glGetIntegerv(GL_NUM_EXTENSIONS, &numExt);
	for (auto index = 0; index < numExt; ++index) {
		std::cout << "extension[" << index << "] = " << glGetStringi(GL_EXTENSIONS, index) << std::endl;
	}*/

	// extension .v, .vs, .vert, *.glsl ou autre
	BasicProgram.LoadShader(GL_VERTEX_SHADER, "basic3D.vs");
	// extension .f, .fs, .frag, *.glsl ou autre
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "basic3D.fs");
	BasicProgram.Create();
	
}

void Shutdown()
{
	//BasicProgram.Destroy();
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
}

void Update() {
	glutPostRedisplay();
}

void Render() {

	glClearColor(0.f, 0.f, 0.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // | GL_DEPTH_BUFFER_BIT
	glEnable(GL_DEPTH);

	BasicProgram.Bind();
	auto programID = BasicProgram.Get();
	// sans inline la ligne precedente donnerait ceci:
	// temp = CALL MEMBER FUNCTION GET of GLShader
	// programID = temp
	// avec inline : programID = BasicProgram._Program 
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	auto positionLocation = glGetAttribLocation(programID, "a_Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);

	float timeInSeconds = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	auto timeLocation = glGetUniformLocation(programID, "u_Time");
	glUniform1f(timeLocation, timeInSeconds);

	// idem mais avec un Index Buffer (IBO)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);

	glDisableVertexAttribArray(positionLocation);

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