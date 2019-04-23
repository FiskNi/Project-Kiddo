#include "Room.h"



Room::Room(std::vector<Material> materials)
{
	LoadLights();
	LoadEntities(materials);
	LoadPuzzleNode(materials);

	// Initialize plane (ground)
	groundPlane.CreatePlaneData();
	groundPlane.setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	groundPlane.setMaterial(materials[0].getMaterialID());

	testMesh.ImportMesh();
	testMesh.setPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	testMesh.setMaterial(materials[0].getMaterialID());

	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	// Compile all the mesh data in the room for the renderer
	// This will first get picked up by the owning scene
	CompileMeshData();
}

Room::~Room()
{
}

std::vector<Light> Room::GetPointLights() const
{
	return pointLights;
}

std::vector<DirectionalLight> Room::GetDirectionalLights() const
{
	return dirLights;
}

std::vector<Entity> Room::GetEntities() const
{
	return entities;
}

std::vector<puzzleNode> Room::GetNodes() const
{
	return nodes;
}

std::vector<Primitive> Room::GetMeshData() const
{
	return meshes;
}

Camera* Room::GetCamera()
{
	return roomCamera;
}

//=============================================================
//	Room update
//	Move a specific entity belonging to the room
//=============================================================
void Room::MoveEntity(unsigned int i, glm::vec3 newPos)
{
	entities[i].setPosition(newPos);

	glm::vec3 newLightPos = glm::vec3(newPos.x, 1.0f, newPos.z);
	pointLights[i].setLightPos(newLightPos);
}

//=============================================================
//	Render update
//	Compiles mesh data for the renderer
//=============================================================
void Room::CompileMeshData()
{
	meshes.clear();
	meshes.push_back(groundPlane);
	meshes.push_back(testMesh);

	for (int i = 0; i < entities.size(); i++)
	{
		meshes.push_back(entities[i].getMeshData());
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		meshes.push_back(nodes[i].getMeshData());
	}

}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights()
{
	Light light;
	light.setDiffuse(glm::vec3(1.0f, 0.3f, 0.5f));
	light.setSpecular(glm::vec3(1.0f, 0.3f, 0.5f));

	light.setLightPos(glm::vec3(3.0f, 1.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 7.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 7.0f));
	pointLights.push_back(light);

	DirectionalLight light2;
	dirLights.push_back(light2);
}

//=============================================================
//	Entity initialization
//	Loads and positions all the entities in the scene
//=============================================================
void Room::LoadEntities(std::vector<Material> materials)
{
	Entity cubeEntity;
	cubeEntity.setMaterialID(materials[0].getMaterialID());

	cubeEntity.setPosition(glm::vec3(3.0f, 0.0f, -3.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(3.0f, 0.0f, 2.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(3.0f, 0.0f, 7.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(-3.0f, 0.0f, -3.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(-3.0f, 0.0f, 2.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(-3.0f, 0.0f, 7.0f));
	entities.push_back(cubeEntity);
	
}

//=============================================================
//	Puzzle node initialization
//	Loads and positions all the puzzle nodes in the scene
//=============================================================
void Room::LoadPuzzleNode(std::vector<Material> materials)
{
	puzzleNode winNode;
	winNode.setMaterialID(materials[3].getMaterialID());
	winNode.setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	nodes.push_back(winNode);
}

