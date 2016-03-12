#pragma once
#include <vector>
#include <GL/glew.h> 
#include <GL/freeglut.h>
#include <glm\vec3.hpp>
#include "glm/glm.hpp"  
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class Model
{
public:
	Model();
	~Model();

	virtual void Draw() = 0;
	virtual void Update() = 0;

	void CleanUp();
	void SetVertexShader(GLuint _VertexShaderId);
	void SetFragmentShader(GLuint _FragmentShaderId);

	// Rotation
	void SetRotation(glm::vec3 angles);
	void SetRotationX(float RotationXAngle);
	void SetRotationY(float RotationYAngle);
	void SetRotationZ(float RotationZAngle);

	glm::vec3 GetRotation() const;
	float GetRotationX() const;
	float GetRotationY() const;
	float GetRotationZ() const;

	// Translation
	void SetTranslation(glm::vec3 translationVector);
	void SetTranslationX(float TranslationX);
	void SetTranslationY(float TranslationY);
	void SetTranslationZ(float TranslationZ);

	glm::vec3 GetTranslation() const;
	float GetTranslationX() const;
	float GetTranslationY() const;
	float GetTranslationZ() const;

	void SetTexturing(int value);

protected:
	glm::vec3 m_pos;
	glm::vec3 m_centerPos;

	glm::mat4 m_transform;
	glm::vec3 m_rotationAngles;
	glm::vec3 m_translationVector;

	std::vector<GLfloat> m_vertices;
	std::vector<GLfloat> m_normals;
	std::vector<GLfloat> m_texcoords;
	std::vector<GLfloat> m_colors;
	std::vector<GLuint>  m_indices;

	int m_texturingEnabled;

	GLuint
		modelMatrixLocation,
		texturingFlagLocation,
		VerticesBufferId,
		TextureCoordsBufferId,
		IndicesBufferId,
		ColorsBufferId,
		VertexShaderId,
		NormalsBufferId,
		FragmentShaderId,
		ProgramId;

	void CreateVBO();
	void DestroyVBO();

	void CreateBuffers();
	void ApplyTransform();

	virtual void GenerateVertices() = 0;
	//void DisplayVertices();
	virtual void GenerateIndexes() = 0;
	//void DisplayIndexes();
	virtual void GenerateColors() = 0;
	virtual void GenerateNormals() = 0;
};

