#include "Loader.h"
//#include <string>
Loader::Loader(std::string fileName)
{
	this->fileName = fileName;

	this->meshGroup = nullptr;
	this->material = nullptr;
	this->mesh = nullptr;
	this->dirLight = nullptr;
	this->pointLight = nullptr;
	this->meshVert = nullptr;


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
				
			// 3.3 Joints
			MeshSkeleton newSkeleton;
			// Allocate memory for the joint vector inside
			newSkeleton.joint.resize(mesh[i].skeleton.jointCount);
			for (int j = 0; j < mesh[i].skeleton.jointCount; j++)
			{
				Joint newJoint;
				std::cout << "Writing joint " << j << "..." << std::endl;
				binFile.read((char*)&newJoint, sizeof(Joint));
				newSkeleton.joint[j] = newJoint;
			}


			MeshAnis newAnimations;
			// Allocate memory for the animation vector inside
			newAnimations.animations.resize(mesh[i].skeleton.aniCount);
			for (int a = 0; a < mesh[i].skeleton.aniCount; a++)
			{
				// 3.4.1 Animations
				Animation newAni;
				std::cout << "Writing animation " << a << "..." << std::endl;
				binFile.read((char*)&newAni, sizeof(Animation));
				// Apply the data about the animation and
				// Allocate memory for the keyframe vector inside
				newAnimations.animations[a].ani = newAni;
				newAnimations.animations[a].keyFrames.resize(newAni.keyframeCount);
				for (int k = 0; k < newAni.keyframeCount; k++)
				{
					// 3.4.2 Keyframes
					KeyFrame newKey;
					std::cout << "Writing keyframe " << k << "..." << std::endl;
					binFile.read((char*)&newKey, sizeof(KeyFrame));
					// Apply the data about the keyframe and
					// Allocate memory for the transform vector inside 
					newAnimations.animations[a].keyFrames[k].key = newKey;
					newAnimations.animations[a].keyFrames[k].transforms.resize(newKey.transformCount);
					for (int t = 0; t < newKey.transformCount; t++)
					{
						// 3.4.3 Transforms
						Transform newTr;
						binFile.read((char*)&newTr, sizeof(Transform));
						// Apply the data about the transform
						newAnimations.animations[a].keyFrames[k].transforms[t].t = newTr;
					}
				}
			}

			animationsD.push_back(newAnimations);
			skeletonsD.push_back(newSkeleton);

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
	if (meshGroup)
		delete[] meshGroup;
	if (material)
		delete[] material;
	if (mesh)
		delete[] mesh;
	if (dirLight)
		delete[] dirLight;
	if (pointLight)
		delete[] pointLight;

	for (int i = 0; i < this->fileHeader.meshCount; i++)
		if (meshVert[i].vertices)
			delete[] meshVert[i].vertices;

	if (meshVert)
		delete[] meshVert;
}



