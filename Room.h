#pragma once
#include "Camera.h"
#include "Light.h"
#include "DirectionalLight.h"

#include "Entity.h"
#include "RigidEntity.h"
#include "StaticEntity.h"

#include "puzzleNode.h"
#include "Primitive.h"
#include "Camera.h"
#include "Material.h"
#include "puzzleNode.h"
#include "BoxHoldEntity.h"
#include "Button.h"
//============================================================================
//	- Rooms
//	A room will hold everything that doesn't get transfered between rooms.
//	Examples are boxes, entities, mechancis, and so on that defines
//	a winstate and the stages to get there for a room.
//
//	- Usage:
//	The entire room mesh itself goes into a room class and can be seen as what defines a room.
//	Add entites that makes up mechanics and pushable boxes into the same room class.
//	Cameras also goes into the room class.
//																
//	For the room to render everything needs to be added into a primitive (later mesh) list.
//	This makes it easy to render and de-render specific rooms and their content.
//============================================================================

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
	std::vector<RigidEntity> rigids;
	std::vector<StaticEntity> statics;
	std::vector<BoxHoldEntity> holdBoxes;

	// PuzzleNode
	std::vector<puzzleNode> nodes;

	std::vector<Primitive> importMeshes;

	// Camera
	Camera* roomCamera;



public:
	Room(std::vector<Material> materials);
	~Room();

	std::vector<Light>& GetPointLights();
	std::vector<DirectionalLight> GetDirectionalLights() const;
	std::vector<RigidEntity>& GetRigids();
	std::vector<StaticEntity>& GetStatics();
	std::vector<BoxHoldEntity>& GetBoxHolds();
	std::vector<puzzleNode> GetNodes() const;
	std::vector<Primitive> GetMeshData() const;
	Camera* GetCamera();

	void CompileMeshData();
};

