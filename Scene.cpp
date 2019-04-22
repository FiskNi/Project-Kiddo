#include "Scene.h"



Scene::Scene()
{
	// Loads content | *Each function could return a bool incase of failure
	LoadShaders();
	LoadMaterials();
	LoadCharacter();

	startingRoom = new Room(materials);

	CompileMeshData();
}

Scene::~Scene()
{
	delete startingRoom;
}

std::vector<Light> Scene::GetPointLights() const
{
	return startingRoom->GetPointLights();
}
std::vector<DirectionalLight> Scene::GetDirectionalLights() const
{
	return startingRoom->GetDirectionalLights();
}

std::vector<Material> Scene::GetMaterials() const
{
	return materials;
}

Shader Scene::GetShader(unsigned int i) const
{
	return shaders[i];
}

std::vector<Primitive> Scene::GetMeshData() const
{
	return meshes;
}

void Scene::LoadShaders()
{
	// Load shaders
	basicShader.CreateShader("VertexShader.glsl", "Fragment.glsl");
	shaders.push_back(basicShader);

	shadowmapShader.CreateShader("VertexShaderSM.glsl", "FragmentSM.glsl");
	shaders.push_back(shadowmapShader);

	// Initialize fullscreen quad vertices
	// Right now the fullscreen quad is coded into the shader handler.
	// Could be moved and better organized
	fsqShader.CreateFSShaders();
	fsqShader.CreateFullScreenQuad();
	shaders.push_back(fsqShader);
}

void Scene::LoadMaterials()
{
	// Initialize materials and textures
	// The constructor integer is the material id slot
	// So the first material has id #0 (materials is size 0), second has id #1, and so on
	Material planeMat("Plane Material", materials.size());
	planeMat.createAlbedo("Resources/Textures/brickwall.jpg");
	planeMat.createNormal("Resources/Textures/brickwall_normal.jpg");
	materials.push_back(planeMat);

	Material cubeMat("Plane Cube Material", materials.size());
	cubeMat.createAlbedo("Resources/Textures/boxTexture.png");
	materials.push_back(cubeMat);

	Material playerMat("Player Material", materials.size());
	playerMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(playerMat);

	Material nodeMat("Node Material", materials.size());
	nodeMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(nodeMat);
}

void Scene::LoadCharacter()
{
	// Could be improved instead of having a specific integer #, example a named integer "playerMaterial"
	playerCharacter.setMaterialID(2);
}

void Scene::CompileMeshData()
{
	startingRoom->CompileMeshData();
	meshes.clear();
	meshes = startingRoom->GetMeshData();
	meshes.push_back(playerCharacter.getMeshData());
}

Camera Scene::GetCamera() const
{
	return *(startingRoom->GetCamera());
}

void Scene::Update(GLFWwindow* renderWindow, float deltaTime)
{
	startingRoom->GetCamera()->FPSCamControls(renderWindow, deltaTime);

	// Character Handling
	bool collision = false;

	// Check a potential new position
	glm::vec3 newPos = playerCharacter.Move(renderWindow, deltaTime);

	int dominatingBox = -1;
	PlayerBoxCollision(collision, newPos, dominatingBox);
	BoxBoxCollision(dominatingBox);

	if (!collision)
	{
		playerCharacter.setPosition(newPos);
		collision = false;
	}

	CompileMeshData();
}

void Scene::PlayerBoxCollision(bool& collision, glm::vec3 &newPos, int& dominatingBox)
{
	for (int i = 0; i < startingRoom->GetEntities().size(); ++i)
	{
		if (playerCharacter.CheckCollision(startingRoom->GetEntities()[i]))
		{
			collision = true;
			// Reset player position (new position is inside a collision this the character has to be moved back again)
			glm::vec3 pushDir = startingRoom->GetEntities()[i].getPosition() - newPos;
			if (abs(pushDir.x) >= abs(pushDir.z))
				pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
			else
				pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);

			pushDir = glm::normalize(pushDir);
			pushDir *= 0.15f;

			startingRoom->MoveEntity(i, startingRoom->GetEntities()[i].getPosition() + pushDir);

			dominatingBox = i;
		}
	}
}

void Scene::BoxBoxCollision(int dominatingBox)
{
	// Could possibly be done with recursion to check subsequent collisions
	// Could be made better with proper physic calculations
	for (int i = 0; i < startingRoom->GetEntities().size(); ++i)
	{
		for (int j = 0; j < startingRoom->GetEntities().size(); ++j)
		{
			if (i != j && startingRoom->GetEntities()[i].CheckCollision(startingRoom->GetEntities()[j]) && j != dominatingBox)
			{
				glm::vec3 pushDir = startingRoom->GetEntities()[j].getPosition() - startingRoom->GetEntities()[i].getPosition();
				if (abs(pushDir.x) >= abs(pushDir.z))
					pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
				else
					pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
				pushDir = glm::normalize(pushDir);
				pushDir *= 0.15f;
				startingRoom->MoveEntity(j, startingRoom->GetEntities()[j].getPosition() + pushDir);
			}
		}
	}
}



