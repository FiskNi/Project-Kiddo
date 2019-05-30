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
#include <iomanip>



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
	bool setUserPointer;

	GAMESTATE state;

	void LoadShaders();
	void LoadMaterials(Loader* inLoader);
	void LoadCharacter();

	void CompileMeshData();

	//void CompileMainMenuData();
	
	// Global world updates
	// Should only be applied to active room
	void Gravity();

	// Shaders
	std::vector<Shader> shaders;
	// This should be created temporarily in the LoadShader funtion
	// Probably move the CreateShader function inside shaders into the constructor instead as well
	Shader basicShader;
	Shader fsqShader;
	Shader shadowmapShader;
	Shader mainMenuShader;

	// Object list for the render queue
	std::vector<Mesh> meshes;

	// Materials are stored in a vector
	std::vector<Material*> materials;

	// Rooms
	Room* roomBuffer;
	//Room* mainMenuRoomBuffer;
	int roomNr;
	bool isLoading;
	bool exittoMenu;
	bool roomLoaded;

	// Character
	Character* playerCharacter;

	//Key_callback
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	//Private functions for key_callback
	void _CheckPressedButtons();
	void _CheckPressedBombs();

	irrklang::ISoundEngine* musicEngine;
	irrklang::ISoundEngine* walkingEngine;

public:
	Scene();
	~Scene();

	std::vector<Light>& GetPointLights()						{ return roomBuffer->GetPointLights(); }
	std::vector<DirectionalLight>& GetDirectionalLights() 		{ return roomBuffer->GetDirectionalLights(); }
	std::vector<Material*>& GetMaterials()						{ return materials; }
	Shader GetShader(unsigned int i) const						{ return shaders[i]; }
	std::vector<Mesh>& GetMeshData()							{ return roomBuffer->GetMeshData(); }
	bool GetIsLoading() const									{ return isLoading; }
	bool GetExit() const										{ return exittoMenu; }
	bool GetRoomLoaded() const									{ return roomLoaded; }
	int GetCurrentState() const									{ return state; }
	Camera GetCamera() const									{ return *(roomBuffer->GetCamera()); }

	void LoadRoom();
	void Update(GLFWwindow* renderWindow, float deltaTime);
	void CharacterUpdates(float deltaTime);
	void ResetRoom();
	void Exited();

	void ResumeGame();
	void RestartGame();
	void ExitToMainMenu();
	void SetCurrentState();

	//void Upgrade() { this->roomBuffer->Upgrade(&this->playerCharacter); }
};

