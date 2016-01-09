#pragma once
#include "Model.h"

using namespace std;

class Terrain : public Model
{
public:
	Terrain(int width = 10, int height = 10, int rowLen = 10, int colLen = 10, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
	~Terrain();

	void Draw() override;

	void Update() override;

	void SetBezierControlPoints(vector<glm::vec3>& controlPoints);
	void GenerateBezierSurface();
	//void CleanUp();
	void GenerateDiamondSquareSurface(int terrainSize, int terrainGridSizeInPowerOfTwo, float startingPointsHeight,
		                              float rngLowRange, float rngHighRange, float rngDivisionValue);

	void SetSize(int width, int height);
	void SetGridSize(int rowNum, int colNum);

	float GetGridPointCoord(int row, int col, int coordIndex);
	void SetGridPointCoord(int row, int col, int coordIndex, float value);

	void SmoothTerrain(int squareWidth);

private:

	int m_width;
	int m_height;
	int m_rowNum;
	int m_colNum;
	int m_nrIndicesToDraw;
	/*
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
	*/
	vector<glm::vec3> m_bezierControlPoints;

	float BernsteinPolynomial3(int index, float u);
	/*
	
	void CreateVBO();
	void DestroyVBO();
	
	void CreateBuffers();
	*/
	void GenerateVertices() override;
	void DisplayVertices();
	void GenerateIndexes() override;
	void DisplayIndexes();
	void GenerateColors() override;

	void CreateTerrain();
	void ClearVertexData();

	float GetDiamondAverage(int row, int col, int step);

	float GetRandFloat(float rangeMin, float rangeMax);
	bool IsValidGridCoord(int row, int col);
};

