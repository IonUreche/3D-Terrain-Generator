#include "stdafx.h"

#include "Helper.h"
#include "Skybox.h"


Skybox::Skybox()
{
}


Skybox::~Skybox()
{
	DestroyBuffers();
}

bool Skybox::Init(int cubeSize)
{
	m_cubeSize = cubeSize;

	// create shaders
	if (g_helper->CreateShaders("data\\shaders\\Skybox_vertex_shader.vert", "data\\shaders\\Skybox_fragment_shader.frag",
		m_ShaderProgramId, m_VertexShaderId, m_FragmentShaderId) == false)
	{
		cout << "Shader could not be created\n";
		return false;
	}

	GenerateBuffers();

	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	
	m_faces.push_back("data\\textures\\skybox\\right.jpg");
	m_faces.push_back("data\\textures\\skybox\\left.jpg");
	m_faces.push_back("data\\textures\\skybox\\top.jpg");
	m_faces.push_back("data\\textures\\skybox\\bottom.jpg");
	m_faces.push_back("data\\textures\\skybox\\back.jpg");
	m_faces.push_back("data\\textures\\skybox\\front.jpg");
	m_cubemapTexture = loadCubemap(m_faces);

	//CreateVBO();

	return true;
}

GLuint Skybox::loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glActiveTexture(GL_TEXTURE0);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for (GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
			GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image
			);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	CreateVBO();

	return textureID;
}

void Skybox::CreateVBO()
{
	GLfloat skyboxVertices[] = {
		// Positions          
		-1.0f, 1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f,
		-1.0f, -1.0f, 1.0f,

		-1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, 1.0f,
		1.0f, -1.0f, 1.0f
	};

	for (int i = 0; i < 36 * 3; ++i)
		m_skyboxVertices[i] = skyboxVertices[i] * m_cubeSize;
	// Setup skybox VAO
	
	glBindVertexArray(m_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), &m_skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
}

void Skybox::GenerateBuffers()
{
	glGenVertexArrays(1, &m_skyboxVAO);
	glGenBuffers(1, &m_skyboxVBO);
}

void Skybox::DestroyBuffers()
{
	glDisableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &m_skyboxVAO);
	glDeleteBuffers(1, &m_skyboxVBO);
	m_skyboxVAO = m_skyboxVBO = 0;
}

void Skybox::Draw(glm::mat4 c_view, glm::mat4 c_projection)
{
	//glDepthMask(GL_FALSE);
	GLint OldCullFaceMode;
	glGetIntegerv(GL_CULL_FACE_MODE, &OldCullFaceMode);
	GLint OldDepthFuncMode;
	glGetIntegerv(GL_DEPTH_FUNC, &OldDepthFuncMode);

	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);
	
	glUseProgram(m_ShaderProgramId);

	glm::mat4 view = glm::mat4(glm::mat3(c_view));	// Remove any translation component of the view matrix
	glm::mat4 projection = c_projection;// glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramId, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	
	glBindVertexArray(m_skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
	glUniform1i(glGetUniformLocation(m_ShaderProgramId, "skybox"), 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	//glDepthMask(GL_TRUE);
	glCullFace(OldCullFaceMode);
	glDepthFunc(OldDepthFuncMode);
}