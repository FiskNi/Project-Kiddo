#pragma once
#include "Renderer.h"
#include "ShadowMap.h"
#include "Primitive.h"
#include "Shader.h"
#include "Material.h"
#include "Room.h"
#include "Character.h"
#include "Camera.h"


class Scene
{
private:
	void LoadShaders();
	void LoadMaterials();
	void LoadCharacter();

	void CompileMeshData();

	void BoxBoxCollision(int dominatingBox);
	void BoxNodeCollision();
	void PlayerBoxCollision(bool& collision, glm::vec3 &newPos, int& dominatingBox);

	// Shaders
	std::vector<Shader> shaders;
	Shader basicShader;
	Shader fsqShader;
	Shader shadowmapShader;

	// Object list for the render queue
	std::vector<Primitive> meshes;

	// Materials are stored in a vector
	std::vector<Material> materials;

	// Rooms
	Room* startingRoom;

	// Character
	Character playerCharacter;

public:
	Scene();
	~Scene();

	std::vector<Light> GetPointLights() const;
	std::vector<DirectionalLight> GetDirectionalLights() const;
	std::vector<Material> GetMaterials() const;
	Shader GetShader(unsigned int i) const;
	std::vector<Primitive> GetMeshData() const;
	Camera GetCamera() const;

	void Update(GLFWwindow* renderWindow, float deltaTime);


};

