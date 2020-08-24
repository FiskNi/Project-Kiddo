#pragma once
#include "Rendering/Camera.h"
#include "Lights/Light.h"
#include "Lights/DirectionalLight.h"

#include "Entities/Character.h"
#include "Entities/Entity.h"
#include "Entities/RigidEntity.h"
#include "Entities/StaticEntity.h"
#include "Entities/BridgeEntity.h"
#include "Entities/Plushie.h"

#include "Rendering/Mesh.h"
#include "Rendering/Camera.h"
#include "Rendering/Material.h"
#include "Entities/BoxHoldEntity.h"
#include "Entities/PressurePlate.h"
#include "Entities/Button.h"
#include "Entities/Item.h"
#include "Entities/boxHolder.h"
#include "MeshGroupClass.h"

#include "Entities/Door.h"
#include "Entities/Collectible.h"
#include "Entities/ColPlane.h"
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
	void LoadEntities(Loader* level);
	void PlayerRigidCollision(Character* playerCharacter);
	void RigidRigidCollision();
	void RigidNodeCollision();
	int inBoundCheck(Character* playerCharacter);

	void RigidStaticCollision(Character* playerCharacter);
	void RigidGroundCollision(Character* playerCharacter);
	void BoxPlateCollision(Character* playerCharacter);
	void ButtonInteract(GLFWwindow* window, Character* playerCharacter);
	void PlayerDoorCollision(Character* playerCharacter);
	void PlushieCollision(Character* playerCharacter);

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
	std::vector<float> GetParentOffset(Mesh * childMesh);
	std::vector<float> GetParentOffset(MeshGroupClass * childGroup);

	// Object list for the render queue
	std::vector<Mesh*> meshes;
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
	std::vector<ColPlane> colPlanes;
	std::vector<boxHolder> holders;
	std::vector<Plushie> plushes;


	//MeshGroups
	std::vector<MeshGroupClass> meshGroups;
	std::vector<glm::vec3> posOffset;

	// Camera
	Camera* roomCamera;

	bool isRoomCompleted;
	bool firstCall;
	int meshAmount;

	irrklang::ISoundEngine* musicEngine;
	irrklang::ISoundEngine* boxEngine;

public:
	Room(Loader* aLoader, irrklang::ISoundEngine* musicEngine);
	~Room();

	void BoxHolding(Character* playerCharacter, GLFWwindow* renderWindow);
	void NewBoxHolding(Character* playerCharacter);
	void ReleaseBox(Character* playerCharacter);
	void DragBox(Character* playerCharacter);
	void CheckIfBoxIsStillInbound(Character* playerCharacter);
	void SetHoldPosition(Character* playerCharacter, int i);

	std::vector<Light>& GetPointLights()					{ return pointLights; }
	std::vector<DirectionalLight>& GetDirectionalLights()	{ return dirLights; }
	std::vector<RigidEntity>& GetRigids()					{ return rigids; }
	std::vector<StaticEntity>& GetStatics()					{ return statics; }
	std::vector<BoxHoldEntity>& GetBoxHolds()				{ return holdBoxes; }
	std::vector<BridgeEntity>& GetBridges()					{ return bridges; }
	std::vector<Mesh*>& GetMeshData()						{ return meshes; }
	std::vector<Button>& getButtons()						{ return buttons; }
	Camera* GetCamera()										{ return roomCamera; }
	bool GetRoomCompleted()									{ return isRoomCompleted; }
	bool PlushIsCollected();

	void SetRoomCompleted(bool tf)							{ isRoomCompleted = tf; }

	void Update(Character* playerCharacter, GLFWwindow* renderWindow, float deltaTime);

	void BridgeUpdates(GLFWwindow* renderWindow);

	void Upgrade(Character* playerCharacter);
	void CompileMeshData();
};

