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
#include <cassert>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "../common/GLShader.h"
#include "common/mat4.h"

//attention ce define ne doit etre specifie que dans 1 seul fichier cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb\stb_image.h"

#define PI 3.14159265f

// Framebuffer Object et render target (texture)
GLuint FBO;
GLuint DepthFBO;
GLuint TexFBO;

auto CreateFrameBuffer(int width, int height) -> bool
{
	glGenRenderbuffers(1, &DepthFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, DepthFBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	// creation de la texture qui va servir de color buffer pour le rendu hors ecran
	glGenTextures(1, &TexFBO);
	glBindTexture(GL_TEXTURE_2D, TexFBO);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	// creation du framebuffer pour le rendu hors ecran
	glGenFramebuffers(1, &FBO);
	//GL_FRAMEBUFFER = read/write, GL_READ_FRAMEBUFFER = read only, GL_DRAW_FRAMBUFFER = write only
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	// attache la texture en tant que color buffer #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TexFBO, 0);

	//attache le buffer en tant que depth buffer
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, DepthFBO);

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	//necessite #include <cassert>
	assert(status == GL_FRAMEBUFFER_COMPLETE);

	// switch vers le framebuffer par defaut (celui cree par GLUT ou le contexte de rendu)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

auto DestroyFrameBuffer() -> void
{
	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &DepthFBO);
	glDeleteTextures(1, &TexFBO);
}

GLShader BasicProgram;
GLShader QuadProgram;
uint32_t basicProgramID;
uint32_t quadProgramID;
GLuint VBO;
GLuint quadVBO;
GLuint IBO;
GLuint quadIBO;
GLuint TexDragon;
GLuint VAO;
GLuint quadVAO;

// format X,Y,Z, NX, NY, NZ, U, V = 8 floats par vertex
#include "DragonData.h"

const float quadVertice[] = {
	-1.f, 1.f,
	-1.f, -1.f,
	1.f, 1.f,
	1.f, -1.f
};

uint16_t quadIndices[] = {
	0, 1, 2, 1, 3, 2
};

int numVertices = _countof(DragonVertices);
int numIndices = _countof(DragonIndices);

float moveX = 0.f;
float moveY = 0.f;
float moveZ = 0.f;

float angle = 0.f;

float deltaTime = 0.f;
float oldTimeSinceStart = 0.f;
float speed = 5.f;

float lightPower = 1.f;

uint32_t texUnit = 0;

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
		case 'k':
			angle -= speed * deltaTime;
			break;
		case 'l':
			angle += speed * deltaTime;
			break;
		case 'o':
			lightPower -= speed * deltaTime;
			break;
		case 'p':
			lightPower += speed * deltaTime;
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

	int w, h, c;

	uint8_t* imageData = stbi_load("dragon.png", &w, &h, &c, STBI_rgb_alpha);
	glGenTextures(1, &TexDragon);
	glBindTexture(GL_TEXTURE_2D, TexDragon);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8 /*ou juste GL_RGBA*/, w, h, 0,	// params mem video
		GL_RGBA, GL_UNSIGNED_BYTE, imageData);								// params mem systeme

	stbi_image_free(imageData);

	//par default le mode de min filter est GL_NEAREST_MIPMAP_LINEAR
	//ce qui necessite de definir tous les niveaux de details (LOD)
	//on peut forcer le filtrage a etre en nearest ou linear pour desactiver le mip mapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// .. ou bien generer les mipmap
	//glGenerateMipMap(GL_TEXTURE_2D);


	// extension .v, .vs, .vert, *.glsl ou autre
	BasicProgram.LoadShader(GL_VERTEX_SHADER, "basic3D.vs");
	// extension .f, .fs, .frag, *.glsl ou autre
	BasicProgram.LoadShader(GL_FRAGMENT_SHADER, "basic3D.fs");
	BasicProgram.Create();

	{
		QuadProgram.LoadShader(GL_VERTEX_SHADER, "quad.vs");
		QuadProgram.LoadShader(GL_FRAGMENT_SHADER, "quad.fs");
		QuadProgram.Create();
	}

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(GL_TEXTURE_2D, TexDragon);

	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);			// <- ATTENTION, un VAO enregistre les parametres suivants :
									//	glBindBuffer(GL_(ELEMENT)_ARRAY_BUFFER)
									//	glEnable/DisableVertexAttribArray()
									//	glVertexAttribPointer()

	glGenBuffers(1, &VBO);
	// notez la relation en _ARRAY_ et DrawArrays
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER
		, numVertices * sizeof(float), DragonVertices
		, GL_STATIC_DRAW);

	glGenBuffers(1, &IBO);
	// notez la relation en _ELEMENT_ARRAY_ et DrawElements
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER
		, numIndices * sizeof(uint16_t), DragonIndices
		, GL_STATIC_DRAW);

	basicProgramID = BasicProgram.Get();
	glBindVertexArray(VAO);

	auto positionLocation = glGetAttribLocation(basicProgramID, "a_Position");
	glVertexAttribPointer(positionLocation, 3, GL_FLOAT, false, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(positionLocation);
	auto normalLocation = glGetAttribLocation(basicProgramID, "a_Normal");
	glVertexAttribPointer(normalLocation, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(normalLocation);
	auto texCoordsLocation = glGetAttribLocation(basicProgramID, "a_TexCoords");
	glVertexAttribPointer(texCoordsLocation, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(texCoordsLocation);

	//toujours reinitialiser le VAO par defaut (0) avant de bind un autre buffer
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	{
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBO);
		// notez la relation en _ARRAY_ et DrawArrays
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER
			, _countof(quadVertice) * sizeof(float), quadVertice
			, GL_STATIC_DRAW);

		glGenBuffers(1, &quadIBO);
		// notez la relation en _ELEMENT_ARRAY_ et DrawElements
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER
			, _countof(quadIndices) * sizeof(uint16_t), quadIndices
			, GL_STATIC_DRAW);

		quadProgramID = QuadProgram.Get();
		glBindVertexArray(quadVAO);

		auto positionLocation = glGetAttribLocation(quadProgramID, "a_Position");
		glVertexAttribPointer(positionLocation, 2, GL_FLOAT, false, 2 * sizeof(float), 0);
		glEnableVertexAttribArray(positionLocation);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	CreateFrameBuffer(glutGet(GLUT_WINDOW_WIDTH),glutGet(GLUT_WINDOW_HEIGHT));
}

void Shutdown()
{
	BasicProgram.Destroy();
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteBuffers(1, &quadIBO);
}

void Update() {
	glutPostRedisplay();
}

void Render() {

	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_TEST);

	//redirige toutes les commades de rendu vers le FBO donc hors ecran
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	GLuint renderedTexture;
	glGenTextures(1, &renderedTexture);
	glBindTexture(GL_TEXTURE_2D, renderedTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1280, 720, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
	GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, DrawBuffers);

	GLuint depthRenderBuffer;
	glGenRenderbuffers(1, &depthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1280, 720);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRenderBuffer);


	glClearColor(0.f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	BasicProgram.Bind();
	//auto programID = BasicProgram.Get();
	// sans inline la ligne precedente donnerait ceci:
	// temp = CALL MEMBER FUNCTION GET of GLShader
	// programID = temp
	// avec inline : programID = BasicProgram._Program

	glBindVertexArray(VAO);

	float timeSinceStart = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
	deltaTime = timeSinceStart - oldTimeSinceStart;
	oldTimeSinceStart = timeSinceStart;
	
	auto worldLocation = glGetUniformLocation(basicProgramID, "worldMatrix");
	maths::Mat4 identity = maths::Mat4::identity;
	maths::Mat4 worldMatrix = identity.Translate(0.f, 0.f, -10.f) * identity.RotateY(angle) * identity.Scale(0.1f);
	glUniformMatrix4fv(worldLocation, 1, GL_FALSE, worldMatrix.val);

	auto projectionLocation = glGetUniformLocation(basicProgramID, "projectionMatrix");
	maths::Mat4 projectionMatrix = identity.Perspective(1280.f, 720.f, rad(45.f), 0.1f, 1000.f);
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, projectionMatrix.val);
	
	auto viewLocation = glGetUniformLocation(basicProgramID, "viewMatrix");
	maths::Mat4 viewMatrix = identity.Translate(moveX, moveY, moveZ);
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, viewMatrix.val);

	auto textureLocation = glGetUniformLocation(basicProgramID, "u_Texture");
	glUniform1i(textureLocation, texUnit);

	auto powerLocation = glGetUniformLocation(basicProgramID, "u_power");
	glUniform1f(powerLocation, lightPower);

	auto camPosLocation = glGetUniformLocation(basicProgramID, "u_cam_pos");
	glUniform3f(camPosLocation, moveX, moveY, moveZ);

	auto timeLocation = glGetUniformLocation(basicProgramID, "u_Time");
	//glUniform3f(timeLocation, std::time)

	glBindTexture(GL_TEXTURE_2D, TexDragon);

	glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	// zero : bind du framebuffer par defaut soit le framebuffer de GLUT ici (backbuffer etc...)
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0.f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BasicProgram.Unbind();
	QuadProgram.Bind();

	glBindVertexArray(quadVAO);

	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	auto txtureLocation = glGetUniformLocation(quadProgramID, "u_Texture");
	glUniform1i(txtureLocation, renderedTexture);

	glDrawElements(GL_TRIANGLES, _countof(quadIndices), GL_UNSIGNED_SHORT, nullptr);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindVertexArray(0);

	// todo : configurer les framebuffers d'entree et sortie pour que le blit
	// sache quel fbo lire et vers quel fbo ecrire
	/*int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, windowWidth, windowHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);*/
	QuadProgram.Unbind();
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