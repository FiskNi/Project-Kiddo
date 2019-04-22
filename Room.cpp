#include "Room.h"



Room::Room()
{
	LoadLights();
	LoadEntities();


	// Initialize plane (ground)
	groundPlane.CreatePlaneData();
	groundPlane.setPosition(glm::vec3(0.0f, -0.5f, 0.0f));

	// ** Need method to get variables from class above
	//groundPlane.setMaterial(materials[0].getMaterialID());
	//objects.push_back(groundPlane);
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

unsigned int Room::GetEntitiesSize() const
{
	return entities.size();
}

std::vector<Primitive> Room::GetMeshData() const
{
	return meshes;
}

Camera Room::GetCamera()
{
	return roomCamera;
}

void Room::CompileMeshData()
{
	meshes.clear();
	for (int i = 0; i < entities.size(); i++)
	{
		meshes.push_back(entities[i].getMeshData());
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

void Room::LoadEntities()
{
	// Initialize Entities
	Entity cubeEntity;

	// ** Need method to get variables from class above
	//cubeEntity.setMaterialID(materials[0].getMaterialID());

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

