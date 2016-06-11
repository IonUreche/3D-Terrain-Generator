#include "stdafx.h"

#include "Terrain.h"
#include "RNG.h"
#include "PerlinNoise.h"
#include <Image.hpp>

Terrain::Terrain(int width, int height, int rowLen, int colLen, glm::vec3 center_pos)
{
	m_width = width;
	m_height = height;
	m_rowNum = rowLen;
	m_colNum = colLen;
	m_centerPos = center_pos;

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
	m_texcoords.clear();

	int k = 5;
	float pos_x = m_centerPos.x - m_width / 2;
	float pos_z = m_centerPos.z - m_height / 2;

	float step = 1.0f / (float)(k);

	for (int i = 0; i < m_rowNum; ++i)
		for (int j = 0; j < m_colNum; ++j)
		{
			m_vertices.push_back(pos_x + j * sliceW); 
			m_vertices.push_back(m_pos.y);
			m_vertices.push_back(pos_z + i * sliceH);
			
			/*
			float t_x = (float)(i & 1), t_y = (float)(j & 1);
			m_texcoords.push_back((float) i / (float) (m_rowNum - 1));
			m_texcoords.push_back((float) j / (float)(m_colNum - 1));
			*/
			int val_i = i % (2 * k) - k;
			if (val_i < 0) val_i = -val_i;
			float t_xi = 1.0f - val_i * step;

			int val_j = j % (2 * k) - k;
			if (val_j < 0) val_j = -val_j;
			float t_xj = 1.0f - val_j * step;

			m_texcoords.push_back(t_xi);
			m_texcoords.push_back(t_xj);
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
			m_colors.push_back(RNG::getFloat());
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

		if (i != m_rowNum - 2)
		{
			m_indices.push_back((i + 1) * m_colNum + m_colNum - 1);
			m_indices.push_back((i + 1) * m_colNum);
		}
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

}

void Terrain::Draw(glm::mat4 c_view, glm::mat4 c_projection)
{
	//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glUseProgram(ProgramId);

	GLuint modelMatrixLocation = glGetUniformLocation(ProgramId, "model");
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &m_transform[0][0]);
	GLuint viewLocation = glGetUniformLocation(ProgramId, "view");
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &c_view[0][0]);
	GLuint projLocation = glGetUniformLocation(ProgramId, "projection");
	glUniformMatrix4fv(projLocation, 1, GL_FALSE, &c_projection[0][0]);

	GLuint minHeightLocation = glGetUniformLocation(ProgramId, "minHeight");
	glUniform1f(minHeightLocation, m_minHeight);

	GLuint maxHeightLocation = glGetUniformLocation(ProgramId, "maxHeight");
	glUniform1f(maxHeightLocation, m_maxHeight);

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

	if (m_nrIndicesToDraw + 1 > (int) m_indices.size())
		m_nrIndicesToDraw = m_indices.size();
	else
		m_nrIndicesToDraw += 1;
}

void Terrain::SetBezierControlPoints(vector<glm::vec3>& controlPoints)
{
	m_bezierControlPoints = controlPoints;
	for (auto &x : m_bezierControlPoints)
	{
		x.x += RNG::getFloat(-4.1f, 5.3f);
		x.y += RNG::getFloat(-3.3f, 4.0f);
		x.z += RNG::getFloat(-5.2f, 3.4f);
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
	ClearVertexData();
	CreateTerrain();
	
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

	UpdateMinMaxHeight();
	GenerateNormals();
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

				if (RNG::getFloat(0.0f, 1.0f) < 0.15f) averageHeight += RNG::getFloat(rngLowRange, rngHighRange);
				// Add random value to averageHeight and set the midpoint height
				SetGridPointCoord(i + PL_D2, j + PL_D2, 1, averageHeight + RNG::getFloat(rngLowRange, rngHighRange));

				// Diamond Step
				SetGridPointCoord(i + PL_D2, j, 1, GetDiamondAverage(i + PL_D2, j, PL_D2) + RNG::getFloat(rngLowRange, rngHighRange));
				SetGridPointCoord(i, j + PL_D2, 1, GetDiamondAverage(i, j + PL_D2, PL_D2) + RNG::getFloat(rngLowRange, rngHighRange));

				SetGridPointCoord(i + PL_D2, j + PL2, 1, GetDiamondAverage(i + PL_D2, j + PL2, PL_D2) + RNG::getFloat(rngLowRange, rngHighRange));
				SetGridPointCoord(i + PL2, j + PL_D2, 1, GetDiamondAverage(i + PL2, j + PL_D2, PL_D2) + RNG::getFloat(rngLowRange, rngHighRange));
			}
		}

		rngLowRange *= rngDivisionValue;
		rngHighRange *= rngDivisionValue;
	}

	 //for(int i = 0; i <= 1; ++i)
	//	SmoothTerrain(3);

	UpdateMinMaxHeight();
	GenerateNormals();
}

void Terrain::GenerateDiamondSquareSurface2(int terrainSize, int terrainGridSizeInPowerOfTwo, double roughness, double heightScaleValue, int numberOfSmoothingIterations)
{
	ClearVertexData();
	int TerrSize = 1 << terrainGridSizeInPowerOfTwo;
	SetGridSize(TerrSize + 1, TerrSize + 1);
	SetSize(terrainSize, terrainSize);
	CreateTerrain();

	double rought = roughness;

	// Set the height of initial four corners
	SetGridPointCoord(0, 0, 1, RNG::getDouble(-rought, rought));
	SetGridPointCoord(0, TerrSize, 1, RNG::getDouble(-rought, rought));
	SetGridPointCoord(TerrSize, 0, 1, RNG::getDouble(-rought, rought));
	SetGridPointCoord(TerrSize, TerrSize, 1, RNG::getDouble(-rought, rought));

	int lim = TerrSize + 1;
	double g1, g2, g3, g4, perturbation;

	for (int len = terrainGridSizeInPowerOfTwo; len > 0; --len)
	{
		int PL2 = 1 << len;
		int PL_D2 = 1 << (len - 1);
		float averageHeight = 0.0f;

		for (int i = 0; i < lim; i += PL2)
		{
			for (int j = 0; j < lim; j += PL2)
			{
				// Square Step - Take The Average of Corner points
				averageHeight = 0.0f;
				g1 = GetGridPointCoord(i, j, 1);
				g2 = GetGridPointCoord(i + PL2, j, 1);
				g3 = GetGridPointCoord(i, j + PL2, 1);
				g4 = GetGridPointCoord(i + PL2, j + PL2, 1);
				averageHeight = (g1 + g2 + g3 + g4) / 4.0f;
				perturbation = RNG::getDouble(-rought, rought);
				averageHeight += perturbation;
				// Add random value to averageHeight and set the midpoint height
				SetGridPointCoord(i + PL_D2, j + PL_D2, 1, averageHeight);
			}
		}

		for (int i = 0; i < lim; i += PL2)
		{
			for (int j = 0; j < lim; j += PL2)
			{
				// Diamond Step
				SetGridPointCoord(i + PL_D2, j, 1, GetDiamondAverage(i + PL_D2, j, PL_D2) + RNG::getDouble(-rought, rought));
				SetGridPointCoord(i, j + PL_D2, 1, GetDiamondAverage(i, j + PL_D2, PL_D2) + RNG::getDouble(-rought, rought));
				SetGridPointCoord(i + PL_D2, j + PL2, 1, GetDiamondAverage(i + PL_D2, j + PL2, PL_D2) + RNG::getDouble(-rought, rought));
				SetGridPointCoord(i + PL2, j + PL_D2, 1, GetDiamondAverage(i + PL2, j + PL_D2, PL_D2) + RNG::getDouble(-rought, rought));
			}
		}

		rought *= roughness;
	}

	for (int i = 0; i < numberOfSmoothingIterations; ++i)
		Apply3x3Filter();

	ScaleHeight(heightScaleValue);
	UpdateMinMaxHeight();
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

inline bool Terrain::IsValidGridCoord(int row, int col)
{
	int tmp = 3 * (row * m_colNum + col);
	return 0 <= tmp && tmp < (int) m_vertices.size();
}

void Terrain::ScaleHeight(double scaleValue)
{
	for (size_t i = 1; i < m_vertices.size(); i += 3)
		m_vertices[i] *= scaleValue;
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

		for (size_t t = 0; t < m_newYCoords.size(); ++t)
		{
			m_vertices[3 * t + 1] = m_newYCoords[t];
		}

	UpdateMinMaxHeight();
	GenerateNormals();
}

void Terrain::Apply3x3Filter(int type)
{
	vector<GLfloat> m_newYCoords;
	
	float f[3][3];
	
	if (type == 0)
	{
		f[0][0] = 0.0625; f[0][1] = 0.125; f[0][2] = 0.0625;
		f[1][0] = 0.125; f[1][1] = 0.250; f[1][2] = 0.125;
		f[2][0] = 0.0625; f[2][1] = 0.125; f[2][2] = 0.0625;
	}
	else
	if (type == 1)
	{
		f[0][0] = 1; f[0][1] = 0; f[0][2] = -1;
		f[1][0] = 0; f[1][1] = 1; f[1][2] = 0;
		f[2][0] = -1; f[2][1] = 0; f[2][2] = 1;
	}
	else if (type == 2)
	{
		f[0][0] = 0; f[0][1] = 1; f[0][2] = 0;
		f[1][0] = 1; f[1][1] = -3; f[1][2] = 1;
		f[2][0] = 0; f[2][1] = 1; f[2][2] = 0;
	}
	else if (type == 3)
	{
		f[0][0] = -1; f[0][1] = -1; f[0][2] = -1;
		f[1][0] = -1; f[1][1] = 9; f[1][2] = -1;
		f[2][0] = -1; f[2][1] = -1; f[2][2] = -1;
	}
	else
	{
		for (int i = 0; i < 9; ++i) f[i / 3][i % 3] = 0;
	}
	
	for (int i = 0; i < m_rowNum; ++i)
	for (int j = 0; j < m_colNum; ++j)
	{
		int nr = 0;
		float sum = 0.0f;
		for (int dx = -1; dx <= 1; ++dx)
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (IsValidGridCoord(i + dx, j + dy))
			{
				++nr;
				sum += GetGridPointCoord(i + dx, j + dy, 1) * f[dx+1][dy+1];
			}
			else
			{
				sum += GetGridPointCoord(i, j, 1) * f[dx + 1][dy + 1];
			}
		}

		m_newYCoords.push_back(sum);
	}

	for (size_t t = 0; t < m_newYCoords.size(); ++t)
	{
		m_vertices[3 * t + 1] = m_newYCoords[t];
	}

	UpdateMinMaxHeight();
	GenerateNormals();
}

void Terrain::GenerateNormals()
{
	if (m_normals.size() != m_vertices.size())
	{
		m_normals.clear();
		m_normals.resize(m_vertices.size());
	}

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

void Terrain::UpdateMinMaxHeight()
{
	if (m_vertices.size() < 2) return;
	m_maxHeight = m_minHeight = m_vertices[1];
	for (size_t i = 4; i < m_vertices.size(); i += 3)
	{
		if (m_vertices[i] > m_maxHeight) m_maxHeight = m_vertices[i];
		if (m_vertices[i] < m_minHeight) m_minHeight = m_vertices[i];
	}
}

void Terrain::ApplyPerlinNoise(int octaves, double persistence, double coordsMultFactor, double noiseMultFactor, double z)
{
	ClearVertexData();
	CreateTerrain();

	float sliceW = (float) m_width / (float)(m_rowNum - 1);
	float sliceH = (float) m_height / (float)(m_colNum - 1);

	size_t nr_vert = m_vertices.size();
	//double 
	//z = RNG::getFloat();
	int index = 0;

	for (int i = 0; i < nr_vert; i += 3)
	{
		int ii = index / m_colNum;
		int jj = index % m_colNum;
		double x = (double)(coordsMultFactor * ii * sliceW) / (m_width);
		double y = (double)(coordsMultFactor * jj * sliceH) / (m_height);
		double pn = PerlinNoise::OctavePerlin(x, y, z, octaves, persistence);
		double n = noiseMultFactor * pn;

		m_vertices[i + 1] = n;

		++index;
	}

	// (int i = 0; i <= 2; ++i)
	//	SmoothTerrain(3);

	UpdateMinMaxHeight();
	GenerateNormals();
}

void Terrain::ExportAsImage(string imgFileName)
{
	sf::Image image;
	image.create(m_rowNum, m_colNum);
	
	int index = 1; 
	double val, diff = (m_maxHeight - m_minHeight);
	double alpha;
	
	for (int i = 0; i < m_rowNum; ++i)
	for (int j = 0; j < m_colNum; ++j)
	{
		sf::Color color;
		alpha = ((m_vertices[index] - m_minHeight) / diff);
		color.r = (int)(alpha * 255);
		color.g = (alpha > 0.5f) ? (int) ((2 - 2 * alpha) * 255) : (int) (255 * (2 * alpha));
		color.b = (int)((1 - alpha) * 255);
		image.setPixel(i, j, color);
		index += 3;
	}
	
	// Save the image to a file
	if (!image.saveToFile(imgFileName + ".png"))
	{
		cout << "Error saving " + imgFileName + ".png\n";
	}
}


/*
float alpha = (fragVert.y - minHeight) / (maxHeight - minHeight);
float red = alpha;
float green = 2 * alpha; // 2 * (1 - alpha)
if(alpha > 0.5f) green = 2 - green;
float blue = (1 - alpha);
out_Color = vec4(red, green, blue, 1.0);
*/