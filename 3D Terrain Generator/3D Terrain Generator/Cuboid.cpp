#include "stdafx.h"
#include "RNG.h"

#include "Cuboid.h"

Cuboid::Cuboid(glm::vec3 centerPos, float length, float height, float width)
{
	m_pos = m_centerPos = centerPos;
	m_length = length;
	m_height = height;
	m_width = width;

	CreateBuffers();
	GenerateVertices();
	GenerateColors();
	GenerateIndexes();
	GenerateNormals();
}

void Cuboid::Draw()
{
	CreateVBO();
	ApplyTransform();
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Cuboid::Update()
{
	//SetRotationX(30.0f);
	//SetRotationY(GetRotationY() + 0.002f);
	SetRotationY(GetRotationY() + 0.003f);
}

Cuboid::~Cuboid()
{
}

void Cuboid::GenerateVertices()
{
	m_vertices.clear();

	float halfLenght = m_length / 2.0f;
	float halfWidth = m_width / 2.0f;
	float halfHeight = m_height / 2.0f;

	for (int l = -1; l <= 1; l += 2)
		for (int h = -1; h <= 1; h += 2)
			for (int w = -1; w <= 1; w += 2)
			{
				m_vertices.push_back(m_pos.x + l * halfLenght);
				m_vertices.push_back(m_pos.y + h * halfHeight);
				m_vertices.push_back(m_pos.z + w * halfWidth);

				m_texcoords.push_back(0.0f);
				m_texcoords.push_back(1.0f);
			}
	
}

void Cuboid::GenerateIndexes()
{
	int ind[14] = { 0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1 };
	
	for (int i = 0; i < 14; ++i)
		m_indices.push_back(ind[i]);
}

void Cuboid::GenerateColors()
{
	for (size_t i = 0; i < m_vertices.size(); i += 3)
	{
		m_colors.push_back(RNG::getFloat()); 
		m_colors.push_back(RNG::getFloat());
		m_colors.push_back(RNG::getFloat());
	}
		
}

void Cuboid::GenerateNormals()
{
	// place-holder implementation
	m_normals.clear();
	m_normals.resize(m_vertices.size());
	for (size_t i = 0; i < m_vertices.size(); i++)
	{
		m_normals[i] = 1.0f;
	}

	// TO DO: Compute cuboid's faced normals
}