#include "Loader.h"
//#include <string>
Loader::Loader(std::string fileName)
{
	this->fileName = fileName;

	this->meshGroup = nullptr;
	this->material = nullptr;
	this->meshVert = nullptr;
	this->mesh = nullptr;
	this->dirLight = nullptr;
	this->pointLight = nullptr;


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

			/*if (mesh[i].skeleton.jointCount > 0)
				this->joints = new Joint[mesh[i].skeleton.jointCount];
			if (mesh[i].skeleton.aniCount > 0)
				this->animations = new Animation[mesh[i].skeleton.aniCount];*/
				
			// 3.3 Joints
			Joint* newJoint = new Joint[mesh[i].skeleton.jointCount];	
			for (int j = 0; j < mesh[i].skeleton.jointCount; j++)
			{
				std::cout << "Writing joint " << j << "..." << std::endl;
				binFile.read((char*)&newJoint[j], sizeof(Joint) * mesh[i].skeleton.jointCount);
			}
			joints.push_back(newJoint);

			// 3.4.1 Animations
			Animation* newAni = new Animation[mesh[i].skeleton.aniCount];
			for (int a = 0; a < mesh[i].skeleton.aniCount; a++)
			{
				std::cout << "Writing animation " << a << "..." << std::endl;
				binFile.read((char*)&newAni[a], sizeof(Animation));

				KeyFrame* newKey = new KeyFrame[newAni[a].keyframeCount];
				for (int k = 0; k < newAni[a].keyframeCount; k++)
				{
					// 3.4.2 Keyframes
					std::cout << "Writing keyframe " << k << "..." << std::endl;
					binFile.read((char*)&newKey[k], sizeof(KeyFrame));

					Transform* newTransform = new Transform[newKey[k].transformCount];
					for (int p = 0; p < newKey[k].transformCount; p++)
					{
						binFile.read((char*)&newTransform[p], sizeof(Transform));
					}
					transforms.push_back(newTransform);
					//delete newTransform;
					newTransform = nullptr;
				}
				keyFrames.push_back(newKey);
				//delete newKey;
				newKey = nullptr;
			}
			animations.push_back(newAni);
			//delete newAni;
			newAni = nullptr;
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
			delete[] meshVert[i].vertices;
	}
	if (mesh)
		delete[] mesh;
	if (material)
		delete[] material;
	if (meshGroup)
		delete[] meshGroup;
	if (dirLight)
		delete[] dirLight;
	if (pointLight)
		delete[] pointLight;
	//for (int i = 0; i < joints.size(); i++)
	//	delete joints[i];
	//for (int i = 0; i < animations.size(); i++)
	//	delete animations[i];
	//for (int i = 0; i < keyFrames.size(); i++)
	//	delete keyFrames[i];
	//for (int i = 0; i < transforms.size(); i++)
	//	delete[] transforms[i];
}



