#include "Light.h"

Light::Light(glm::vec3 pos, glm::vec3 color)
{
	m_pos = pos;
	m_color = color;
}

Light::~Light()
{
}

glm::vec3 Light::GetPos() const
{
	return m_pos;
}

glm::vec3& Light::GetPosRef() 
{
	return m_pos;
}

float Light::GetXPosCoord() const
{
	return m_pos.x;
}

float Light::GetYPosCoord() const
{
	return m_pos.y;
}

float Light::GetZPosCoord() const
{
	return m_pos.z;
}

void Light::SetPos(glm::vec3 pos)
{
	m_pos = pos;
}

void Light::SetXPosCoord(float x)
{
	m_pos.x = x;
}

void Light::SetYPosCoord(float y)
{
	m_pos.y = y;
}

void Light::SetZPosCoord(float z)
{
	m_pos.z = z;
}

// Color
glm::vec3 Light::GetColor() const
{
	return m_color;
}

glm::vec3& Light::GetColorRef() 
{
	return m_color;
}

float Light::GetRedChannel() const
{
	return m_color.r;
}

float Light::GetGreenChannel() const
{
	return m_color.g;
}

float Light::GetBlueChannel() const
{
	return m_color.b;
}

void Light::SetColor(glm::vec3 color)
{
	m_color = color;
}

void Light::SetRedChannel(float red)
{
	m_color.r = red;
}

void Light::SetGreenChannel(float green)
{
	m_color.g = green;
}

void Light::SetBlueChannel(float blue)
{
	m_color.b = blue;
}