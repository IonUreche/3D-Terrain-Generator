#pragma once
#include "Model.h"
#include <Image.hpp>

using namespace std;

class Terrain : public Model
{
public:
	Terrain(int width = 10, int height = 10, int rowLen = 10, int colLen = 10, glm::vec3 pos = glm::vec3(0.0f, 0.0f, 0.0f));
	~Terrain();

	void Draw() override;
	void Draw(glm::mat4 c_view, glm::mat4 c_projection);

	void Update() override;

	void SetBezierControlPoints(vector<glm::vec3>& controlPoints);
	void SetDefaultBezierControlPoints();
	void SetBezierControlPointHeight(int index, double newHeight);
	double GetBezierControlPointHeight(int index);
	void GenerateBezierSurface(int isAdditive, double sizeW, double sizeH, double HeightMultFactor, int rows, int columns, int generateControlPoints = 1, int recreate = 1);
	//void CleanUp();
	void GenerateDiamondSquareSurface(int terrainSize, int terrainGridSizeInPowerOfTwo, float startingPointsHeight,
		                              float rngLowRange, float rngHighRange, float rngDivisionValue);
	void GenerateDiamondSquareSurface2(int terrainSize, int terrainGridSizeInPowerOfTwo, double roughness, double heightScaleValue, int numberOfSmoothingIterations = 0);

	void ApplyPerlinNoise(int octaves = 5, double persistence = 0.5, double coordsMultFactor = 3, double noiseMultFactor = 1000, double z = 0);

	void SetSize(int width, int height);
	void SetGridSize(int rowNum, int colNum);

	glm::vec3 GetGridPointCoordVect(int row, int col);
	float GetGridPointCoord(int row, int col, int coordIndex);
	void SetGridPointCoord(int row, int col, int coordIndex, float value);

	void SmoothTerrain(int squareWidth);
	void Apply3x3Filter(int type = 0);
	void Apply5x5Filter(vector<float> f);
	void ExportAsImage(string imgFileName);

	void ScaleHeight(double scaleValue);

private:

	int m_width;
	int m_height;
	int m_rowNum;
	int m_colNum;
	int m_nrIndicesToDraw;
	float m_maxHeight;
	float m_minHeight;

	vector<glm::vec3> m_bezierControlPoints;
	std::vector<GLdouble> m_backupVertices;

	float BernsteinPolynomial3(int index, float u);

	void GenerateVertices() override;
	void DisplayVertices();
	void GenerateIndexes() override;
	void DisplayIndexes();
	void GenerateColors() override;
	void GenerateNormals() override;

	void CreateTerrain();
	void ClearVertexData();

	void UpdateMinMaxHeight();
	void UpdateMinMaxHeight(double min, double max);
	void GetGradient(int type, double alpha, sf::Color &color);

	float GetDiamondAverage(int row, int col, int step);
	inline bool IsValidGridCoord(int row, int col);
};

