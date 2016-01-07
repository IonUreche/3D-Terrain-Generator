#include "Terrain.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <climits>

Terrain::Terrain(int width = 10, int height = 10, int rowLen = 10, int colLen = 10)
{
	srand((unsigned int)time(NULL));
	// Just to make sure we have even number of vertices in each row and column
	if (m_rowNum & 1) ++m_rowNum;
	if (m_colNum & 1) ++m_colNum;
	m_width = width;
	m_height = height;
	m_rowNum = rowLen;
	m_colNum = colLen;

	m_nrIndicesToDraw = 3;
	
	CreateTerrain();
}

Terrain::~Terrain()
{
}

void Terrain::GenerateVertices()
{
	float sliceW = (float)m_width / (float)(m_rowNum - 1);
	float sliceH = (float)m_height / (float)(m_colNum - 1);

	m_vertices.clear();

	for (int i = 0; i < m_rowNum; ++i)
		for (int j = 0; j < m_colNum; ++j)
		{
			m_vertices.push_back(j * sliceW);
			m_vertices.push_back(0.0f);
			m_vertices.push_back(i * sliceH);
		}
}

void Terrain::DisplayVertices()
{
	int ind = 0;
	for (auto &vert : m_vertices)
	{
		cout << vert << ' ';
		ind = (ind + 1) % 3;
		if (ind == 0)
		{
			cout << '\n';
		}
	}
}

void Terrain::GenerateColors()
{
	for (int i = 0; i < m_rowNum * m_colNum; ++i)
	{
		for(int t = 0; t < 3; ++ t) 
			m_colors.push_back(0.0f);
	}
}

void Terrain::GenerateIndexes()
{
	for (int i = 0; i < m_rowNum - 1; ++i)
	{
		for (int j = 0; j < m_colNum; ++j)
		{
			m_indices.push_back(m_colNum * i + j);
			m_indices.push_back((i + 1) * m_colNum + j);
		}
		//m_indices.push_back(m_rowNum * m_colNum);
		//m_indices.push_back(m_rowNum * m_colNum);
		m_indices.push_back((i + 1) * m_colNum + m_colNum - 1);
		if (i != m_rowNum - 2)
			m_indices.push_back((i + 1) * m_colNum);
	}
}

void Terrain::DisplayIndexes()
{
	int ind = 0;
	for (auto &x : m_indices)
	{
		cout << x << ' ';
		ind = (ind + 1) % m_colNum;
		if (ind == 0)
		{
			cout << '\n';
		}
	}
}

void Terrain::CreateVBO()
{
	// se creeaza un buffer nou se seteaza ca buffer curent si punctele sunt "copiate" in bufferul curent
	//glGenVertexArrays(1, &VaoId);
	//glBindVertexArray(VaoId);

	//glGenBuffers(1, &VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	// se activeaza lucrul cu atribute; atributul 0 = pozitie
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru culoare
	//glGenBuffers(1, &ColorBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m_colors.size(), &m_colors[0], GL_STATIC_DRAW);
	// atributul 1 =  culoare
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// un nou buffer, pentru indici
	//glGenBuffers(1, &indicesBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(m_colNum * m_rowNum);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(2);
	//glVertexAttribPointer(2, 1, GL_SHORT, GL_FALSE, 0, 0);
}

void Terrain::CreateBuffers()
{
	glGenBuffers(1, &VboId);
	glGenBuffers(1, &ColorBufferId);
	glGenBuffers(1, &indicesBufferId);
}

void Terrain::CleanUp()
{
	DestroyVBO();
}

void Terrain::DestroyVBO()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorBufferId);
	glDeleteBuffers(1, &VboId);
	glDeleteBuffers(1, &indicesBufferId);

	//glBindVertexArray(0);
	//glDeleteVertexArrays(1, &VaoId);
}

void Terrain::Draw()
{
	//glBindVertexArray(VboId);
	//glDrawArrays(GL_POINTS, 0, m_nrIndicesToDraw);
	//
	
	//glBindBuffer(GL_ARRAY_BUFFER, VboId);
	//glBindBuffer(GL_ARRAY_BUFFER, ColorBufferId);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBufferId);
	//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	CreateVBO();
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Terrain::Update()
{
	if (m_nrIndicesToDraw + 1 > m_indices.size())
		m_nrIndicesToDraw = m_indices.size();
	else
		m_nrIndicesToDraw += 1;
}

void Terrain::SetBezierControlPoints(vector<glm::vec3>& controlPoints)
{
	m_bezierControlPoints = controlPoints;
	for (auto &x : m_bezierControlPoints)
	{
		x.x += GetRandFloat(-4.1f, 5.3f);
		x.y += GetRandFloat(-3.3f, 4.0f);
		x.z += GetRandFloat(-5.2f, 3.4f);
	}
		
}

float Terrain::BernsteinPolynomial3(int index, float u)
{
	if (index > 3 || index < 0) return -1.0f;
	float ans = pow(1 - u, 3 - index) * pow(u, index);
	if (index == 1 || index == 2) ans *= 3.0f;
	return ans;
}

void Terrain::GenerateBezierSurface()
{
	for (int i = 0; i < m_rowNum; ++i)
	{
		for (int j = 0; j < m_colNum; ++j)
		{
			float u = (float)i / (float)(m_rowNum - 1);
			float v = (float)j / (float)(m_colNum - 1);

			int v_ind = 3 * (i * m_colNum + j);
			m_vertices[v_ind] = m_vertices[v_ind + 1] = m_vertices[v_ind + 2] = 0.0f;

			glm::vec3 new_point(0.f, 0.f, 0.f);

			for (int ii = 0; ii < 4; ++ii)
			{
				for (int jj = 0; jj < 4; ++jj)
				{
					new_point += m_bezierControlPoints[ii * 4 + jj] * BernsteinPolynomial3(ii, u) * BernsteinPolynomial3(jj, v);
				}
			}
			m_vertices[v_ind] = new_point.x;
			m_vertices[v_ind + 1] = new_point.y;
			m_vertices[v_ind + 2] = new_point.z;
		}
	}
}

void Terrain::GenerateDiamondSquareSurface(int terrainSize, int terrainGridSizeInPowerOfTwo, float startingPointsHeight,
	float rngLowRange, float rngHighRange, float rngDivisionValue)
{
	ClearVertexData();
	SetGridSize((1 << terrainGridSizeInPowerOfTwo) + 2, (1 << terrainGridSizeInPowerOfTwo) + 2);
	SetSize(terrainSize, terrainSize);
	CreateTerrain();

	// Set the height of initial four corners
	SetGridPointCoord(0, 0, 1, startingPointsHeight);
	SetGridPointCoord(0, (1 << terrainGridSizeInPowerOfTwo), 1, startingPointsHeight);
	SetGridPointCoord((1 << terrainGridSizeInPowerOfTwo), 0, 1, startingPointsHeight);
	SetGridPointCoord((1 << terrainGridSizeInPowerOfTwo), (1 << terrainGridSizeInPowerOfTwo), 1, startingPointsHeight);

	int lim = (1 << terrainGridSizeInPowerOfTwo) + 1;
	for (int len = terrainGridSizeInPowerOfTwo; len > 0; --len)
	{
		int PL2 = 1 << len;
		int PL_D2 = 1 << (len - 1);
		float averageHeight = 0.0f;

		for (int i = 0; i < lim; i += PL2)
		{
			for (int j = 0; j < lim; j += PL2)
			{
				// Take The Average of Corner points
				averageHeight = 0.0f;
				averageHeight = (GetGridPointCoord(i, j, 1) + GetGridPointCoord(i + PL_D2, j, 1)
					+ GetGridPointCoord(i + PL_D2, j, 1) + GetGridPointCoord(i + PL_D2, j + PL_D2, 1)) / 4.0f;

				if (GetRandFloat(0.0f, 1.0f) < 0.15f) averageHeight += GetRandFloat(rngLowRange, rngHighRange);
				// Add random value to averageHeight and set the midpoint height
				SetGridPointCoord(i + PL_D2, j + PL_D2, 1, averageHeight + GetRandFloat(rngLowRange, rngHighRange));

				// Diamond Step
				SetGridPointCoord(i + PL_D2, j, 1, GetDiamondAverage(i + PL_D2, j, PL_D2) + GetRandFloat(rngLowRange, rngHighRange));
				SetGridPointCoord(i, j + PL_D2, 1, GetDiamondAverage(i, j + PL_D2, PL_D2) + GetRandFloat(rngLowRange, rngHighRange));

				SetGridPointCoord(i + PL_D2, j + PL2, 1, GetDiamondAverage(i + PL_D2, j + PL2, PL_D2) + GetRandFloat(rngLowRange, rngHighRange));
				SetGridPointCoord(i + PL2, j + PL_D2, 1, GetDiamondAverage(i + PL2, j + PL_D2, PL_D2) + GetRandFloat(rngLowRange, rngHighRange));
			}
		}

		rngLowRange *= rngDivisionValue;
		rngHighRange *= rngDivisionValue;
	}

}

void Terrain::ClearVertexData()
{
	m_indices.clear();
	m_vertices.clear();
	m_colors.clear();
	m_normals.clear();
}

void Terrain::CreateTerrain()
{
	GenerateVertices();
	GenerateColors();
	GenerateIndexes();
	CreateBuffers();
	CreateVBO();
}

void Terrain::SetSize(int width, int height)
{
	m_width = width;
	m_height = height;
}

void Terrain::SetGridSize(int rowNum, int colNum)
{
	m_rowNum = rowNum;
	m_colNum = colNum;
}

bool Terrain::IsValidGridCoord(int row, int col)
{
	int tmp = 3 * (row * m_colNum + col);
	return 0 <= tmp && tmp < m_vertices.size();
}

float Terrain::GetGridPointCoord(int row, int col, int coordIndex)
{
	if (coordIndex < 0 || coordIndex > 2) return -1.0f;
	if (!IsValidGridCoord(row, col)) return -1.0f;
	return m_vertices[3 * (row * m_colNum + col) + coordIndex];
}

void Terrain::SetGridPointCoord(int row, int col, int coordIndex, float value)
{
	if (coordIndex < 0 || coordIndex > 2) return ;
	if (! IsValidGridCoord(row, col)) return ;
	m_vertices[3 * (row * m_colNum + col) + coordIndex] = value;
}

float Terrain::GetRandFloat(float rangeMin, float rangeMax)
{
	if (rangeMin > rangeMax) return -1.0f;
	float rZeroOne = ((float) rand() / (float) MAXINT16);
	return rZeroOne * (rangeMax - rangeMin) + rangeMin;
}

float Terrain::GetDiamondAverage(int row, int col, int step)
{
	int nr = 0;
	float sum = 0;

	if (IsValidGridCoord(row - step, col)){ sum += GetGridPointCoord(row - step, col, 1); nr++; }
	if (IsValidGridCoord(row, col - step)){ sum += GetGridPointCoord(row, col - step, 1); nr++; }
	if (IsValidGridCoord(row + step, col)){ sum += GetGridPointCoord(row + step, col, 1); nr++; }
	if (IsValidGridCoord(row, col + step)){ sum += GetGridPointCoord(row, col + step, 1); nr++; }
	return sum / (float) nr;
}