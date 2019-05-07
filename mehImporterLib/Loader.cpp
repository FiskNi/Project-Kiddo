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
	
	//Mesh
	binFile.read((char*)&this->meshCount, sizeof(unsigned int));
	std::cout << "Mesh count: " << meshCount << std::endl;
	binFile.read((char*)&this->materialCount, sizeof(unsigned int));
	std::cout << "Material count: " << materialCount << std::endl;

	//==========================================
	// Size of file in bytes/length of file in chars.
	// std::cout << aLength << std::endl;
	//==========================================
	this->mesh = new LoadedMesh[meshCount];
	this->meshVert = new MeshVert[meshCount];
	this->material = new PhongMaterial[materialCount];

	for (int i = 0; i < meshCount; i++)
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

	for (int i = 0; i < materialCount; i++)
	{
		binFile.read((char*)&this->material[i], sizeof(PhongMaterial));
	}

	binFile.close();
}

Loader::~Loader()
{
	for (unsigned int i = 0; i < this->meshCount; i++)
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
	return this->meshCount;
}

int Loader::getNrOfMeshes()
{
	return this->meshCount;
}

std::string Loader::getFileName()
{
	return this->fileName;
}

Vertex * Loader::getVerticies(int meshID)
{
	return this->meshVert[meshID].vertices;
}

int Loader::getNrOfVerticies(int meshID)
{
	return this->mesh[meshID].vertexCount;
}

char* Loader::getAlbedo()const
{
	return material->albedo;
}

char* Loader::getNormal()const
{
	return material->normal;
}

LoadedMesh Loader::getMesh(int meshID)
{
	return LoadedMesh();
}

PhongMaterial Loader::getMaterial(int meshID, int materialID)
{
	return PhongMaterial();
}

PhongMaterial * Loader::getAllMaterials(int meshID)
{
	return nullptr;
}

Texture Loader::getTexture(int meshID, int materialID, int textureID)
{
	//Just step through the textures triple array with given info.
	return Texture();
}

//texture ** Loader::getAllTextures(int meshID)
//{
//	//texture** returnPtr;//This must be allocated to with this->materials[meshID][i].nrOfTextures
//	//But that would require a bool variable or something for the destructor to destroy it. We can't destroy it here if we wanna use the array.
//
//	//returnPtr = new texture*[this->meshArr[meshID].nrOfMaterials]
//
//	for (unsigned int i = 0; i < this->meshArr[meshID].nrOfMaterials; i++)
//	{
//		//returnPtr[i] = new texture[his->materialArr[meshID][i].nrOfTextures]
//		for (unsigned int j = 0; j < this->materialArr[meshID][i].nrOfTextures; j++)
//		{
//			//returnPtr[i][j] = textures[meshID][i][j];//this won't work right now as it has no allocated memory.
//		}
//	}
//	//Should return the returnPtr
//	return nullptr;
//}
