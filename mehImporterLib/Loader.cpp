#include "Loader.h"

//#include <string>
Loader::Loader(std::string fileName)
{
	this->fileName = fileName;

	// Create input stream variable for the file
	std::ifstream binFile(this->fileName, std::ios::binary);

	if (!binFile)
	{
		std::cout << "COULD NOT FIND FILE: '" << (char*)&fileName << "' PROGRAM WILL MOST LIKELY CRASH." << std::endl;
		system("pause");
	}

	//reading length of the file
	binFile.seekg(0, binFile.end);
	int aLength = binFile.tellg();
	binFile.seekg(0, binFile.beg);

	//Read the exact size of mehHeader AKA size of Int for now which = 4 bytes.
	//Each time we read the sizeof a struct we're really reading the size of all it contains so if it has 2 ints that'd be 8 bytes.
	binFile.read((char*)&this->header, sizeof(mehHeader));

	std::cout << "NrOfMeshes: " << header.nrOfMeshes << std::endl;

	//Size of file in bytes/length of file in chars.
	//std::cout << aLength << std::endl;

	this->meshArr = new mesh[header.nrOfMeshes];

	//Allocate memory for the array of vertex arrays
	this->vertexArr = new vertex*[header.nrOfMeshes];

	//Allocate memory for the array of material arrays.
	this->materialArr = new material*[header.nrOfMeshes];

	//Allocate memory for the outermost part of the texture array.
	//The order goes like this meshID->MaterialID->TextureID. 
	//so textures[0][0][0] would refer to mesh in place 0, material in place 0 and texture nr 0 in that material.
	this->textures = new texture**[header.nrOfMeshes];

	//The for-loop for each mesh
	for (unsigned int i = 0; i < header.nrOfMeshes; i++)
	{
		//Read the size of Mesh from the file. And put it in meshArr[i]
		binFile.read((char*)&this->meshArr[i], sizeof(mesh));

		//All Couts are just for testing so the .bin file isn't broken. 
		//COMMENT OUT BEFORE GIT PUSH
		//Otherwise it'll slow down start up for everyone else and print a lot of info to the console.
		/*std::cout << header.nrOfMeshes << std::endl;
		std::cout << meshArr[i].meshName << std::endl;
		std::cout << meshArr[i].nrOfMaterials << std::endl;
		std::cout << meshArr[i].nrOfVerticies << std::endl;*/

		//Allocate memory for each vertex in this mesh.
		this->vertexArr[i] = new vertex[this->meshArr[i].nrOfVerticies];

		//COMMENT OUT BEFORE GIT PUSH
		/*std::cout << "Mesh Nr: " << i << std::endl;
		std::cout << "NrOfVerticies: " << meshArr[i].nrOfVerticies << std::endl;*/

		//For-loop for each vertex
		for (unsigned int j = 0; j < this->meshArr[i].nrOfVerticies; j++)
		{
			/*std::cout << "Vert Nr: " << j << std::endl;*/
			binFile.read((char*)&this->vertexArr[i][j], sizeof(vertex));


			//COMMENT OUT BEFORE GIT PUSH
			/*std::cout << this->vertexArr[i][j].pos[0] <<
				"||" << this->vertexArr[i][j].pos[1] <<
				"||" << this->vertexArr[i][j].pos[2] << std::endl;

			std::cout << "----" << std::endl;

			std::cout << this->vertexArr[i][j].uv[0] <<
				"||" << this->vertexArr[i][j].uv[1] << std::endl;

			std::cout << "----" << std::endl;

			std::cout << this->vertexArr[i][j].normal[0] <<
				"||" << this->vertexArr[i][j].normal[1] <<
				"||" << this->vertexArr[i][j].normal[2] << std::endl;

			std::cout << "----" << std::endl;

			std::cout << this->vertexArr[i][j].tangent[0] <<
				"||" << this->vertexArr[i][j].tangent[1] <<
				"||" << this->vertexArr[i][j].tangent[2] << std::endl;

			std::cout << "----" << std::endl;

			std::cout << this->vertexArr[i][j].biNormal[0] <<
				"||" << this->vertexArr[i][j].biNormal[1] <<
				"||" << this->vertexArr[i][j].biNormal[2] << std::endl;

			std::cout << "____________________________" << std::endl;*/
		}
		//COMMENT OUT BEFORE GIT PUSH
		/*std::cout << "count: " << meshArr[i].nrOfMaterials << std::endl;*/

		//Allocate memory for each material in this mesh.
		this->materialArr[i] = new material[meshArr[i].nrOfMaterials];
		//Allocate memory for each texture array in this mesh
		this->textures[i] = new texture*[meshArr[i].nrOfMaterials];

		//In case of wanting total amount of textures
		//int totTextures = 0;

		//For-loop for each material in the mesh
		for (unsigned int j = 0; j < meshArr[i].nrOfMaterials; j++)
		{
			binFile.read((char*)&materialArr[i][j], sizeof(material));

			//totTextures += materialArr[i][j].nrOfTextures;

			//COMMENT OUT BEFORE GIT PUSH
			/*std::cout << "MATERIAL DATA" << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;

			std::cout << "matIndex: " << materialArr[i][j].index << std::endl;
			std::cout << "material Name: " << materialArr[i][j].materialName << std::endl;
	
			std::cout << "ambient 0: " << materialArr[i][j].ambient[0] << std::endl;
			std::cout << "ambient 1: " << materialArr[i][j].ambient[1] << std::endl;
			std::cout << "ambient 2: " << materialArr[i][j].ambient[2] << std::endl;

			std::cout << "diffuse 0: " << materialArr[i][j].diffuse[0] << std::endl;
			std::cout << "diffuse 1: " << materialArr[i][j].diffuse[1] << std::endl;
			std::cout << "diffuse 2: " << materialArr[i][j].diffuse[2] << std::endl;

			std::cout << "Specular 0: " << materialArr[i][j].specular[0] << std::endl;
			std::cout << "Specular 1: " << materialArr[i][j].specular[1] << std::endl;
			std::cout << "Specular 2: " << materialArr[i][j].specular[2] << std::endl;

			std::cout << "Emissive 0: " << materialArr[i][j].emissive[0] << std::endl;
			std::cout << "Emissive 1: " << materialArr[i][j].emissive[1] << std::endl;
			std::cout << "Emissive 2: " << materialArr[i][j].emissive[2] << std::endl;

			std::cout << "Opacity: " << materialArr[i][j].opacity << std::endl;
			std::cout << "Shininess: " << materialArr[i][j].shininess << std::endl;
			std::cout << "Reflectivity: " << materialArr[i][j].reflectivity << std::endl;
			std::cout << "nrOfTextures: " << materialArr[i][j].nrOfTextures << std::endl;

			std::cout << "MATERIAL END" << std::endl;
			std::cout << "-------------------------------------------------------" << std::endl;*/

			//Allocate Memory for each texture in the material.
			this->textures[i][j] = new texture[materialArr[i][j].nrOfTextures];

			//For-loop for each texture in the material
			for (unsigned int k = 0; k < materialArr[i][j].nrOfTextures; k++)
			{
				//alternatively this could be read after all materials and save the k to texture.matIndex
				//But then we'd have to go through each material again just to get the texture count.
				//Of course we COULD store a mesh index in the texture as well to avoid a triple pointer. But then we'd have unnecessary variables. 
				binFile.read((char*)&this->textures[i][j][k], sizeof(texture));

				//COMMENT OUT BEFORE GIT PUSH
				/*std::cout << "Material Index: " << textures[i][j][k].MatIndex << std::endl;
				std::cout << "Texture Index: " << textures[i][j][k].texID << std::endl;
				std::cout << "Texture Name: " << textures[i][j][k].textureName << std::endl;

				std::cout << "TEXTURE END" << std::endl;
				std::cout << "-------------------------------------------------------" << std::endl;*/

			}
		}

	}

	binFile.close();
}

Loader::~Loader()
{
	for (unsigned int i = 0; i < this->header.nrOfMeshes; i++)
	{
		for (unsigned int j = 0; j < this->meshArr[i].nrOfMaterials; j++)
		{
			delete[] this->textures[i][j];
		}
		delete[] this->textures[i];
		delete[] this->vertexArr[i];
		delete[] this->materialArr[i];
	}
	delete[] this->textures;
	delete[] this->vertexArr;
	delete[] this->meshArr;
	delete[] this->materialArr;
}

int Loader::getHeader()
{
	return this->header.nrOfMeshes;
}

int Loader::getNrOfMeshes()
{
	return this->header.nrOfMeshes;
}

std::string Loader::getFileName()
{
	return this->fileName;
}

vertex * Loader::getVerticies(int meshID)
{
	return this->vertexArr[meshID];
}

int Loader::getNrOfVerticies(int meshID)
{
	return this->meshArr[meshID].nrOfVerticies;
}

mesh Loader::getMesh(int meshID)
{
	return mesh();
}

material Loader::getMaterial(int meshID, int materialID)
{
	return material();
}

material * Loader::getAllMaterials(int meshID)
{
	return nullptr;
}

texture Loader::getTexture(int meshID, int materialID, int textureID)
{
	//Just step through the textures triple array with given info.
	return texture();
}

texture ** Loader::getAllTextures(int meshID)
{
	//texture** returnPtr;//This must be allocated to with this->materials[meshID][i].nrOfTextures
	//But that would require a bool variable or something for the destructor to destroy it. We can't destroy it here if we wanna use the array.

	//returnPtr = new texture*[this->meshArr[meshID].nrOfMaterials]

	for (unsigned int i = 0; i < this->meshArr[meshID].nrOfMaterials; i++)
	{
		//returnPtr[i] = new texture[his->materialArr[meshID][i].nrOfTextures]
		for (unsigned int j = 0; j < this->materialArr[meshID][i].nrOfTextures; j++)
		{
			//returnPtr[i][j] = textures[meshID][i][j];//this won't work right now as it has no allocated memory.
		}
	}
	//Should return the returnPtr
	return nullptr;
}
