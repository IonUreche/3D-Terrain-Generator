#pragma once

class Light
{
public:

	Light(glm::vec3 pos = glm::vec3(0.0f), glm::vec3 color = glm::vec3(1.0f));
	~Light();

	glm::vec3 GetPos() const;
	glm::vec3& GetPosRef();
	float GetXPosCoord() const;
	float GetYPosCoord() const;
	float GetZPosCoord() const;

	void SetPos(glm::vec3 pos);
	void SetXPosCoord(float x);
	void SetYPosCoord(float y);
	void SetZPosCoord(float z);

	// Color
	glm::vec3 GetColor() const;
	glm::vec3& GetColorRef();
	float GetRedChannel() const;
	float GetGreenChannel() const;
	float GetBlueChannel() const;

	void SetColor(glm::vec3 color);
	void SetRedChannel(float red);
	void SetGreenChannel(float green);
	void SetBlueChannel(float blue);

private:

	glm::vec3 m_pos;
	glm::vec3 m_color;
	
};

