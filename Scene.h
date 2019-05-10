#pragma once
#include "Renderer.h"
#include "ShadowMap.h"
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
#include "Room.h"
#include "Menu.h"
#include "Character.h"
#include "Camera.h"
#include "BoxHoldEntity.h"
#include "Headers.h"
//============================================================================
//	- Scenes
//	A scene can be seen as an entire "Level". Compare to public game-engines such as Unity or Unreal.
//	Everything that defines current states and everything that gets transfered between rooms goes here.
//	Examples are materials and the character, also other gamestate changes relevant to that level. 
//						(Currently relevant to the entire game itself since no additional levels).
//												
//	*Right now there is no need for additional levels for the game project "Project Kiddo" though the usage
//	 of the scene is still relevant.
//
//	- Usage:
//	Put all the rooms into this class. The character also goes here.
//	Any updates that happens to the world is currently handled by the 'Updates' function.
//	The scene should keep track of which room is currently active and run updates on the relevant room.
//	
//	*Note
//		Currently materials and shaders are also handled by the scene class even though these could
//		potentially be carried between scenes. This can be changed in the future or left as is.
//============================================================================

class Scene
{
private:
	bool setUserPointer = false;
	/*enum GAMESTATE {
		PAUSED,
		PLAYING,
		MAINMENU
	};*/
	GAMESTATE state = PLAYING;

	void LoadShaders();
	void LoadMaterials(Loader* inLoader);
	void LoadCharacter();
	void LoadLevels();

	void CompileMeshData();
	void CompileMeshDataMainMenu();
	
	// Global world updates
	// Should only be applied to active room
	void Gravity();

	// Shaders
	std::vector<Shader> shaders;
	Shader basicShader;
	Shader fsqShader;
	Shader shadowmapShader;

	// Object list for the render queue
	std::vector<Mesh> meshes;
	Mesh* meshess;

	// Materials are stored in a vector
	std::vector<Material> materials;

	// Rooms
	bool currentBuffer;
	Room* firstRoomBuffer;
	Room* secondRoomBuffer;
	Room* mainMenuRoomBuffer;
	int roomNr;
	bool isSwitched;
	bool isLoading;

	Menu menuHandler;

	// Character
	Character playerCharacter;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	Scene();
	~Scene();

	std::vector<Light> GetPointLights() const { return firstRoomBuffer->GetPointLights(); }
	std::vector<DirectionalLight> GetDirectionalLights() const { return firstRoomBuffer->GetDirectionalLights(); }
	std::vector<Material> GetMaterials() const { return materials; }
	Shader GetShader(unsigned int i) const { return shaders[i]; }
	std::vector<Mesh> GetMeshData() const { return meshes; }
	bool GetIsSwitched() const{ return isSwitched; }
	bool GetIsLoading() const { return isLoading; }
	void SetIsLoading(bool isLoading) { this->isLoading = isLoading; }

	//void SetState() { this->press(); }
	int GetCurrentState() const { return state; };
	Camera GetCamera() const { return *(firstRoomBuffer->GetCamera()); }


	void SwitchRoom();
	void SwitchMainMenu();
	void Update(GLFWwindow* renderWindow, float deltaTime);
	void SetIsSwitched(bool isSwitched);
	void ResetRoom();


};

