#include "Cube.h"


Cube::Cube(glm::vec3 centerPos, float width, float height)
{
	m_pos = m_centerPos = centerPos;
	m_width = width;
	m_height = height;
}

void Cube::Draw()
{
	CreateVBO();
	ApplyTransform();
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Cube::Update()
{

}

Cube::~Cube()
{
}
