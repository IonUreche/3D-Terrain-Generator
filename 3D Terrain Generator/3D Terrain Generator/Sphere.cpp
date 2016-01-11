#include "Sphere.h"
#include <cmath> 
#include "RNG.h"
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
	GenerateNormals();
}

Sphere::~Sphere()
{
}

void Sphere::Draw() 
{
	CreateVBO();
	ApplyTransform();
	glDrawElements(GL_TRIANGLE_STRIP, m_indices.size(), GL_UNSIGNED_INT, 0);
}

void Sphere::Update() 
{
	m_rotationAngles.x += 0.01f;
	m_rotationAngles.y += 0.015f;
	m_rotationAngles.z += 0.013f;
	//if (m_translationVector.x < 2.0f) m_translationVector += 0.005f;
	//if (m_translationVector.x > 2.0f) m_translationVector -= 0.004f;
	//if (m_translationVector.y < 1.5f) m_translationVector.y += 0.003;
	//if (m_translationVector.y > 1.5f) m_translationVector.y -= 0.006;
	//if (m_translationVector.z < 3.0f) m_translationVector.z += 0.003;
	//if (m_translationVector.z > 3.0f) m_translationVector.z -= 0.006;
	//m_translationVector.x = cos(m_rotationAngles.x) * cos(m_rotationAngles.z);
	//m_translationVector.y = sin(m_rotationAngles.y) * cos(m_rotationAngles.z);
	//m_translationVector.z = sin(m_rotationAngles.z) * cos(m_rotationAngles.z);

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
	for (int i = 0; i < m_rings * m_sectors; ++i)
	{
		for (int t = 0; t < 3; ++t)
			m_colors.push_back(RNG::GetRandFloat());
	}
}

void Sphere::GenerateNormals()
{
	m_normals.clear();
	m_normals.resize(m_vertices.size());

	for (int i = 0; i < m_rings; ++i)
	{
		for (int j = 0; j < m_sectors; ++j)
		{
			int vertIndex = i * m_sectors + j;
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

				if (IsValidSphereCoord(I1, J1) && IsValidSphereCoord(I2, J2))
				{
					glm::vec3 v1 = GetGridPointCoordVect(I1, J1) - currVect;
					glm::vec3 v2 = GetGridPointCoordVect(I2, J2) - currVect;
					glm::vec3 cross_prod = glm::cross(v2, v1);
					ans += glm::normalize(cross_prod);
					++nr;
				}
			}

			ans = 1.0f * (ans / (float)nr);
			m_normals[3 * vertIndex] = ans.x;
			m_normals[3 * vertIndex + 1] = ans.y;
			m_normals[3 * vertIndex + 2] = ans.z;
		}
	}
}

bool Sphere::IsValidSphereCoord(int ring, int sector)
{
	int index = 3 * (ring * m_sectors + sector);
	if (index < 0 || index >= m_vertices.size()) return false;
	return true;
}

glm::vec3 Sphere::GetGridPointCoordVect(int ring, int sector)
{
	if (!IsValidSphereCoord(ring, sector)) return glm::vec3(-1.0f);
	return glm::vec3(m_vertices[3 * (ring * m_sectors + sector)], m_vertices[3 * (ring * m_sectors + sector) + 1], m_vertices[3 * (ring * m_sectors + sector) + 2]);
}