#pragma once
#include "Model.h"

class Cube : public Model
{
public:
	Cube(glm::vec3 centerPos = glm::vec3(0.0f, 0.0f, 0.0f), float width = 1.0f, float height = 1.0f);
	~Cube();

	void Draw() override;
	void Update() override;

private:
	
	float m_width;
	float m_height;
};

