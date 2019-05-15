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
		std::cout << "Error: Could not find importer file'" << (char*)&fileName << std::endl;
	}
	else
	{
		//==========================================
		// Read the exact size of mehHeader AKA size
		// of Int for now which = 4 bytes.
		// Each time we read the sizeof a struct we're 
		// really reading the size of all it contains
		// so if it has 2 ints that'd be 8 bytes.
		//==========================================


		//std::cout << things << std::endl;

		//Mesh
		binFile.read((char*)&this->fileHeader, sizeof(MehHeader));

		this->meshGroup = new MeshGroup[fileHeader.groupCount];
		this->mesh = new LoaderMesh[fileHeader.meshCount];
		this->meshVert = new MeshVert[fileHeader.meshCount];
		this->material = new PhongMaterial[fileHeader.materialCount];
		this->dirLight = new DirLight[fileHeader.dirLightCount];
		this->pointLight = new PointLight[fileHeader.pointLightCount];

		for (int i = 0; i < fileHeader.groupCount; i++)
		{
			binFile.read((char*)&this->meshGroup[i], sizeof(MeshGroup));
		}
		MeshGroup a = meshGroup[1];
		for (int i = 0; i < fileHeader.meshCount; i++)
		{

			binFile.read((char*)&this->mesh[i], sizeof(LoaderMesh));

			// Allocate memory for the array of vertex arrays
			meshVert[i].vertices = new Vertex[mesh[i].vertexCount];

			//Read data for all the vertices, this includes pos, uv, normals, tangents and binormals.
			for (int j = 0; j < mesh[i].vertexCount; j++)
			{
				binFile.read((char*)&this->meshVert[i].vertices[j], sizeof(Vertex));
			}

		}

		for (int i = 0; i < fileHeader.materialCount; i++)
		{
			binFile.read((char*)&this->material[i], sizeof(PhongMaterial));

			std::cout << "Albedo name: " << material[i].albedo << std::endl;
			std::cout << "Normal Name: " << material[i].normal << std::endl;
		}

		for (int i = 0; i < fileHeader.dirLightCount; i++)
		{
			binFile.read((char*)&this->dirLight[i], sizeof(float) * 10);
			std::cout << "Directional light" << std::endl;
			std::cout << "Position: " << dirLight[i].position[0] << "  " << dirLight[i].position[1]
				<< "  " << dirLight[i].position[2] << std::endl;

			std::cout << "Rotation: " << dirLight[i].rotation[0] << "  " << dirLight[i].rotation[1]
				<< "  " << dirLight[i].rotation[2] << std::endl;

			std::cout << "Color: " << dirLight[i].color[0] << "  " << dirLight[i].color[1]
				<< "  " << dirLight[i].color[2] << std::endl;

			std::cout << "Intensity: " << dirLight[i].intensity << std::endl;
			//Pos xyz/ rotation xyz/ color xyz/ intensity
		}
		for (int i = 0; i < fileHeader.pointLightCount; i++)
		{
			binFile.read((char*)&this->pointLight[i], sizeof(float) * 7);
			std::cout << "Point light" << std::endl;
			std::cout << "Position: " << pointLight[i].position[0] << "  " << pointLight[i].position[1]
				<< "  " << pointLight[i].position[2] << std::endl;

			std::cout << "Color: " << pointLight[i].color[0] << "  " << pointLight[i].color[1]
				<< "  " << pointLight[i].color[2] << std::endl;

			std::cout << "Intensity: " << pointLight[i].intensity << std::endl;
			//Pos xyz/ color xyz/ intensity
		}
	}
	binFile.close();
}

Loader::~Loader()
{
	// NEEDS TO BE LOOKED OVER AND CONFIRMED FOR NO MEMORY LEAKS
	for (int i = 0; i < this->fileHeader.meshCount; i++)
	{
		if (meshVert[i].vertices)
			delete[] this->meshVert[i].vertices;
	}
	delete[] this->mesh;
	delete[] this->material;
}



