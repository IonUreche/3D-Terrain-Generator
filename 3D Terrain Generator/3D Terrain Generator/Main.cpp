
#include <windows.h>  // biblioteci care urmeaza sa fie incluse
#include <stdlib.h> // necesare pentru citirea shader-elor
#include <GL/glew.h> 
#include <GL/freeglut.h>

#include <stdio.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>

#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define M_PI       3.14159265358979323846

#include "Sphere.h"
#include "Terrain.h"

using namespace std;

//////////////////////////////////////
// identificatori 
GLuint
VaoId,
VboId,
ColorBufferId,
ProgramId,
VertexShaderId,
FragmentShaderId,
modelMatrixLocation,
viewLocation,
projLocation,
matrRotlLocation,
codColLocation;

Sphere *mySphere;

int codCol;
float PI = 3.141592;

// matrice utilizate
glm::mat4 model, matrRot;

float *points, *colors;

// elemente pentru matricea de vizualizare
float Obsx = -15, Obsy = 15, Obsz = -15;
float Refx = 5.0f, Refy = 0.0f, Refz = 5.0f;
float Vx = 0.0;
glm::mat4 view;

int isWireFrame = 1;

Terrain *ter;

// Mouse
int lastMouseX = -1, lastMouseY = -1;
float AngleXoZ = 303.5f, AngleYoZ = 0;
float scrollSpeed = 0.5f, cameraSphereRadius = 14.0f;

//Camera
float CenterX = 5.0f, CenterY = 5.0f, CenterZ = 5.0f;

// elemente pentru matricea de proiectie
float width = 1200, height = 900, xwmin = -1200.f, xwmax = 1200, ywmin = -1000, ywmax = 1000, znear = 0.001f, zfar = 10000.0f, fov = 120;
glm::mat4 projection;

void displayMatrix()
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << model[ii][jj] << "  ";
		cout << endl;
	};
	cout << "\n";

};

void processNormalKeys(unsigned char key, int x, int y)
{

	switch (key) {
	case 'l':
		Vx += 0.1;
		break;
	case 'r':
		Vx -= 0.1;
		break;
	case '+':
		cameraSphereRadius -= 1.0f;
		break;
	case '-':
		cameraSphereRadius += 1.0f;
		break;
	case 'e':
		Obsy += 2;
		Refy += 2;
		break;
	case 'q':
		Obsy -= 2;
		Refy -= 2;
		break;
	case 'o':
		ter->Update();
		break;
	case 'v':
		isWireFrame ^= 1;
		break;
	case 'p':
		ter->SmoothTerrain(2);
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//case 'b':
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT:
		Obsx -= 2; 
		Refx -= 2;
		break;
	case GLUT_KEY_RIGHT:
		Obsx += 2;
		Refx += 2;
		break;
	case GLUT_KEY_UP:
		Obsz += 2;
		Refz += 2;
		break;
	case GLUT_KEY_DOWN:
		Obsz -= 2;
		Refz -= 2;
		break;
	}
}

const GLchar *VertexShader, *FragmentShader;

const GLchar* LoadShaderFromFile(char * filename){

	ifstream f(filename);
	string s;

	getline(f, s, char(EOF));

	f.close();

	const char *cShader = s.c_str();
	int l = strlen(cShader);

	GLchar* shader = new GLchar[l];
	strcpy(shader, cShader);

	return shader;
}

bool CreateShaders(void)
{
	GLint compiled_ok;

	VertexShader = LoadShaderFromFile("vertex_shader.vert");
	FragmentShader = LoadShaderFromFile("fragment_shader.frag");

	VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(VertexShaderId, 1, &VertexShader, NULL);
	glCompileShader(VertexShaderId);
	glGetShaderiv(VertexShaderId, GL_COMPILE_STATUS, &compiled_ok);
	if (!compiled_ok){ printf("Vertex shader didn't compile!\n"); return false; }

	FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(FragmentShaderId, 1, &FragmentShader, NULL);
	glCompileShader(FragmentShaderId);

	glGetShaderiv(FragmentShaderId, GL_COMPILE_STATUS, &compiled_ok);
	if (!compiled_ok){ printf("Fragment shader didn't compile!\n"); return false; }

	ProgramId = glCreateProgram();
	glAttachShader(ProgramId, VertexShaderId);
	glAttachShader(ProgramId, FragmentShaderId);
	glLinkProgram(ProgramId);
	glUseProgram(ProgramId);
	return true;
}

void DestroyShaders(void)
{
	glDeleteProgram(ProgramId);
}

void CreateBuffers()
{
	glGenBuffers(1, &VboId);
	glGenBuffers(1, &ColorBufferId);
}

void CreateVBO(void)
{
	// se creeaza un buffer nou se seteaza ca buffer curent si punctele sunt "copiate" in bufferul curent
	//glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points) * 18, points, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	//glGenVertexArrays(1, &VaoId);
	//glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	//glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors) * 18, colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
}

void DestroyVBO()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);
	
	//glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VaoId);
}

bool Initialize()
{
	model = glm::mat4(1.0f);

	matrRot = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));

	if (CreateShaders() == false)
	{
		printf("%s\n", "Shader could not be created");
		return false;
	}

	CreateShaders();
	CreateBuffers();

	//mySphere = new Sphere(glm::vec3(1.0f, 1.0f, 1.0f), 1, 7, 14);
	ter = new Terrain(10, 10, 100, 100);
	ter->SetVertexShader(ProgramId);
	
	vector<glm::vec3> bez;
	for (int i = 0; i < 4; ++i)
	{
		bez.push_back(glm::vec3(0.0f, 0.0f, i * 3.33f));
		bez.push_back(glm::vec3(3.33f, 5.0f, i * 3.33f));
		bez.push_back(glm::vec3(6.66f, -5.0f, i * 3.33f));
		bez.push_back(glm::vec3(10.0f, 0.0f, i * 3.33f));
	}

	//ter->SetBezierControlPoints(bez);
	//ter->GenerateBezierSurface();

	ter->GenerateDiamondSquareSurface(10, 7, 0.f, -5.8f, 3.5f, 0.55f);

	//ter->GenerateVertices();
	//ter->DisplayVertices();

	mySphere = new Sphere(glm::vec3(5.0f, 3.0f, 5.0f), 2.5f, 30, 30);
	mySphere->SetVertexShader(ProgramId);

	// enable read-only depth buffer
	//glDepthMask(GL_FALSE);

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // culoarea de fond a ecranului

	points = new float[3 * 6];
	colors = new float[3 * 6];

	for (int i = 0; i < 3 * 6; i += 6)
	{
		points[i] = points[i + 1] = points[i + 2] = points[i + 3] = points[i + 4] = points[i + 5] = 0.0f;
		
		if (i == 0) points[i + 3] = 1.0f;
		if (i == 6) points[i + 4] = 1.0f;
		if (i == 12) points[i + 5] = 1.0f;
	}

	for (int i = 0; i < 3 * 6; ++i)
		colors[i] = 0.0f;

	colors[0] = colors[3] = 1.0f;
	colors[7] = colors[10] = 1.0f;
	colors[14] = colors[17] = 1.0f;
	/*
	for (int i = 0; i < 18; i += 3)
	{
		cout << points[i] << ' ' << points[i + 1] << ' ' << points[i + 2] << ' ';
	}

	cout << "\ncolors\n";
	for (int i = 0; i < 18; i += 3)
	{
		cout << colors[i] << ' ' << colors[i + 1] << ' ' << colors[i + 2] << ' ';
	}
	*/
	return true;
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//DestroyVBO();
	//ter->CleanUp();
	//ter->SmoothTerrain(2);
	ter->Update();

	CreateVBO();
	if (isWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
	}
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//cout << Obsx << ' ' << Obsy << ' ' << Obsz << '\n';

	//cout << AngleXoZ << ' ' << cameraSphereRadius << ' ' << Obsz << '\n';

	// se schimba pozitia observatorului
	float aXoZ = M_PI * AngleXoZ / 180.0f;
	float aYoZ = M_PI * AngleYoZ / 180.0f;

	Obsx = CenterX;// +cameraSphereRadius * cos(aXoZ) * sin(aYoZ);
	Obsy = CenterY+ cameraSphereRadius * sin(aXoZ) * sin(aYoZ);
	Obsz = CenterZ + cameraSphereRadius * sin(aXoZ);
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);

	// pozitia punctului de referinta
	// Refx=Obsx; Refy=Obsy;
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz);

	// verticala din planul de vizualizare 
	glm::vec3 Vert = glm::vec3(0.0f, 0.0f, 1.0f);

	view = glm::lookAt(Obs, PctRef, Vert);

	// cout << Obsx << '\n';
	// model=view;
	// displayMatrix();

	//projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
	model = glm::mat4(1.0f);

	modelMatrixLocation = glGetUniformLocation(ProgramId, "model");
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &model[0][0]);
	viewLocation = glGetUniformLocation(ProgramId, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projLocation = glGetUniformLocation(ProgramId, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// desenare puncte din colturi si axe
	glPointSize(5.0f);
	glLineWidth(1.0f);
	
	glDrawArrays(GL_POINTS, 0, 6);

	ter->Draw();

	mySphere->Draw();

	glutSwapBuffers();
}

void Cleanup()
{
	ter->CleanUp();
}

void MousePassiveMotionFunction(int x, int y)
{
	/*
	float radians = float(PI*(fov - 90.0f) / 180.0f);

	Obsx = Refx + sin(radians) * y;
	Obsz = Refz + cos(radians) * y;
	Obsy = Refy + y / 2.0f;
	*/
	if (lastMouseX < 0)
	{
		lastMouseX = x;
		lastMouseY = y;
	}

	AngleXoZ = (AngleXoZ + (y - lastMouseY) * scrollSpeed);
	if (AngleXoZ < 0) AngleXoZ += 360.0f;
	if (AngleXoZ >= 360.0f) AngleXoZ -= 360.0f;

	AngleYoZ = (AngleYoZ + (x - lastMouseX) * scrollSpeed);
	if (AngleYoZ < 0) AngleYoZ += 360.0f;
	if (AngleYoZ >= 360.0f) AngleYoZ -= 360.0f;

	lastMouseX = x;
	lastMouseY = y;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 3D Terrain Generator");
	glewInit();
	if (!Initialize())
	{
		cout << "Program failed to initialize !\n";
		return 0;
	}
	glutDisplayFunc(RenderFunction);
	glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMotionFunc(MousePassiveMotionFunction);
	glutMainLoop();

	getchar();
}

