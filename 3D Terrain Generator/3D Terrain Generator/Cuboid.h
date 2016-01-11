#pragma once
#include "Model.h"

class Cuboid : public Model
{
public:
	Cuboid(glm::vec3 centerPos = glm::vec3(0.0f, 0.0f, 0.0f), float lenght = 1.0f, float height = 1.0f, float width = 1.0f);
	~Cuboid();

	void Draw() override;
	void Update() override;

private:
	
	float m_length;
	float m_width;
	float m_height;

	void GenerateVertices() override;
	void GenerateIndexes() override;
	void GenerateColors() override;
	void GenerateNormals() override;
};

