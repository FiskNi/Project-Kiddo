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

	// Initialize camera
	roomCamera = new Camera;

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

std::vector<Entity> Room::GetEntities()
{
	return entities;
}

std::vector<Primitive> Room::GetMeshData() const
{
	return meshes;
}

Camera* Room::GetCamera()
{
	return roomCamera;
}

void Room::MoveEntity(unsigned int i, glm::vec3 newPos)
{
	entities[i].setPosition(newPos);
}

void Room::CompileMeshData()
{
	meshes.clear();
	meshes.push_back(groundPlane);

	for (int i = 0; i < entities.size(); i++)
	{
		meshes.push_back(entities[i].getMeshData());
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		meshes.push_back(nodes[i].getMeshData());
	}

}

void Room::LoadLights()
{
	// Initialize lights
	// Could be stored in a light handler class instead
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

void Room::LoadEntities(std::vector<Material> materials)
{
	// Initialize Entities
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

void Room::LoadPuzzleNode(std::vector<Material> materials)
{
	puzzleNode winNode;
	winNode.setMaterialID(materials[3].getMaterialID());
	winNode.setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	nodes.push_back(winNode);
}

