#include "Sphere.h"
#include <cmath> 
#define M_PI       3.14159265358979323846
#define M_PI_2     1.57079632679489661923

Sphere::Sphere(glm::vec3 pos, float radius, unsigned int rings, unsigned int sectors)
{
	m_pos = m_centerPos = pos;
	m_radius = radius;
	m_rings = rings;
	m_sectors = sectors;

	CreateBuffers();
	GenerateVertices();
	GenerateIndexes();
}

void Sphere::Draw() 
{
	m_rotationAngles.x += 0.01f;
	m_rotationAngles.y += 0.015f;
	m_rotationAngles.z += 0.013f;
	m_translationVector.x += 0.005f;
	CreateVBO();
	ApplyTransform();
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
}

Sphere::~Sphere()
{
}


void Sphere::Update() 
{

}

void Sphere::GenerateVertices() 
{
	float const R = 1. / (float)(m_rings - 1);
	float const S = 1. / (float)(m_sectors - 1);
	int r, s;

	m_vertices.resize(m_rings * m_sectors * 3);
	m_colors.resize(m_rings * m_sectors * 3);
	m_normals.resize(m_rings * m_sectors * 3);
	m_texcoords.resize(m_rings * m_sectors * 2);
	std::vector<GLfloat>::iterator v = m_vertices.begin();
	std::vector<GLfloat>::iterator n = m_normals.begin();
	std::vector<GLfloat>::iterator t = m_texcoords.begin();
	std::vector<GLfloat>::iterator c = m_colors.begin();
	for (r = 0; r < m_rings; r++)
	for (s = 0; s < m_sectors; s++) {
		//float y = sin(-M_PI_2 + M_PI * r * R);
		//float x = cos(2 * M_PI * s * S) * sin(M_PI * r * R);
		//float z = sin(2 * M_PI * s * S) * sin(M_PI * r * R);
		float phi = 2 * M_PI * s * S;
		float theta = M_PI * r * R - M_PI_2;

		float x = cos(theta) * cos(phi);
		float y = cos(theta) * sin(phi);
		float z = sin(theta);

		*t++ = s*S;
		*t++ = r*R;

		*v++ = x * m_radius + m_pos.x;
		*v++ = y * m_radius + m_pos.y;
		*v++ = z * m_radius + m_pos.z;

		*n++ = x + m_pos.x;
		*n++ = y + m_pos.y;
		*n++ = z + m_pos.z;

		float yy = y * m_radius + m_pos.y;
		float ymin = m_pos.y - m_radius;
		float Red = (yy - ymin) / (2.0 * m_radius);

		*c++ = Red; // 1.0f - (m_pos;
		*c++ = ((float) s / (float) (m_sectors - 1)) * (1.0f - fabs(y - m_pos.y) / m_radius);
		*c++ = 1.0f - Red;
	}
}

void Sphere::GenerateIndexes() 
{
	m_indices.clear();
	for (int r = 0; r < m_rings; r++)
	{
		for (int s = 0; s < m_sectors; s++) {
			m_indices.push_back(r * m_sectors + s);
			m_indices.push_back((r + 1) * m_sectors + s);
		}
		m_indices.push_back((r + 1) * m_sectors + m_sectors - 1);
		if (r != m_rings - 2)
			m_indices.push_back((r + 1) * m_sectors);
	}
}

void Sphere::GenerateColors() 
{
	
}
