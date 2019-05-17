#include "Mesh.h" 

Mesh::Mesh(Vertex* vertArr, unsigned int vertexCount)
{
	this->position			= glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationEulerXYZ	= glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation			= glm::quat(rotationEulerXYZ);
	this->scale				= glm::vec3(1.0f);

	isChild = false;
	myParent = nullptr;
	myGroupParent = nullptr;

	this->materialID = 0;
	ImportMesh(vertArr, vertexCount);
}

Mesh::Mesh(Loader* inLoader, int index)
{
	glm::vec3 ePosition		= glm::vec3(inLoader->GetMesh(index).translation[0], inLoader->GetMesh(index).translation[1], inLoader->GetMesh(index).translation[2]);
	glm::vec3 eRotationXYZ	= glm::vec3(inLoader->GetMesh(index).rotation[0], inLoader->GetMesh(index).rotation[1], inLoader->GetMesh(index).rotation[2]);
	eRotationXYZ			= glm::radians(eRotationXYZ);
	glm::quat eRotation		= glm::quat(eRotationXYZ);
	glm::vec3 eScale		= glm::vec3(inLoader->GetMesh(index).scale[0], inLoader->GetMesh(index).scale[1], inLoader->GetMesh(index).scale[2]);

	name = inLoader->GetMesh(index).name;
	if (inLoader->GetMesh(index).isChild == true && inLoader->GetMesh(index).parentType != -1)
	{
		pName = inLoader->GetMesh(index).parentName;
		isChild = true;
	}
	else
	{
		pName = "NO PARENT FOUND";
		isChild = false;
	}

	this->position = ePosition;
	this->rotation = eRotation;
	this->scale = eScale;

	parentType = inLoader->GetMesh(index).parentType;

	myParent = nullptr;
	myGroupParent = nullptr;

	this->materialID = inLoader->GetMaterialID(index);

	skeleton.joints.resize(inLoader->GetSkeleton(index).jointCount);
	for (int j = 0; j < inLoader->GetSkeleton(index).jointCount; j++)
	{
		SkeletonD::JointD newJoint;
		Joint& jointRef = inLoader->GetJoint(index, j);

		newJoint.name = (string)jointRef.name;
		newJoint.invBindPose = glm::make_mat4(jointRef.invBindPose);
		newJoint.parentIndex = jointRef.parentIndex;

		this->skeleton.joints[j] = newJoint;
	}

	skeleton.animations.resize(inLoader->GetSkeleton(index).aniCount);
	for (int a = 0; a < inLoader->GetSkeleton(index).aniCount; a++)
	{
		SkeletonD::AnimationD newAni;
		Animation& aniRef = inLoader->GetAnimation(index, a);

		newAni.name				= (string)aniRef.name;
		newAni.keyframeFirst	= aniRef.keyframeFirst;
		newAni.keyframeLast		= aniRef.keyframeLast;
		newAni.duration			= aniRef.duration;
		newAni.rate				= aniRef.rate;
		newAni.keyframes.resize(aniRef.keyframeCount);
		for (int k = 0; k < aniRef.keyframeCount; k++)
		{
			SkeletonD::AnimationD::KeyFrameD newKey;
			KeyFrame& keyRef = inLoader->GetKeyFrame(index, a, k);
			newKey.id = keyRef.id;
			newKey.local_joints_T.resize(keyRef.transformCount);
			newKey.local_joints_R.resize(keyRef.transformCount);
			newKey.local_joints_S.resize(keyRef.transformCount);
			for (int t = 0; t < keyRef.transformCount; t++)
			{
				Transform& ref = inLoader->GetTransform(index, a, k, t);
				glm::vec3 newT = glm::make_vec3(ref.transform);
				glm::quat newR = glm::make_vec4(ref.rotate);
				glm::vec3 newS = glm::make_vec3(ref.scale);
				newKey.local_joints_T[t] = newT;
				newKey.local_joints_R[t] = newR;
				newKey.local_joints_S[t] = newS;
			}
			newAni.keyframes[k] = newKey;
		}
		skeleton.animations[a] = newAni;
	}

	skeleton.currentAnimTime = 0;


	ImportMesh(inLoader->GetVerticies(index), inLoader->GetVertexCount(index));
}

Mesh::Mesh()
{
	this->position = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotationEulerXYZ = glm::vec3(0.0f, 0.0f, 0.0f);
	this->rotation = glm::quat(rotationEulerXYZ);
	this->scale = glm::vec3(1.0f);

	isChild = false;
	myGroupParent = nullptr;
	myParent = nullptr;

	this->materialID = 0;
}

Mesh::~Mesh()
{
	/*if (myParent)
		delete myParent;
	if (myGroupParent)
		delete myGroupParent;*/
}

//============================================================= 
//	Creates a hardcoded cube primitive and binds it into a VAO buffer 
//============================================================= 
void Mesh::CreateCubeData()
{
	// 36 hardcoded vertices representing a cube 
	vertexPolygon cubeVertex;

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, 0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);

	vertexCount = (int)vertices.size();
	CalculateTangents();
}

//============================================================= 
//	Creates a hardcoded plane primitive and binds it into a VAO buffer 
//============================================================= 
void Mesh::CreatePlaneData()
{
	//// 6 hardcoded vertices representing a plane 
	vertexPolygon planeVertex;
	planeVertex.position = glm::vec3(-10.0f, 0.0f, -10.0f);
	planeVertex.uv = glm::vec2(0.0f, 5.0f);
	planeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	//planeVertex.tangent = glm::vec3(0.0f); 
	//planeVertex.bitangent = glm::vec3(0.0f); 
	vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(10.0f, 0.0f, -10.0f);
	planeVertex.uv = glm::vec2(5.0f, 5.0f);
	vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(10.0f, 0.0f, 10.0f);
	planeVertex.uv = glm::vec2(5.0f, 0.0f);
	vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(10.0f, 0.0f, 10.0f);
	planeVertex.uv = glm::vec2(5.0f, 0.0f);
	vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(-10.0f, 0.0f, 10.0f);
	planeVertex.uv = glm::vec2(0.0f, 0.0f);
	vertices.push_back(planeVertex);

	planeVertex.position = glm::vec3(-10.0f, 0.0f, -10.0f);
	planeVertex.uv = glm::vec2(0.0f, 5.0f);
	vertices.push_back(planeVertex);

	vertexCount = (int)vertices.size();
	CalculateTangents();
}

void Mesh::CreatePlateData()
{
	// 36 hardcoded vertices representing a cube 
	vertexPolygon cubeVertex;

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, -1.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 0.0f, 1.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(-1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(1.0f, 0.0f, 0.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.5f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, -1.0f, 0.0f);
	vertices.push_back(cubeVertex);

	cubeVertex.position = glm::vec3(-0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(1.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.3f, 0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 0.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);
	cubeVertex.position = glm::vec3(-0.5f, -0.3f, -0.5f);
	cubeVertex.uv = glm::vec2(0.0f, 1.0f);
	cubeVertex.normals = glm::vec3(0.0f, 1.0f, 0.0f);
	vertices.push_back(cubeVertex);

	vertexCount = (int)vertices.size();
	CalculateTangents();
}

void Mesh::ImportMesh(Vertex* vertArr, int vertexCount)
{

	this->vertexCount = vertexCount;
	for (int i = 0; i < vertexCount; i++)
	{
		Vertex vertexData = vertArr[i];
		vertexPolygon newVertex;
		newVertex.position = glm::vec3(vertexData.position[0], vertexData.position[1], vertexData.position[2]);
		newVertex.uv = glm::vec2(vertexData.uv[0], vertexData.uv[1]);
		newVertex.normals = glm::vec3(vertexData.normal[0], vertexData.normal[1], vertexData.normal[2]);
		newVertex.tangent = glm::vec3(vertexData.tangent[0], vertexData.tangent[1], vertexData.tangent[2]);
		newVertex.bitangent = glm::vec3(vertexData.bitangent[0], vertexData.bitangent[1], vertexData.bitangent[2]);

		newVertex.weights = glm::vec4(vertexData.weight[0], vertexData.weight[1], vertexData.weight[2], vertexData.weight[3]);
		newVertex.bones = glm::vec4(vertexData.bone[0], vertexData.bone[1], vertexData.bone[2], vertexData.bone[3]);

		//The reserve should be above the for-loop 
		vertices.reserve(vertexCount);
		vertices.push_back(newVertex);
	}

	//CalculateTangents();

}

void Mesh::CalculateTangents()
{
	// Normal and tangent Calculation 
	glm::vec3 normal = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 tangent = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 biTangent = glm::vec3(0.0f, 0.0f, 0.0f);
	float U1, V1, U2, V2 = 0.0f;

	float vertexVectorX, vertexVectorY, vertexVectorZ = 0.0f;
	glm::vec3 triangleEdge1 = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 triangleEdge2 = glm::vec3(0.0f, 0.0f, 0.0f);

	for (int i = 0; i < vertices.size(); i += 3)
	{
		vertexVectorX = vertices[(i)].position.x - vertices[i + 2].position.x;
		vertexVectorY = vertices[(i)].position.y - vertices[i + 2].position.y;
		vertexVectorZ = vertices[(i)].position.z - vertices[i + 2].position.z;
		triangleEdge1 = glm::vec3(vertexVectorX, vertexVectorY, vertexVectorZ);


		vertexVectorX = vertices[i + 2].position.x - vertices[i + 1].position.x;
		vertexVectorY = vertices[i + 2].position.y - vertices[i + 1].position.y;
		vertexVectorZ = vertices[i + 2].position.z - vertices[i + 1].position.z;
		triangleEdge2 = glm::vec3(vertexVectorX, vertexVectorY, vertexVectorZ);

		// Gets the edges of the triangle and calculates the normal 
		normal = glm::cross(triangleEdge1, triangleEdge2);
		normal = normalize(normal);

		// Get the UV coordinates to calculate a tangent aligned with the UV (using UV and vertex vectors) 
		U1 = vertices[i].uv.x - vertices[i + 2].uv.x;
		V1 = vertices[i].uv.y - vertices[i + 2].uv.y;

		U2 = vertices[i + 2].uv.x - vertices[i + 1].uv.x;
		V2 = vertices[i + 2].uv.y - vertices[i + 1].uv.y;

		tangent.x = (U1 * triangleEdge1.x - V2 * triangleEdge2.x) * (1.0f / (U1 * V2 - U2 * V1));
		tangent.y = (U1 * triangleEdge1.y - V2 * triangleEdge2.y) * (1.0f / (U1 * V2 - U2 * V1));
		tangent.z = (U1 * triangleEdge1.z - V2 * triangleEdge2.z) * (1.0f / (U1 * V2 - U2 * V1));

		tangent = glm::normalize(tangent);

		//vertices[i].normals = normal;
		//vertices[i + 1].normals = normal;
		//vertices[i + 2].normals = normal;

		vertices[i].tangent = tangent;
		vertices[i + 1].tangent = tangent;
		vertices[i + 2].tangent = tangent;

		biTangent = glm::cross(vertices[i].normals, tangent);
		vertices[i].bitangent = biTangent;
		vertices[i + 1].bitangent = biTangent;
		vertices[i + 2].bitangent = biTangent;

	}

}

void Mesh::setMaterial(unsigned int id)
{
	this->materialID = id;
}

void Mesh::SetPosition(glm::vec3 newPos)
{
	position = newPos;
}

void Mesh::SetPosition(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
}

void Mesh::SetPositionX(float x)
{
	position.x = x;
}

void Mesh::SetPositionY(float y)
{
	position.y = y;
}

void Mesh::SetPositionZ(float z)
{
	position.z = z;
}

void Mesh::SetRotation(glm::quat newRot)
{
	rotation = newRot;
}

void Mesh::SetScale(glm::vec3 newSca)
{
	scale = newSca;
}

void Mesh::SetScale(float x, float y, float z)
{
	scale = glm::vec3(x, y, z);
}

void Mesh::ForwardTime(float t)
{
	skeleton.currentAnimTime += t;
}

void Mesh::SetTime(float t)
{
	skeleton.currentAnimTime = t;
}

std::vector<vertexPolygon>& Mesh::ModifyVertices()
{
	return vertices;
}

void Mesh::SetMeshParent(Mesh * parent)
{
	this->myParent = parent;
}

void Mesh::SetGroupParent(MeshGroupClass * parent)
{
	myGroupParent = parent;
}

