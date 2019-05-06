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

#include "boxHolder.h"
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
	void LoadLights();
	void LoadEntities(std::vector<Material> materials);
	void LoadPuzzleNode(std::vector<Material> materials);

	void PlayerRigidCollision(Character* playerCharacter);
	int inBoundCheck(Character playerCharacter);
	void RigidRigidCollision();
	void RigidNodeCollision();

	void RigidStaticCollision(Character* playerCharacter);
	void RigidGroundCollision(Character* playerCharacter);
	void BoxHolding(Character* playerCharacter, GLFWwindow* renderWindow);
	void BoxPlateCollision();
	void ButtonInteract(GLFWwindow* window, Character* playerCharacter);

	//void 


	// Object list for the render queue
	std::vector<Mesh> meshes;

	// Lights
	std::vector<Light> pointLights;
	std::vector<DirectionalLight> dirLights;

	// Entities
	std::vector<RigidEntity> rigids;
	std::vector<StaticEntity> statics;
	std::vector<BridgeEntity> bridges;
	std::vector<BoxHoldEntity> holdBoxes;
	std::vector<PressurePlate> plates;
	std::vector<Button> buttons;

	std::vector<boxHolder> holders;

	// PuzzleNodes
	std::vector<puzzleNode> nodes;

	// Camera
	Camera* roomCamera;

public:
	Room(std::vector<Material> materials);
	~Room();

	std::vector<Light>& GetPointLights() { return pointLights; }
	std::vector<DirectionalLight> GetDirectionalLights() const { return dirLights; }
	std::vector<RigidEntity>& GetRigids() { return rigids; }
	std::vector<StaticEntity>& GetStatics() { return statics; }
	std::vector<BoxHoldEntity>& GetBoxHolds() { return holdBoxes; }
	std::vector<puzzleNode>& GetNodes() { return nodes; }
	std::vector<BridgeEntity>& GetBridges() { return bridges; }
	std::vector<Mesh> GetMeshData() const { return meshes; }
	std::vector<Button>& getButtons() { return buttons; }
	Camera* GetCamera() { return roomCamera; }

	void Update(Character* playerCharacter, GLFWwindow* renderWindow, float deltaTime);

	void BridgeUpdates(GLFWwindow * renderWindow);



	void CompileMeshData();
};

