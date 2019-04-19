#include "Scene.h"



Scene::Scene()
{
	// Load shaders
	basicShader.CreateShader("VertexShader.glsl", "Fragment.glsl");
	gShaderSM.CreateShader("VertexShaderSM.glsl", "FragmentSM.glsl");

	// Initialize textures
	// The constructor integer is the material id slot
	// Could be improved to keep better track of materials
	Material planeMat(0);
	planeMat.createAlbedo("Resources/Textures/brickwall.jpg");
	planeMat.createNormal("Resources/Textures/brickwall_normal.jpg");
	materials.push_back(planeMat);

	Material cubeMat(1);
	cubeMat.createAlbedo("Resources/Textures/boxTexture.png");
	materials.push_back(cubeMat);

	Material playerMat(2);
	playerMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(playerMat);

	Material nodeMat(3);
	nodeMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(nodeMat);

	// Initialize lights
	// Could be stored in a light handler class instead
	Light light;
	light.setDiffuse(glm::vec3(1.0f, 0.3f, 0.5f));
	light.setSpecular(glm::vec3(1.0f, 0.3f, 0.5f));

	light.setLightPos(glm::vec3(3.0f, 1.0f, -3.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 2.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 7.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, -3.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 2.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 7.0f));
	lights.push_back(light);


}

Scene::~Scene()
{
}

