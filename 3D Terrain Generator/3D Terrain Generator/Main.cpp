
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

//#include "Sphere.h"
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
myMatrixLocation,
viewLocation,
projLocation,
matrRotlLocation,
codColLocation;

//Sphere *mySphere;

int codCol;
float PI = 3.141592;

// matrice utilizate
glm::mat4 myMatrix, matrRot;

// elemente pentru matricea de vizualizare
float Obsx = 5.0, Obsy = 2.0, Obsz = -5.f;
float Refx = 5.0f, Refy = 150.0f, Refz = -5.0f;
float Vx = 0.0;
glm::mat4 view;

Terrain *ter;

// elemente pentru matricea de proiectie
float width = 800, height = 600, xwmin = -800.f, xwmax = 800, ywmin = -600, ywmax = 600, znear = 0.1f, zfar = 1000.0f, fov = 45;
glm::mat4 projection;

void displayMatrix()
{
	for (int ii = 0; ii < 4; ii++)
	{
		for (int jj = 0; jj < 4; jj++)
			cout << myMatrix[ii][jj] << "  ";
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
		Obsz += 10.f;
		break;
	case '-':
		Obsz -= 10.f;
		break;
	case 'e':
		Obsy += 2;
		//Refy += 2;
		break;
	case 'q':
		Obsy -= 2;
		//Refy -= 2;
		break;
	case 'o':
		ter->update();
		break;
	case 'v':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	case 'b':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (key == 27)
		exit(0);
}
void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT:
		Obsx -= 2; 
		//Refx -= 2;
		break;
	case GLUT_KEY_RIGHT:
		Obsx += 2;
		//Refx += 2;
		break;
	case GLUT_KEY_UP:
		Obsz += 2;
		//Refz += 2;
		break;
	case GLUT_KEY_DOWN:
		Obsz -= 2;
		//Refz -= 2;
		break;
	}
}

void CreateVBO(void)
{
	// varfurile 
	GLfloat Vertices[] = {
		// cele 4 varfuri din colturi 
		-390.0f, -290.0f, 0.0f, 1.0f,
		390.0f, -290.0f, 0.0f, 1.0f,
		390.0f, 290.0f, 0.0f, 1.0f,
		-390.0f, 290.0f, 0.0f, 1.0f,
		// varfuri pentru axe
		-400.0f, 0.0f, 0.0f, 1.0f,
		400.0f, 0.0f, 0.0f, 1.0f,
		0.0f, -300.0f, 0.0f, 1.0f,
		0.0f, 300.0f, 0.0f, 1.0f,
		// varfuri pentru dreptunghi
		-50.0f, -50.0f, 0.0f, 1.0f,
		50.0f, -50.0f, 0.0f, 1.0f,
		50.0f, 50.0f, 0.0f, 1.0f,
		-50.0f, 50.0f, 0.0f, 1.0f,

	};



	// culorile varfurilor din colturi
	GLfloat Colors[] = {
		1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f,
	};


	// se creeaza un buffer nou
	glGenBuffers(1, &VboId);
	// este setat ca buffer curent
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	// punctele sunt "copiate" in bufferul curent
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	// se creeaza / se leaga un VAO (Vertex Array Object) - util cand se utilizeaza mai multe VBO
	glGenVertexArrays(1, &VaoId);
	glBindVertexArray(VaoId);
	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	// 
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);


}
void DestroyVBO(void)
{


	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VaoId);


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

bool Initialize()
{

	myMatrix = glm::mat4(1.0f);

	matrRot = glm::rotate(glm::mat4(1.0f), PI / 8, glm::vec3(0.0, 0.0, 1.0));

	if (CreateShaders() == false)
	{
		printf("%s\n", "Shader could not be created");
		return false;
	}

	//mySphere = new Sphere(glm::vec3(1.0f, 1.0f, 1.0f), 1, 7, 14);
	ter = new Terrain(10, 10, 10, 10);
	//ter->GenerateVertices();
	//ter->DisplayVertices();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // culoarea de fond a ecranului
	return true;
}

void RenderFunction(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	cout << Obsx << ' ' << Obsy << ' ' << Obsz << '\n';

	// se schimba pozitia observatorului
	glm::vec3 Obs = glm::vec3(Obsx, Obsy, Obsz);

	// pozitia punctului de referinta
	// Refx=Obsx; Refy=Obsy;
	glm::vec3 PctRef = glm::vec3(Refx, Refy, Refz);

	// verticala din planul de vizualizare 
	glm::vec3 Vert = glm::vec3(Vx, 1.0f, 0.0f);

	view = glm::lookAt(Obs, PctRef, Vert);

	// cout << Obsx << '\n';
	// myMatrix=view;
	// displayMatrix();

	//projection = glm::ortho(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	// projection = glm::frustum(xwmin, xwmax, ywmin, ywmax, znear, zfar);
	projection = glm::perspective(fov, GLfloat(width) / GLfloat(height), znear, zfar);
	myMatrix = glm::mat4(1.0f);
	CreateVBO();
	CreateShaders();

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	viewLocation = glGetUniformLocation(ProgramId, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projLocation = glGetUniformLocation(ProgramId, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	// desenare puncte din colturi si axe
	glPointSize(10.0f);
	glLineWidth(5.0f);
	//glDrawArrays(GL_POINTS, 0, 4);
	//glDrawArrays(GL_LINES, 4, 4);

	//myMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 100.0f));
	//myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	//glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);

	//mySphere->draw();
	// 
	ter->draw();

	myMatrix = matrRot;
	CreateShaders();

	myMatrixLocation = glGetUniformLocation(ProgramId, "myMatrix");
	glUniformMatrix4fv(myMatrixLocation, 1, GL_FALSE, &myMatrix[0][0]);
	viewLocation = glGetUniformLocation(ProgramId, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &view[0][0]);
	projLocation = glGetUniformLocation(ProgramId, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &projection[0][0]);

	glutSwapBuffers();
}


void MousePassiveMotionFunction(int x, int y)
{
	float radians = float(PI*(fov - 90.0f) / 180.0f);

	Obsx = Refx + sin(radians) * y;
	Obsz = Refz + cos(radians) * y;
	Obsy = Refy + y / 2.0f;
}

void Cleanup(void)
{
	DestroyShaders();
	DestroyVBO();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(500, 500);
	glutInitWindowSize(800, 600);
	glutCreateWindow("OpenGL 3D Scene");
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

