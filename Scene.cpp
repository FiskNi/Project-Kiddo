#include "Scene.h"



Scene::Scene()
{
	// Loads content | *Each function could return a bool incase of failure
	LoadShaders();
	LoadMaterials();

	// Initialize fullscreen quad vertices
	// Right now the fullscreen quad is coded into the shader handler.
	// Could be moved and better organized
	fsqShader.CreateFSShaders();
	fsqShader.CreateFullScreenQuad();
}

Scene::~Scene()
{
}

std::vector<Material> Scene::GetMaterials() const
{
	return materials;
}

std::vector<Shader> Scene::GetShaders() const
{
	return shaders;
}

void Scene::LoadShaders()
{
	// Load shaders
	basicShader.CreateShader("VertexShader.glsl", "Fragment.glsl");
	shadowmapShader.CreateShader("VertexShaderSM.glsl", "FragmentSM.glsl");
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
	objects.clear();
	objects = startingRoom.GetMeshData();
}

void Scene::Update()
{

	// ** Need method to get variables from class above
	//startingRoom.GetCamera().FPSCamControls(mainRenderer.getWindow(), deltaTime);

	// Character Handling

	// Check a potential new position
	// ** Need method to get variables from class above
	//glm::vec3 newPos = playerCharacter.Move(mainRenderer.getWindow(), deltaTime);
	glm::vec3 newPos = glm::vec3(0.0f);

	// Check new positions collision of character
	bool collision = false;

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

void Scene::BoxBoxCollision(int dominatingBox)
{
	// Could possibly be done with recursion to check subsequent collisions
	// Could be made better with proper physic calculations
	for (int i = 0; i < startingRoom.GetEntities().size(); ++i)
	{
		for (int j = 0; j < startingRoom.GetEntities().size(); ++j)
		{
			if (i != j && startingRoom.GetEntities()[i].CheckCollision(startingRoom.GetEntities()[j]) && j != dominatingBox)
			{
				glm::vec3 pushDir = startingRoom.GetEntities()[j].getPosition() - startingRoom.GetEntities()[i].getPosition();
				if (abs(pushDir.x) >= abs(pushDir.z))
					pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
				else
					pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
				pushDir = glm::normalize(pushDir);
				pushDir *= 0.15f;
				startingRoom.GetEntities()[j].setPosition(startingRoom.GetEntities()[j].getPosition() + pushDir);
			}
		}
	}
}

void Scene::PlayerBoxCollision(bool &collision, glm::vec3 &newPos, int &dominatingBox)
{
	for (int i = 0; i < startingRoom.GetEntities().size(); ++i)
	{
		if (playerCharacter.CheckCollision(startingRoom.GetEntities()[i]))
		{
			collision = true;
			// Reset player position (new position is inside a collision this the character has to be moved back again)
			glm::vec3 pushDir = startingRoom.GetEntities()[i].getPosition() - newPos;
			if (abs(pushDir.x) >= abs(pushDir.z))
				pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
			else
				pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);

			pushDir = glm::normalize(pushDir);
			pushDir *= 0.15f;
			startingRoom.GetEntities()[i].setPosition(startingRoom.GetEntities()[i].getPosition() + pushDir);
			dominatingBox = i;
		}
	}
}


