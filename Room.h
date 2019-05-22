#pragma once
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"

#include "Character.h"
#include "Entity.h"
#include "RigidEntity.h"
#include "StaticEntity.h"
#include "puzzleNode.h"
#include "BridgeEntity.h"

#include "Mesh.h"
#include "Camera.h"
#include "Material.h"
#include "puzzleNode.h"
#include "BoxHoldEntity.h"
#include "PressurePlate.h"
#include "Button.h"
#include "Item.h"
#include "boxHolder.h"
#include "MeshGroupClass.h"

#include "Door.h"
#include "Collectible.h"
//============================================================================
//	- Rooms
//	A room will hold everything that doesn't get transfered between rooms.
//	Examples are boxes, entities, mechancis, and so on that defines
//	a winstate and the stages to get there for a room.
//
//	- Usage:
//	Add entites that makes up mechanics and pushable boxes into the room class.
//	Cameras also goes into the room class.
//																
//	For the room to render, everything needs to be added into a mesh vector.
//	This makes it easy to render and de-render specific content.
//============================================================================

class Room
{
private:
	void LoadLights(Loader* inLoader);
	void LoadEntities(std::vector<Material> materials, Loader* level);
	void PlayerRigidCollision(Character* playerCharacter);
	int inBoundCheck(Character playerCharacter);
	void RigidRigidCollision();
	void RigidNodeCollision();

	void RigidStaticCollision(Character* playerCharacter);
	void RigidGroundCollision(Character* playerCharacter);
	void BoxHolding(Character* playerCharacter, GLFWwindow* renderWindow);
	void BoxPlateCollision(Character* playerCharacter);
	void ButtonInteract(GLFWwindow* window, Character* playerCharacter);
	void PlayerDoorCollision(Character* playerCharacter);

	void PlayerCollectibleCollision(Character* playerCharacter);
	void PlayerItemCollision(Character* playerCharacter);

	//Neccesary functions for applying parents and hierarchies.
	bool FindParent(Mesh * childMesh);
	bool FindParent(MeshGroupClass * childMeshGroup);
	void SetAllParents();
	glm::vec3 updateChild(Mesh* meshPtr);
	glm::vec3 updateChild(MeshGroupClass* meshPtr);
	void updateChildren();

	//Left over functions for if we don't freeze the objects ahead of exportions.
	//Outdated otherwise.
	std::vector <float> GetParentOffset(Mesh * childMesh);
	std::vector <float> GetParentOffset(MeshGroupClass * childGroup);


	// Object list for the render queue
	std::vector<Mesh> meshes;
	std::vector<Mesh> roomMeshes;

	// Lights
	std::vector<Light> pointLights;
	std::vector<DirectionalLight> dirLights;

	// Entities
	std::vector<RigidEntity> rigids;
	std::vector<StaticEntity> statics;
	std::vector<BridgeEntity> bridges;
	std::vector<BoxHoldEntity> holdBoxes;
	std::vector<PressurePlate> pressurePlates;
	std::vector<Button> buttons;
	std::vector<Item> items;
	std::vector<Door> doors;
	std::vector<Collectible> collectibles;

	std::vector<boxHolder> holders;

	//MeshGroups
	std::vector<MeshGroupClass> meshGroups;
	std::vector<glm::vec3> posOffset;

	// PuzzleNodes
	//std::vector<puzzleNode> nodes;

	// Camera
	Camera* roomCamera;

	bool isRoomCompleted = false;
	bool firstCall;
	int meshAmount;

	irrklang::ISoundEngine* audioEngine;

public:
	Room(std::vector<Material> materials, Loader* aLoader, irrklang::ISoundEngine* audioEngine);
	~Room();

	std::vector<Light>& GetPointLights()						{ return pointLights; }
	std::vector<DirectionalLight> GetDirectionalLights() const	{ return dirLights; }
	std::vector<RigidEntity>& GetRigids()						{ return rigids; }
	std::vector<StaticEntity>& GetStatics()						{ return statics; }
	std::vector<BoxHoldEntity>& GetBoxHolds()					{ return holdBoxes; }
	//std::vector<puzzleNode>& GetNodes()						{ return nodes; }
	std::vector<BridgeEntity>& GetBridges()						{ return bridges; }
	std::vector<Mesh>& GetMeshData()							{ return meshes; }
	std::vector<Mesh>& ModifyMeshData()							{ return meshes; }
	std::vector<Button>& getButtons()							{ return buttons; }
	Camera* GetCamera()											{ return roomCamera; }
	bool GetRoomCompleted()										{ return this->isRoomCompleted; }

	void SetRoomCompleted(bool tf)								{ this->isRoomCompleted = tf; }

	void Update(Character* playerCharacter, GLFWwindow* renderWindow, float deltaTime);

	void BridgeUpdates(GLFWwindow* renderWindow);

	void Upgrade(Character* playerCharacter);
	void CompileMeshData();
};

