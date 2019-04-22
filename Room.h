#pragma once
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "puzzleNode.h"
#include "Primitive.h"
#include "Camera.h"
#include "Material.h"
#include "puzzleNode.h"

class Room
{
private:
	void LoadLights();
	void LoadEntities(std::vector<Material> materials);
	void LoadPuzzleNode(std::vector<Material> materials);

	// Object list for the render queue
	std::vector<Primitive> meshes;

	// Lights are stored in a vector
	std::vector<Light> pointLights;
	std::vector<DirectionalLight> dirLights;

	// Entity
	std::vector<Entity> entities;

	// PuzzleNode
	std::vector<puzzleNode> nodes;
	
	// Temporary primitive
	Primitive groundPlane;

	// Camera
	Camera* roomCamera;

public:
	Room(std::vector<Material> materials);
	~Room();

	std::vector<Light> GetPointLights() const;
	std::vector<DirectionalLight> GetDirectionalLights() const;
	std::vector<Entity> GetEntities();
	std::vector<Primitive> GetMeshData() const;
	Camera* GetCamera();

	void MoveEntity(unsigned int i, glm::vec3 newPos);

	void CompileMeshData();


};

