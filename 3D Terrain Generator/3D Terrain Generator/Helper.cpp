#include "stdafx.h"
#include "Helper.h"


Helper::Helper()
{
	if (g_helper == nullptr)
		g_helper = new Helper;
}

Helper::~Helper()
{
	if (g_helper != nullptr)
		delete g_helper;
}

const GLchar* Helper::LoadShaderFromFile(char * filename){

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

bool Helper::CreateShaders(char* VertexShaderFileName, char* FragmentShaderFileName, GLuint &ProgramId, GLuint &VertexShaderId, GLuint &FragmentShaderId)
{
	GLint compiled_ok;
	const GLchar *VertexShader, *FragmentShader;

	VertexShader = LoadShaderFromFile(VertexShaderFileName);
	FragmentShader = LoadShaderFromFile(FragmentShaderFileName);

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

void Helper::DestroyShaders(GLuint &ProgramId)
{
	glDeleteProgram(ProgramId);
	ProgramId = 0;
}

Helper *g_helper = nullptr;