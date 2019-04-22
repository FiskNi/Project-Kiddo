#pragma once
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Entity.h"
#include "puzzleNode.h"
#include "Primitive.h"
#include "Camera.h"

class Room
{
private:
	void LoadLights();
	void LoadEntities();



	// Object list for the render queue
	std::vector<Primitive> meshes;

	// Lights are stored in a vector
	std::vector<Light> pointLights;
	std::vector<DirectionalLight> dirLights;

	// Entity
	std::vector<Entity> entities;
	
	// Temporary primitive
	Primitive groundPlane;

	// Camera
	Camera roomCamera;

public:
	Room();
	~Room();

	std::vector<Light> GetPointLights() const;
	std::vector<DirectionalLight> GetDirectionalLights() const;
	std::vector<Entity> GetEntities() const;
	unsigned int GetEntitiesSize() const;
	std::vector<Primitive> GetMeshData() const;
	Camera GetCamera();

	void CompileMeshData();
	void Update();

};

