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
	void GenerateDiamondSquareSurface(int terrainSize, int terrainGridSizeInPowerOfTwo, float startingPointsHeight,
		                              float rngLowRange, float rngHighRange, float rngDivisionValue);

	void SetSize(int width, int height);
	void SetGridSize(int rowNum, int colNum);

	float GetGridPointCoord(int row, int col, int coordIndex);
	void SetGridPointCoord(int row, int col, int coordIndex, float value);

private:

	int m_width;
	int m_height;
	int m_rowNum;
	int m_colNum;
	int m_nrIndicesToDraw;

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

	void CreateBuffers();

	void CreateTerrain();
	void ClearVertexData();

	float GetDiamondAverage(int row, int col, int step);

	float GetRandFloat(float rangeMin, float rangeMax);
	bool IsValidGridCoord(int row, int col);
};

