
#include "stdafx.h"
#include "Helper.h"
#include "Skybox.h"

#include "Camera.h"
#include "Cuboid.h"
#include "Sphere.h"
#include "Terrain.h"
#include "Light.h"

#include "Texture.h"

#include <glui.h>
#include <sstream>

using namespace std;

int main_window;
int obj = 0;

GLUI *glui;
GLUI_EditText   *edittext;
GLUI_RadioGroup *radio;

GLUI_String s_terrainWidth, s_terrainHeight, s_octaves, s_persistence, s_coordMultFactor, s_noiseMultFactor;
GLUI_String s_heightScaleValue, s_roughness, s_power2Size, s_terrainSize, s_numberOfSmoothingIterations;
GLUI_String s_perlinAnimationSpeed;
int g_AnimationIsRunning;

bool ApplyPerlinNoiseNextFrame = false;
int g_octaves;
double g_persistence, g_coordsMultFactor, g_noiseMultFactor, g_terrainWidth, g_terrainHeight, g_perlinAnimationSpeed;

int g_power2Size, g_numberOfSmoothingIterations = 0;
double g_roughness, g_heightScaleValue, g_terrainSize;
bool ApplyDiamondSquareNextFrame = false;

bool StartAnimatingPerlin = false;
double PerlinTime = 0.0;

GLuint
ProgramId, VertexShaderId, FragmentShaderId,
modelMatrixLocation, viewLocation, projLocation,
matrRotlLocation, grassSamplerLoc, rockSamplerLoc, snowSamplerLoc;

Camera camera;
extern Camera *g_camera = &camera;

Sphere *mySphere;
Skybox skybox;

// matrice utilizate
glm::mat4 model, matrRot;

glm::vec3 Obs = glm::vec3(-10, 50.0f, -10.0f);
glm::vec3 RefPct = glm::vec3(20.0f, 0.0f, 20.0f);

int isWireFrame = 0;

Light *light;
Terrain *ter;
Cuboid *cuboid;

// Mouse
int lastMouseX = -1, lastMouseY = -1;
float AngleXoZ = 303.5f, AngleYoZ = 0;
float scrollSpeed = 0.01f, cameraSphereRadius = 14.0f;

//Camera
float CenterX = 0.0f, CenterY = 0.0f, CenterZ = 0.0f;

// elemente pentru matricea de proiectie
float width = 1080, height = 720, znear = 0.1f, zfar = 20000.0f, fov = 45;
glm::mat4 projection;

// Light
vector<GLfloat> lightPosVec, lightColorVec;
GLuint lightPosLoc, lightColLoc, viewPosLoc;

// Camera Rotation angles
glm::vec3 cameraRotation = glm::vec3(0.0f);

CTexture tGrass, tRock, tSnow;

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		camera.Move(FORWARD);
		break;
	case 'a':
		camera.Move(LEFT);
		break;
	case 's':
		camera.Move(BACK);
		break;
	case 'd':
		camera.Move(RIGHT);
		break;
	case 'q':
		camera.Move(DOWN);
		break;
	case 'e':
		camera.Move(UP);
		break;
	case 'r':
		break;
	case '+':
		break;
	case '-':
		cameraSphereRadius += 1.0f;
		break;
	case 'v':
		isWireFrame ^= 1;
		break;
	case 'p':
		ter->SetTexturing(ter->GetTexturing() ^ 1);
		break;
	case 'u':
		lightPosVec[0] += 0.1f;
		break;
	case 'i':
		lightPosVec[1] += 0.1f;
		break;
	case 'o':
		lightPosVec[2] += 0.1f;
		break;
	case 'j':
		lightPosVec[0] -= 0.1f;
		break;
	case 'k':
		lightPosVec[1] -= 0.1f;
		break;
	case 'l':
		lightPosVec[2] -= 0.1f;
		break;
	case '1': 
		break;
	case '2':
		break;
	case '3':
		break;
	case '4':
		break;
	case '5':
		ter->SmoothTerrain(3);
		break;
	case '6':
		ter->Apply3x3Filter();
		break;
	}

	if (key == 27)
		exit(0);
}

void processSpecialKeys(int key, int xx, int yy) {

	switch (key) {
	case GLUT_KEY_LEFT:
		
		break;
	case GLUT_KEY_RIGHT:
		
		break;
	case GLUT_KEY_UP:
		
		break;
	case GLUT_KEY_DOWN:
		
		break;
	}
}

bool Initialize()
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	model = glm::mat4(1.0f);

	if (g_helper->CreateShaders("data\\shaders\\Terrain_vertex_shader.vert", "data\\shaders\\Terrain_fragment_shader.frag", 
		ProgramId, VertexShaderId, FragmentShaderId) == false)
	{
		cout << "Shader could not be created";
		return false;
	}
	
	skybox.Init(10000);

	glUseProgram(ProgramId);

	camera.SetProgramId(ProgramId);
	camera.SetPosition(Obs);
	camera.SetLookAt(RefPct);

	camera.SetFOV(fov);
	camera.aspect = GLfloat(width) / GLfloat(height);
	camera.near_clip = znear;
	camera.far_clip = zfar;
	camera.move_camera = true;

	ter = new Terrain(100, 100, 100, 100, glm::vec3(0.0, 0.0, 0.0));
	ter->SetShaderProgram(ProgramId);
	ter->GenerateDiamondSquareSurface2(10, 7, 0.7f, 3.0f);
	//ter->GenerateDiamondSquareSurface(100, 5, 10, -5, 5, 0.8);
	
	//ter->GetGridPointCoordVect(5, 5);
	//ter->ApplyPerlinNoise();
	light = new Light(glm::vec3(5.0f, 0.0f, 5.0f), glm::vec3(0.5f, 0.5f, 0.5f));

	// Set uniform fragment shader
	lightPosVec.push_back(light->GetPos().x);
	lightPosVec.push_back(light->GetPos().y);
	lightPosVec.push_back(light->GetPos().z);

	lightColorVec.push_back(light->GetColor().x);
	lightColorVec.push_back(light->GetColor().y);
	lightColorVec.push_back(light->GetColor().z);

	lightPosLoc = glGetUniformLocation(ProgramId, "lightPosition");
	glUniform3fv(lightPosLoc, 3, &lightPosVec[0]);

	lightColLoc = glGetUniformLocation(ProgramId, "lightIntensities");
	glUniform3fv(lightColLoc, 3, &lightColorVec[0]);

	viewPosLoc = glGetUniformLocation(ProgramId, "viewPos");
	glUniform3f(viewPosLoc, Obs.x, Obs.y, Obs.z);
	//mySphere = new Sphere(glm::vec3(1.0f, 1.0f, 1.0f), 1, 7, 14);

	cuboid = new Cuboid(glm::vec3(3.0f, 1.0f, 2.0f), 1.0f, 1.0f, 1.0f);
	cuboid->SetShaderProgram(ProgramId);
	/*
	vector<glm::vec3> bez;
	for (int i = 0; i < 4; ++i)
	{
		bez.push_back(glm::vec3(0.0f, 0.0f, i * 3.33f));
		bez.push_back(glm::vec3(3.33f, 5.0f, i * 3.33f));
		bez.push_back(glm::vec3(6.66f, -5.0f, i * 3.33f));
		bez.push_back(glm::vec3(10.0f, 0.0f, i * 3.33f));
	}
	
	ter->SetBezierControlPoints(bez);
	ter->GenerateBezierSurface();
	*/
	mySphere = new Sphere(glm::vec3(-2.0f, 0.0f, -2.0f), 1.5f, 30, 30);
	mySphere->SetShaderProgram(ProgramId);

	grassSamplerLoc = glGetUniformLocation(ProgramId, "tGrass");
	rockSamplerLoc = glGetUniformLocation(ProgramId, "tRock");
	snowSamplerLoc = glGetUniformLocation(ProgramId, "tSnow");
	glUniform1i(grassSamplerLoc, 1);
	glUniform1i(rockSamplerLoc, 2);
	glUniform1i(snowSamplerLoc, 3);

	tGrass.loadTexture2D("data\\textures\\moon-texture.jpg", true);
	tGrass.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	tRock.loadTexture2D("data\\textures\\Asteroid-texture.png", true);
	tRock.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	tSnow.loadTexture2D("data\\textures\\snow.jpg", true);
	tSnow.setFiltering(TEXTURE_FILTER_MAG_BILINEAR, TEXTURE_FILTER_MIN_BILINEAR_MIPMAP);

	tGrass.bindTexture(1);
	tRock.bindTexture(2);
	tSnow.bindTexture(3);

	return true;
}

void RenderFunction(void)
{
	glutSetWindow(main_window);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDisable(GL_CULL_FACE);

	// skybox.Draw(camera.view, camera.projection);

	if (ApplyPerlinNoiseNextFrame || (StartAnimatingPerlin && g_AnimationIsRunning))
	{
		ApplyPerlinNoiseNextFrame = false;
		if (!StartAnimatingPerlin)
		{
			StartAnimatingPerlin = true;
			PerlinTime = 0;
			ter->SetSize(g_terrainWidth, g_terrainHeight);
			ter->SetGridSize(g_terrainWidth, g_terrainHeight);
		}
		
		PerlinTime += g_perlinAnimationSpeed;
		ter->ApplyPerlinNoise(g_octaves, g_persistence, g_coordsMultFactor, g_noiseMultFactor, PerlinTime);
	}
	if (ApplyDiamondSquareNextFrame)
	{
		ter->GenerateDiamondSquareSurface2(g_terrainSize, g_power2Size, g_roughness, g_heightScaleValue, g_numberOfSmoothingIterations);
		ApplyDiamondSquareNextFrame = false;
	}
	
	if (isWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glEnable(GL_CULL_FACE);
	}
	
	GLuint texturingFlagLocation = glGetUniformLocation(VertexShaderId, "texturing_enabled");
	glUniform1i(texturingFlagLocation, 0);

	//lightPosVec[1] += 0.001f;
	//lightPosVec[2] += 0.001f;
	//cout << lightPosVec[0] << ' ' << lightPosVec[1] << ' ' << lightPosVec[2] << '\n';

	lightPosLoc = glGetUniformLocation(ProgramId, "lightPosition");
	glUniform3fv(lightPosLoc, 1, &lightPosVec[0]);

	lightColLoc = glGetUniformLocation(ProgramId, "lightIntensities");
	glUniform3fv(lightColLoc, 1, &lightColorVec[0]);

	viewPosLoc = glGetUniformLocation(ProgramId, "viewPos");
	glUniform3f(viewPosLoc, Obs.x, Obs.y, Obs.z);

	tGrass.bindTexture(1);
	tRock.bindTexture(2);
	tSnow.bindTexture(3);
	
	//mySphere->Draw();
	ter->Draw(camera.view, camera.projection);

	camera.Update();

	glutSwapBuffers();
}

void Cleanup()
{
	ter->CleanUp();
}

void MouseMotionFunction(int x, int y)
{
	camera.Move2D(x, y);
}

void MousePassiveMotionFunction(int x, int y)
{
	camera.mouse_position = glm::vec3(x, y, 0);
}

void GenerateButtonCallbackFunctionF1()
{
	stringstream ss;

	ss << s_terrainWidth << ' ' << s_terrainHeight << ' ' << s_octaves << ' ' << s_persistence << ' ' << s_coordMultFactor << ' ' << s_noiseMultFactor << ' ' << s_perlinAnimationSpeed;
	ss.seekg(0, ios_base::beg);
	ss >> g_terrainWidth >> g_terrainHeight >> g_octaves >> g_persistence >> g_coordsMultFactor >> g_noiseMultFactor >> g_perlinAnimationSpeed;
	ApplyPerlinNoiseNextFrame = true;
	cout << g_AnimationIsRunning << '\n';
}

void GenerateButtonCallbackFunctionF2()
{
	stringstream ss;

	ss << s_terrainSize << ' ' << s_power2Size << ' ' << s_roughness << ' ' << s_heightScaleValue << ' ' << s_numberOfSmoothingIterations;
	ss.seekg(0, ios_base::beg);
	ss >> g_terrainSize >> g_power2Size >> g_roughness >> g_heightScaleValue >> g_numberOfSmoothingIterations;
	ApplyDiamondSquareNextFrame = true;
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
	glutInitWindowPosition(250, 50);
	glutInitWindowSize(1080, 720);
	main_window = glutCreateWindow("OpenGL 3D Terrain Generator");
	glewInit();
	if (!Initialize())
	{
		cout << "Program failed to initialize !\n";
		return 0;
	}
	glutDisplayFunc(RenderFunction);
	//glutIdleFunc(RenderFunction);
	glutKeyboardFunc(processNormalKeys);
	glutSpecialFunc(processSpecialKeys);
	glutCloseFunc(Cleanup);
	glutMotionFunc(MouseMotionFunction);
	glutPassiveMotionFunc(MousePassiveMotionFunction);

	// GLUI
	glui = GLUI_Master.create_glui("GLUI", 0, 0, 0); 

	GLUI_Master.set_glutIdleFunc(RenderFunction);

	glui->add_statictext("Terrain Parameters");
	glui->add_edittext("Terrain Width", s_terrainWidth);
	glui->add_edittext("Terrain Height", s_terrainHeight);

	glui->add_statictext("Perlin Noise parameters:"); 
	
	glui->add_edittext("octaves", s_octaves);
	glui->add_edittext("persistence", s_persistence);
	glui->add_edittext("coords Mult. factor", s_coordMultFactor);
	glui->add_edittext("noise Mult. factor", s_noiseMultFactor);
	glui->add_edittext("Perlin Animation Speed", s_perlinAnimationSpeed);
	glui->add_checkbox("Animation", &g_AnimationIsRunning);
	glui->add_button("Generate", 0, (GLUI_Update_CB) GenerateButtonCallbackFunctionF1);
	
	glui->add_statictext("Diamond-Square parameters:");

	glui->add_edittext("Terrain Size", s_terrainSize);
	glui->add_edittext("Size In Power Of Two", s_power2Size);
	glui->add_edittext("roughness", s_roughness);
	glui->add_edittext("Height scale value", s_heightScaleValue);
	glui->add_edittext("Number of Smoothing Iterations", s_numberOfSmoothingIterations);
	glui->add_button("Generate", 0, (GLUI_Update_CB)GenerateButtonCallbackFunctionF2);
	
	//new GLUI_Separator(glui);
	//checkbox = new GLUI_Checkbox(glui, "Wireframe", &wireframe, 1, control_cb);
	//spinner = new GLUI_Spinner(glui, "Segments:", &segments, 2, control_cb);
	//spinner->set_int_limits(3, 60);
	/*
	edittext = new GLUI_EditText(glui, "Text:", "sample...");
	GLUI_Panel *obj_panel = new GLUI_Panel(glui, "Object Type");
	radio = new GLUI_RadioGroup(obj_panel, &obj, 4);
	new GLUI_RadioButton(radio, "Sphere");
	new GLUI_RadioButton(radio, "Torus");
	new GLUI_RadioButton(radio, "Teapot");
	new GLUI_Button(glui, "Quit", 0, (GLUI_Update_CB)exit);
*/
	//glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	//GLUI_Master.set_glutIdleFunc( myGlutIdle );
	//GLUI_Master.set_glutIdleFunc(NULL);

	glutMainLoop();

	getchar();
}

