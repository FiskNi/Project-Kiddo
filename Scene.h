#pragma once
#include "Renderer.h"
#include "ShadowMap.h"
#include "Primitive.h"
#include "Shader.h"
#include "Camera.h"
#include "Character.h"
#include "Light.h"
#include "Material.h"
#include "Entity.h"
#include "DirLight.h"
#include "puzzleNode.h"

class Scene
{
public:
	Scene();
	~Scene();


private:
	// Shaders
	Shader basicShader;
	Shader fsqShader;
	Shader gShaderSM;

	// Materials are stored in a vector
	std::vector<Material> materials;

	// Lights are stored in a vector
	std::vector<Light> lights;
};

