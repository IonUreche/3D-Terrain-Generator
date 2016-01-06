#include "Terrain.h"
#include <iostream>
#include <cmath>

Terrain::Terrain(int width = 10, int height = 10, int rowLen = 10, int colLen = 10)
{
	// Just to make sure we have even number of vertices in each row and column
	if (m_rowLen & 1) ++m_rowLen;
	if (m_colLen & 1) ++m_colLen;
	m_width = width;
	m_height = height;
	m_rowLen = rowLen;
	m_colLen = colLen;

	m_nrIndicesToDraw = 3;

	GenerateVertices();
	//DisplayVertices();
	GenerateColors();
	GenerateIndexes();
	//DisplayIndexes();
	CreateBuffers();
	CreateVBO();
}

Terrain::~Terrain()
{
}

void Terrain::GenerateVertices()
{
	float sliceW = m_width / (m_rowLen - 1);
	float sliceH = m_height / (m_colLen - 1);

	m_vertices.clear();

	for (int i = 0; i < m_rowLen; ++i)
		for (int j = 0; j < m_colLen; ++j)
		{
			m_vertices.push_back(j * sliceW);
			m_vertices.push_back(0.0f);
			m_vertices.push_back(i * sliceH);
		}

		int k = 0;
		p_points = new float[m_vertices.size()];
		for (auto &x : m_vertices)
			p_points[k++] = x;
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
	for (int i = 0; i < m_rowLen * m_colLen; ++i)
	{
		for(int t = 0; t < 3; ++ t) 
			m_colors.push_back(0.0f);
	}
}

void Terrain::GenerateIndexes()
{
	for (int i = 0; i < m_rowLen - 1; ++i)
	{
		for (int j = 0; j < m_colLen; ++j)
		{
			m_indices.push_back(m_colLen * i + j);
			m_indices.push_back((i + 1) * m_colLen + j);
		}
		//m_indices.push_back(m_rowLen * m_colLen);
		//m_indices.push_back(m_rowLen * m_colLen);
		m_indices.push_back((i + 1) * m_colLen + m_colLen - 1);
		if (i != m_rowLen - 2)
			m_indices.push_back((i + 1) * m_colLen);
	}
}

void Terrain::DisplayIndexes()
{
	int ind = 0;
	for (auto &x : m_indices)
	{
		cout << x << ' ';
		ind = (ind + 1) % m_colLen;
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
	//glPrimitiveRestartIndex(m_colLen * m_rowLen);
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
	for (int i = 0; i < m_rowLen; ++i)
	{
		for (int j = 0; j < m_colLen; ++j)
		{
			float u = (float)i / (float)(m_rowLen - 1);
			float v = (float)j / (float)(m_colLen - 1);

			int v_ind = 3 * (i * m_colLen + j);
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