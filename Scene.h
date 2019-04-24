#pragma once
#include "Renderer.h"
#include "ShadowMap.h"
#include "Primitive.h"
#include "Shader.h"
#include "Material.h"
#include "Room.h"
#include "Character.h"
#include "Camera.h"

//============================================================================
//	- Scenes
//	A scene can be seen as an entire "Level". Compare to public game-engines such as Unity or Unreal.
//	Everything that defines current states and everything that gets transfered between rooms goes here.
//	Examples are materials and the character, also other gamestate changes relevant to that level.
//													(Currently relevant to the entire game itself).
//	*Right now there is no need for additional levels for the game project "Project Kiddo" though the usage
//	 of the scene is still relevant.
//
//	- Usage:
//	Put all the rooms into this class. The character also goes here.
//	Any updates that happens in the room and world is currently handled by the 'Updates' function.
//	Any specifc or larger code segements should be made into their own functions and put in there.									
//		Examples are the collision functions run in the Updates function
//	
//	*Note
//		Currently materials and shaders are also handled by the scene class even though these could
//		potentially be carried between scenes. This can be changed in the future or left as is.
//		Additionally updates could be split into handler for each relevant object.
//			(Example "room handler" or "entity handler")
//		As the project grows the need for these should show itself.
//============================================================================

class Scene
{
private:
	void LoadShaders();
	void LoadMaterials();
	void LoadCharacter();

	void CompileMeshData();

	void PlayerBoxCollision(glm::vec3 &newPos, int& dominatingBox, int meshIndex);
	void BoxBoxCollision(int dominatingBox);
	void BoxNodeCollision();
	unsigned int inBoundCheck(bool& collision);
	void RigidStaticCollision();
	void RigidGroundCollision();
	void ApplyGravity();
	

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

