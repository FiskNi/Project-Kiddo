#include "Loader.h"

//#include <string>
Loader::Loader(std::string fileName)
{
	this->fileName = fileName;

	// =========================================
	// Create input stream variable for the file,
	// Open the file as binary
	// =========================================
	std::ifstream binFile(this->fileName, std::ios::binary); 

	// =========================================
	// Error handling, if file can't be opened
	// =========================================
	if (!binFile)
	{
		std::cout << "COULD NOT FIND FILE: '" << (char*)&fileName 
			<< "' PROGRAM WILL MOST LIKELY CRASH." << std::endl;
		system("pause");
	}

	//==========================================
	// reading length of the file
	//==========================================
	binFile.seekg(0, binFile.end);
	int aLength = binFile.tellg();
	binFile.seekg(0, binFile.beg); 

	//==========================================
	// Read the exact size of mehHeader AKA size
	// of Int for now which = 4 bytes.
	// Each time we read the sizeof a struct we're 
	// really reading the size of all it contains
	// so if it has 2 ints that'd be 8 bytes.
	//==========================================


	//binFile.read((char*)&this->meshCount, sizeof(int));
	//std::cout << meshCount << std::endl;

	//Mesh
	binFile.read((char*)&this->fileHeader, sizeof(MehHeader));
	std::cout
		<< "meshCount: " << fileHeader.meshCount << std::endl
		<< "groupCount: " << fileHeader.groupCount << std::endl
		<< "materialCount: " << fileHeader.materialCount << std::endl
		<< "pointLightCount: " << fileHeader.pointLightCount << std::endl
		<< "dirLightCount: " << fileHeader.dirLightCount << std::endl;

	//==========================================
	// Size of file in bytes/length of file in chars.
	// std::cout << aLength << std::endl;
	//==========================================
	this->meshGroup = new MeshGroup[fileHeader.groupCount ];
	this->mesh = new LoadedMesh[fileHeader.meshCount];
	this->meshVert = new MeshVert[fileHeader.meshCount];
	this->material = new PhongMaterial[fileHeader.materialCount ];

	for (int i = 0; i < fileHeader.groupCount ; i++)
	{
		binFile.read((char*)&this->meshGroup[i], sizeof(MeshGroup));
		std::cout
			<< "Group Name: " << meshGroup[i].groupName << std::endl
			<< "translation: " << meshGroup[i].translation << std::endl
			<< "rotation: " << meshGroup[i].rotation << std::endl
			<< "scale: " << meshGroup[i].scale << std::endl
			<< "isChild: " << meshGroup[i].isChild << std::endl
			<< "parent name: " << meshGroup[i].parentName << std::endl
			<< "parent type: " << meshGroup[i].parentType << std::endl;
	}

	for (int i = 0; i < fileHeader.meshCount; i++)
	{

		binFile.read((char*)&this->mesh[i], sizeof(LoadedMesh));

		// Allocate memory for the array of vertex arrays
		meshVert[i].vertices = new Vertex[mesh[i].vertexCount];
		
		//Read data for all the vertices, this includes pos, uv, normals, tangents and binormals.
		for (int j = 0; j < mesh[i].vertexCount; j++)
		{
			binFile.read((char*)&this->meshVert[i].vertices[j], sizeof(Vertex));
		}

	}

	for (int i = 0; i < fileHeader.materialCount ; i++)
	{
		binFile.read((char*)&this->material[i], sizeof(PhongMaterial));
	}

	binFile.close();
}

Loader::~Loader()
{
	for (unsigned int i = 0; i < this->fileHeader.meshCount; i++)
	{
		/*for (unsigned int j = 0; j < this->meshArr[i].nrOfMaterials; j++)
		{
			delete[] this->textures[i][j];
		}*/
		//delete[] this->textures[i];
		//delete[] this->vertexArr[i];
		//delete[] this->materialArr[i];
		if (meshVert[i].vertices)
			delete[] this->meshVert[i].vertices;
	}
	delete[] this->mesh;
	delete[] this->material;
	//delete[] this->textures;
	//delete[] this->vertexArr;
	//delete[] this->meshArr;
	//delete[] this->materialArr;
}

int Loader::getHeader()
{
	return this->fileHeader.meshCount;
}

int Loader::getNrOfMeshes()
{
	return this->fileHeader.meshCount;
}

std::string Loader::getFileName()
{
	return this->fileName;
}

Vertex* Loader::getVerticies(int meshID)
{
	return this->meshVert[meshID].vertices;
}

int Loader::getNrOfVerticies(int meshID)
{
	return this->mesh[meshID].vertexCount;
}

char* Loader::getAlbedo(int materialID)const
//MehHeader Loader::GetMeshData(int index) const
//{
//	MehHeader temp;
//
//
//	return tmep;
//}

char* Loader::getAlbedo()const
{
	return material[materialID].albedo;
}

char* Loader::getNormal(int materialID)const
{
	return material[materialID].normal;
}

PhongMaterial Loader::GetMaterial(int i) const
{
	return material[i];
}

int Loader::GetMaterialCount() const
{
	return materialCount;
}

PhongMaterial * Loader::getAllMaterials(int meshID)
{
	return nullptr;
}



