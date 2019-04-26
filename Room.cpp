#include "Room.h"



Room::Room(std::vector<Material> materials)
{
	LoadLights();
	LoadEntities(materials);
	LoadPuzzleNode(materials);



	// Initialize camera (Default constructor)
	roomCamera = new Camera;

	// Compile all the mesh data in the room for the renderer
	// This will first get picked up by the owning scene
	CompileMeshData();
}

Room::~Room()
{
}

std::vector<Light>& Room::GetPointLights()
{
	return pointLights;
}

std::vector<DirectionalLight> Room::GetDirectionalLights() const
{
	return dirLights;
}

std::vector<RigidEntity>& Room::GetRigids()
{
	return rigids;
}

std::vector<StaticEntity>& Room::GetStatics()
{
	return statics;
}

std::vector<BoxHoldEntity>& Room::GetBoxHolds()
{
	return this->holdBoxes;
}

std::vector<puzzleNode> Room::GetNodes() const
{
	return nodes;
}

std::vector<Primitive> Room::GetMeshData() const
{
	return meshes;
}

Camera* Room::GetCamera()
{
	return roomCamera;
}

//=============================================================
//	Render update
//	Compiles mesh data for the renderer
//=============================================================
void Room::CompileMeshData()
{
	meshes.clear();

	for (int i = 0; i < this->importMeshes.size(); i++)
	{
		meshes.push_back(this->importMeshes[i]);
	}

	for (int i = 0; i < rigids.size(); i++)
	{
		meshes.push_back(rigids[i].GetMeshData());
	}

	for (int i = 0; i < statics.size(); i++)
	{
		meshes.push_back(statics[i].GetMeshData());
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		meshes.push_back(nodes[i].GetMeshData());
	}
	for (int i = 0; i < holdBoxes.size(); i++) {
		if(holdBoxes[i].holdingBox())
			meshes.push_back(holdBoxes[i].GetMeshData());
	}
}

//=============================================================
//	Light initialization
//	Loads and positions all the lights in the scene
//=============================================================
void Room::LoadLights()
{
	Light light;
	light.setDiffuse(glm::vec3(1.0f, 0.3f, 0.5f));
	light.setSpecular(glm::vec3(1.0f, 0.3f, 0.5f));

	light.setLightPos(glm::vec3(3.0f, 1.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 7.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, -3.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 2.0f));
	pointLights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 7.0f));
	pointLights.push_back(light);

	DirectionalLight light2;
	dirLights.push_back(light2);
}

//=============================================================
//	Entity initialization
//	Loads and positions all the entities in the scene
//=============================================================
void Room::LoadEntities(std::vector<Material> materials)
{
	RigidEntity cubeEntity(1);

	Loader testLoader("LevelTestBinary.bin");

	for (int i = 0; i < testLoader.getNrOfMeshes(); i++)
	{
		Primitive newPrim;
		newPrim.setPosition(glm::vec3(0, 0.0f, 0));
		newPrim.setMaterial(materials[0].getMaterialID());

		this->importMeshes.push_back(newPrim);

		this->importMeshes[i].ImportMesh(testLoader.getVerticies(i), testLoader.getNrOfVerticies(i));
	}

	rigids.reserve(6);



	cubeEntity.SetMaterialID(materials[0].getMaterialID());

	cubeEntity.SetPosition(glm::vec3(3.0f, 1.0f, -3.0f));
	cubeEntity.setStartPosition(glm::vec3(3.0f, 1.0f, -3.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(3.0f, 1.0f, 2.0f));
	cubeEntity.setStartPosition(glm::vec3(3.0f, 1.0f, 2.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(3.0f, 1.0f, 7.0f));
	cubeEntity.setStartPosition(glm::vec3(3.0f, 1.0f, 7.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(-3.0f, 1.0f, -3.0f));
	cubeEntity.setStartPosition(glm::vec3(-3.0f, 1.0f, -3.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(-3.0f, 1.0f, 2.0f));
	cubeEntity.setStartPosition(glm::vec3(-3.0f, 1.0f, 2.0f));
	rigids.push_back(cubeEntity);

	cubeEntity.SetPosition(glm::vec3(-3.0f, 1.0f, 7.0f));
	cubeEntity.setStartPosition(glm::vec3(-3.0f, 1.0f, 7.0f));
	rigids.push_back(cubeEntity);
	
	StaticEntity planeEntity(0);
	planeEntity.SetMaterialID(materials[0].getMaterialID());
	planeEntity.SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	statics.push_back(planeEntity);

	BoxHoldEntity boxHold(1);
	boxHold.SetMaterialID(materials[0].getMaterialID());
	boxHold.SetPosition(glm::vec3(-11, -1, 0));
	holdBoxes.push_back(boxHold);

	boxHold.SetPosition(glm::vec3(-12, -1, 0));
	holdBoxes.push_back(boxHold);

}

//=============================================================
//	Puzzle node initialization
//	Loads and positions all the puzzle nodes in the scene
//=============================================================
void Room::LoadPuzzleNode(std::vector<Material> materials)
{
	puzzleNode winNode;
	winNode.SetMaterialID(materials[3].getMaterialID());
	winNode.SetPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	nodes.push_back(winNode);
}

