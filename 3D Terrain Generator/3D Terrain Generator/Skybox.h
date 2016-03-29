#pragma once
class Skybox
{
public:
	Skybox();
	~Skybox();

	bool Init(int cubeSize);
	GLuint loadCubemap(std::vector<const GLchar*> faces);
	void Draw(glm::mat4 c_view, glm::mat4 c_projection);

private:

	void CreateVBO();
	void GenerateBuffers();
	void DestroyBuffers();

	GLfloat m_skyboxVertices[108];
	int m_cubeSize;
	GLuint m_textureID;
	GLuint m_cubemapTexture;
	GLuint m_ShaderProgramId;
	GLuint m_VertexShaderId;
	GLuint m_FragmentShaderId;
	GLuint m_skyboxVAO;
	GLuint m_skyboxVBO;
	std::vector<const GLchar*> m_faces;
};

