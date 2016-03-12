#include "Terrain.h"
#include "RNG.h"
#include <iostream>

Terrain::Terrain(int width, int height, int rowLen, int colLen, glm::vec3 pos)
{
	m_width = width;
	m_height = height;
	m_rowNum = rowLen;
	m_colNum = colLen;
	m_centerPos = glm::vec3(m_pos.x + m_width / 2, m_pos.y, m_pos.z + m_height / 2);

	m_nrIndicesToDraw = 3;
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
			
			m_texcoords.push_back(0.0f);
			m_texcoords.push_back(1.0f);
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
			m_colors.push_back(RNG::GetRandFloat());
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

void Terrain::Draw()
{
	//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	CreateVBO();
	ApplyTransform();
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferId);
	//glDrawArrays(GL_POINTS, 0, m_normals.size());
}

void Terrain::Update()
{
	//m_rotationAngles.x += 0.001f;
	//m_rotationAngles.y += 0.001f;
	m_rotationAngles.z += 0.001f;

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
		x.x += RNG::GetRandFloat(-4.1f, 5.3f);
		x.y += RNG::GetRandFloat(-3.3f, 4.0f);
		x.z += RNG::GetRandFloat(-5.2f, 3.4f);
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

				if (RNG::GetRandFloat(0.0f, 1.0f) < 0.15f) averageHeight += RNG::GetRandFloat(rngLowRange, rngHighRange);
				// Add random value to averageHeight and set the midpoint height
				SetGridPointCoord(i + PL_D2, j + PL_D2, 1, averageHeight + RNG::GetRandFloat(rngLowRange, rngHighRange));

				// Diamond Step
				SetGridPointCoord(i + PL_D2, j, 1, GetDiamondAverage(i + PL_D2, j, PL_D2) + RNG::GetRandFloat(rngLowRange, rngHighRange));
				SetGridPointCoord(i, j + PL_D2, 1, GetDiamondAverage(i, j + PL_D2, PL_D2) + RNG::GetRandFloat(rngLowRange, rngHighRange));

				SetGridPointCoord(i + PL_D2, j + PL2, 1, GetDiamondAverage(i + PL_D2, j + PL2, PL_D2) + RNG::GetRandFloat(rngLowRange, rngHighRange));
				SetGridPointCoord(i + PL2, j + PL_D2, 1, GetDiamondAverage(i + PL2, j + PL_D2, PL_D2) + RNG::GetRandFloat(rngLowRange, rngHighRange));
			}
		}

		rngLowRange *= rngDivisionValue;
		rngHighRange *= rngDivisionValue;
	}

	for (int i = 0; i <= 7; ++i)
		SmoothTerrain(3);

	GenerateNormals();
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
	//GenerateNormals();
	CreateBuffers();
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

glm::vec3 Terrain::GetGridPointCoordVect(int row, int col)
{
	if (!IsValidGridCoord(row, col)) return glm::vec3(-1.0f);
	return glm::vec3(m_vertices[3 * (row * m_colNum + col)], m_vertices[3 * (row * m_colNum + col) + 1], m_vertices[3 * (row * m_colNum + col) + 2]);
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

void Terrain::SmoothTerrain(int squareWidth)
{
	vector<GLfloat> m_newYCoords;

	//int dx[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	//int dy[9] = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	int width = (squareWidth - 1) / 2;

	for (int i = 0; i < m_rowNum; ++i)
		for (int j = 0; j < m_colNum; ++j)
		{
			int nr = 0;
			float aver = 0.0f;
			for (int dx = -width; dx <= width; ++ dx)
				for (int dy = -width; dy <= width; ++dy)
				{
					if (IsValidGridCoord(i + dx, j + dy))
					{
						++nr;
						aver += GetGridPointCoord(i + dx, j + dy, 1);
					}
				}

			m_newYCoords.push_back(aver / (float) nr);
		}

		for (int t = 0; t < m_newYCoords.size(); ++t)
		{
			m_vertices[3 * t + 1] = m_newYCoords[t];
		}
}

void Terrain::GenerateNormals()
{
	m_normals.clear();
	m_normals.resize(m_vertices.size());

	for (int i = 0; i < m_rowNum; ++i)
	{
		for (int j = 0; j < m_colNum; ++j)
		{
			int vertIndex = i * m_colNum + j;
			int nr = 0;
			int sgn = 0;
			int dx, dy;
			int I1, J1, I2, J2;
			int tt;

			glm::vec3 currVect = GetGridPointCoordVect(i, j);
			glm::vec3 ans = glm::vec3(0.0f);

			for (int t = 0; t < 4; ++t)
			{
				// first
				if (t > 1) sgn = -1;
				else sgn = 1;

				dx = sgn * (t & 1);
				dy = sgn * (1 - (t & 1));

				I1 = i + dx; J1 = j + dy;

				// second
				tt = (t + 1) % 4;

				if (tt > 1) sgn = -1;
				else sgn = 1;

				dx = sgn * (tt & 1);
				dy = sgn * (1 - (tt & 1));

				I2 = i + dx; J2 = j + dy;

				if (IsValidGridCoord(I1, J1) && IsValidGridCoord(I2, J2))
				{
					glm::vec3 v1 = GetGridPointCoordVect(I1, J1) - currVect;
					glm::vec3 v2 = GetGridPointCoordVect(I2, J2) - currVect;
					glm::vec3 cross_prod = glm::cross(v2, v1);
					ans += glm::normalize(cross_prod);
					++nr;
				}
			}

			ans = 1.0f * (ans / (float) nr);
			m_normals[3 * vertIndex] = ans.x;
			m_normals[3 * vertIndex + 1] = ans.y;
			m_normals[3 * vertIndex + 2] = ans.z;
		}
	}

}