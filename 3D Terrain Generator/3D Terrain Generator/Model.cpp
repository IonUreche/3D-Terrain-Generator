#include "Model.h"


Model::Model()
{
	m_pos = glm::vec3(0.0f);
	m_transform = glm::mat4(1.0f);
	m_rotationAngles = glm::vec3(0.0f);
	m_translationVector = glm::vec3(0.0f);
}

Model::~Model()
{
}

void Model::ApplyTransform()
{
	m_transform = glm::rotate(glm::mat4(1.0f), m_rotationAngles.x, glm::vec3(1.0f, 0.0f, 0.0f));
	m_transform = glm::rotate(m_transform, m_rotationAngles.y, glm::vec3(0.0f, 1.0f, 0.0f));
	m_transform = glm::rotate(m_transform, m_rotationAngles.z, glm::vec3(0.0f, 0.0f, 1.0f));

	m_transform = glm::translate(glm::mat4(1.0f), -m_translationVector) * glm::translate(glm::mat4(1.0f), m_centerPos)
		* m_transform
		* glm::translate(glm::mat4(1.0f), -m_centerPos);

	modelMatrixLocation = glGetUniformLocation(VertexShaderId, "model");
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &m_transform[0][0]);
}

void Model::CreateVBO()
{
	glBindBuffer(GL_ARRAY_BUFFER, VerticesBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* m_vertices.size(), &m_vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, ColorsBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)* m_colors.size(), &m_colors[0], GL_STATIC_DRAW);
	
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(GLuint), &m_indices[0], GL_STATIC_DRAW);
	//glEnableVertexAttribArray(GL_PRIMITIVE_RESTART);
	//glPrimitiveRestartIndex(m_colNum * m_rowNum);
}

void Model::CreateBuffers()
{
	glGenBuffers(1, &VerticesBufferId);
	glGenBuffers(1, &ColorsBufferId);
	glGenBuffers(1, &IndicesBufferId);
}

void Model::DestroyVBO()
{
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ColorsBufferId);
	glDeleteBuffers(1, &VerticesBufferId);
	glDeleteBuffers(1, &IndicesBufferId);
}

void Model::CleanUp()
{
	DestroyVBO();
}

void Model::SetVertexShader(GLuint _VertexShaderId)
{
	VertexShaderId = _VertexShaderId;
}

void Model::SetFragmentShader(GLuint _FragmentShaderId)
{
	FragmentShaderId = _FragmentShaderId;
}

// Rotation
void Model::SetRotation(glm::vec3 angles)
{
	m_rotationAngles = angles;
}

void Model::SetRotationX(float RotationXAngle)
{
	m_rotationAngles.x = RotationXAngle;
}

void Model::SetRotationY(float RotationYAngle)
{
	m_rotationAngles.y = RotationYAngle;
}

void Model::SetRotationZ(float RotationZAngle)
{
	m_rotationAngles.z = RotationZAngle;
}

glm::vec3 Model::GetRotation() const
{
	return m_rotationAngles;
}

float Model::GetRotationX() const
{
	return m_rotationAngles.x;
}

float Model::GetRotationY() const
{
	return m_rotationAngles.y;
}

float Model::GetRotationZ() const
{
	return m_rotationAngles.z;
}

// Translation
void Model::SetTranslation(glm::vec3 translationVector)
{
	m_translationVector = translationVector;
}

void Model::SetTranslationX(float TranslationX)
{
	m_translationVector.x = TranslationX;
}

void Model::SetTranslationY(float TranslationY)
{
	m_translationVector.y = TranslationY;
}

void Model::SetTranslationZ(float TranslationZ)
{
	m_translationVector.z = TranslationZ;
}

glm::vec3 Model::GetTranslation() const
{
	return m_translationVector;
}

float Model::GetTranslationX() const
{
	return m_translationVector.x;
}

float Model::GetTranslationY() const
{
	return m_translationVector.y;
}

float Model::GetTranslationZ() const
{
	return m_translationVector.z;
}
