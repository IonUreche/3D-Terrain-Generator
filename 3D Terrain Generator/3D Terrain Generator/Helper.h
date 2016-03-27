#pragma once

using namespace std;

class Helper{
public:
	Helper();
	~Helper();
	const GLchar* LoadShaderFromFile(char * filename);
	bool CreateShaders(char* VertexShaderFileName, char* FragmentShaderFileName,
						GLuint &ProgramId, GLuint &VertexShaderId, GLuint &FragmentShaderId);
	void DestroyShaders(GLuint &ProgramId);
};

extern Helper *g_helper;