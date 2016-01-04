#pragma once

#include <vector>
#include <GL/glew.h> 
#include <GL/freeglut.h>
using namespace std;

class Terrain
{
public:
	Terrain(int width, int height, int rowLen, int colLen);
	~Terrain();

	void draw();

	void update();

private:

	int m_width;
	int m_height;
	int m_rowLen;
	int m_colLen;
	int m_nrIndicesToDraw;

	GLuint
		VaoId,
		VboId,
		elementBufferId,
		ColorBufferId,
		VertexShaderId,
		FragmentShaderId,
		ProgramId;

	vector<GLint> m_indices;
	vector<GLfloat> m_vertices;
	vector<GLfloat> m_colors;
	vector<GLfloat> m_normals;

	void GenerateVertices();
	void DisplayVertices();
	void GenerateIndexes();
	void DisplayIndexes();
	void GenerateColors();
	void CreateVBO();
	void DestroyVBO();
};

