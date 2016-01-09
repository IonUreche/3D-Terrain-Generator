#pragma once

#include "Model.h"

using namespace std;

class Sphere : public Model
{
protected:
	int m_rings;
	int m_sectors;
	float m_radius;
	
public:
	Sphere(glm::vec3 pos, float radius, unsigned int rings, unsigned int sectors);
	~Sphere();

	void Draw() override;
	void Update() override;

private:

	void GenerateVertices() override;
	void GenerateIndexes() override;
	void GenerateColors() override;
};

