#pragma once

#include <vector>
#include <GL/glew.h> 
#include <GL/freeglut.h>
#include "glm/vec3.hpp"  
using namespace std;

class Terrain
{
public:
	Terrain(int width, int height, int rowLen, int colLen);
	~Terrain();

	void Draw();

	void Update();

	void SetBezierControlPoints(vector<glm::vec3>& controlPoints);
	void GenerateBezierSurface();
	void CleanUp();

private:

	int m_width;
	int m_height;
	int m_rowLen;
	int m_colLen;
	int m_nrIndicesToDraw;

	float *p_points;
	int *p_indices;

	GLuint
		VaoId,
		VboId,
		indicesBufferId,
		ColorBufferId,
		VertexShaderId,
		FragmentShaderId,
		ProgramId;

	vector<GLuint> m_indices;
	vector<GLfloat> m_vertices;
	vector<GLfloat> m_colors;
	vector<GLfloat> m_normals;

	vector<glm::vec3> m_bezierControlPoints;

	float BernsteinPolynomial3(int index, float u);

	void GenerateVertices();
	void DisplayVertices();
	void GenerateIndexes();
	void DisplayIndexes();
	void GenerateColors();
	void CreateVBO();
	void DestroyVBO();
};

