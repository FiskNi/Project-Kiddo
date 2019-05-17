#pragma once
#include "Headers.h"
#include "Mesh.h"
#include "Camera.h"
#include "ShadowMap.h"
#include "Shader.h"
#include "Light.h"
#include "DirectionalLight.h"
#include "Material.h"
#include "Menu.h"

//============================================================================
//	- Renderer
//	The renderer picks up a vector list of elements for renderering from the GameEngine class.
//	The GameEngine class transfers this list from the scene class, which in turns compiles its list
//	from all the scene elements and the elements in each room.
//	The renderer compiles one buffer from all the vertex data recieved from the gameengine - scene - room
//
//	- Usage:
//	Any specific render techniques can be added and handled here.
//	Swapping of uniforms and shader inputs are all handled here as expected.
//	Currently the window is created and handled by the renderer, this could be extracted into it's own class.															
//	
//============================================================================

struct SkinDataBuffer
{
	glm::mat4 bones[64];
};

class Renderer
{
private:
	// Window //*****// There should be a window class instead
	GLFWwindow *gWindow;

	// Shadowmap
	ShadowMap shadowMap;

	unsigned int gFbo;
	// Color and depth Render-to-texture for the fullscreen quad
	unsigned int gFboTextureAttachments[2];

	glm::mat4 MODEL_MAT;
	glm::mat4 VIEW_MAT;
	glm::mat4 PROJ_MAT;

	GLuint gVertexBuffer;
	GLuint gVertexAttribute;

	GLuint gVertexBufferMenu;
	GLuint gVertexAttributeMenu;

	GLuint boneBuffer;


public:
	Renderer();
	~Renderer();

	GLFWwindow *getWindow();

	void firstPassRenderTemp(Shader gShaderProgram, std::vector<Mesh> objects, float gClearColour[]);
	void secondPassRenderTemp(Shader gShaderProgram);
	void secondPassRenderPauseOverlay(Shader gShaderProgram, GLuint pauseOverlayTexture);

	void prePassRender(Shader gShaderProgram, 
		std::vector<Mesh> objects, 
		Camera camera, 
		float gClearColour[3], 
		std::vector<DirectionalLight> dirLightArr);

	void Render(Shader gShaderProgram, 
		std::vector<Mesh> objects, 
		Camera camera, float gClearColour[3], 
		std::vector<Light> lightArr, 
		std::vector<DirectionalLight> dirLightArr, 
		std::vector<Material> materials);

	void CompileVertexData(int vertexCount, vertexPolygon* vertices);

	void RenderMainMenu(Shader gShaderProgram, std::vector<MenuButton> objects, float gClearColour[3], GLuint bgTexture, std::vector<GLuint> textures);
	void CompileMenuVertexData(int vertexCount, ButtonVtx * vertices);

	int CreateFrameBuffer();
	void initWindow(unsigned int w, unsigned int h);
	void SetViewport();

	void CreateModelMatrix(glm::vec3 translation, glm::quat rotation, glm::vec3 scale, GLuint shaderProg);
	void ComputeAnimationMatrix(SkinDataBuffer* boneList, float anim_time, Mesh* mesh);

	void passTextureData(GLuint TextureUnit, GLuint texID, GLuint shaderProg, GLchar* uniformName, int index);


};

