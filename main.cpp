//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//     - updated by FLL
//--------------------------------------------------------------------------------------
//#include <windows.h>
//
//#include <string>
//#include <fstream>
//#include <streambuf>
//
//#define IMGUI_IMPL_OPENGL_LOADER_GLEW
//
//#include "imgui/imgui.h"
//#include "imgui/imgui_impl_glfw.h"
//#include "imgui/imgui_impl_opengl3.h"
//
//#include "glew/include/GL/glew.h"
//#include "glfw/include/GLFW/glfw3.h"
//
//// read docs: https://github.com/g-truc/glm/blob/master/manual.md#section1
//#include "glm/glm.hpp"
//#include "glm/ext.hpp"
//#include <gl/GL.h>
//
//#pragma comment(lib, "opengl32.lib")
//#pragma comment(lib, "glew32.lib")
//#pragma comment(lib, "glfw3.lib")
//
//#define WIDTH 1280
//#define HEIGHT 720
//
//using namespace std;

#include "Headers.h"
#include "Renderer.h"
#include "ShaderHandler.h"
#include "SimpleShape.h"

// OpenGL uses unsigned integers to keep track of
// created resources (shaders, vertices, textures, etc)
// For simplicity, we make them global here, but it is
// safe to put them in a class and pass around...
GLuint gVertexBuffer = 0;
GLuint gVertexAttribute = 0;
GLuint gShaderProgram = 0;

// full screen quad stuff
GLuint gVertexBufferFS = 0;
GLuint gVertexAttributeFS = 0;
GLuint gShaderProgramFS = 0;
float gTx, gTy;

float gFloat = 0;

float gClearColour[3] {};
float gUniformColour[3] {};
GLint gUniformColourLoc = -1;

float gOffsetX = 0.0f;
float gIncrement = 0.0f;
float gRotateZ = 0.0f;

glm::mat4 gRotate2D;
// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

unsigned int gFbo;
unsigned int gFboTextureAttachments[2]; // first for colour, second for depth
int CreateFrameBuffer() {
	int err = 0;
	// =================== COLOUR BUFFER =======================================
	// add "Attachments" to the framebuffer (textures to write to/read from)
	glGenTextures(2, gFboTextureAttachments);
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[0]);
	// define storage for texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// define sampler settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// attach texture to framebuffer object

	// ===================== DEPTH BUFFER ====================================
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &gFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gFboTextureAttachments[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gFboTextureAttachments[1], 0);
	
	// check if framebuffer is complete (usable):
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		err = 0; 
	}
	else
		err = -1;

	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return err;
}

void CreateFullScreenQuad()
{
	struct Pos2UV {
		float x,y;
		float u,v;
	};
	Pos2UV myQuad[6] = {
		-1,-1, 0, 0,	// TOP		LEFT
		-1,+1, 0, 1,	// BOTTOM	LEFT
		+1,+1, 1, 1,	// BOTTOM	RIGHT
		-1,-1, 0, 0,	// TOP		LEFT
		+1,+1, 1, 1,	// BOTTOM	RIGHT
		+1,-1, 1, 0,	// TOP		RIGHT
	};

	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttributeFS);
	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttributeFS);
	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBufferFS);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferFS);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, sizeof(myQuad), myQuad, GL_STATIC_DRAW);

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Pos2UV), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Pos2UV), BUFFER_OFFSET(sizeof(float)*2));
};

void CreateTriangleData()
{
	// this is how we will structure the input data for the vertex shader
	// every six floats, is one vertex.
	struct TriangleVertex
	{
		float x, y, z;
		float r, g, b;
		float myAttr;
	};

	// create the actual data in plane Z = 0
	// This is called an Array of Structs (AoS) because we will 
	// end up with an array of many of these structs.
	TriangleVertex triangleVertices[3] = 
	{
		// pos and color for each vertex
		{ 0.0f, 0.5f, 0.3f,	1.0f, 0.0f, 0.0f, 0.0},
		{ 0.5f, -0.5f, 0.3f, 0.0f, 1.0f, 0.0f, 0.0},
		{ -0.5f, -0.5f, 0.3f, 0.0f, 0.0f, 1.0f, 0.0}
	};

	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);
	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);
	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	// query which "slot" corresponds to the input vertex_position in the Vertex Shader 
	GLint vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");
	// if this returns -1, it means there is a problem, and the program will likely crash.
	// examples: the name is different or missing in the shader

	if (vertexPos == -1) {
		OutputDebugStringA("Error, cannot find 'vertex_position' attribute in Vertex shader\n");
		return;
	}

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		vertexPos,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(TriangleVertex), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);

	// repeat the process for the second attribute.
	// query which "slot" corresponds to the input vertex_color in the Vertex Shader 
	GLuint vertexColor = glGetAttribLocation(gShaderProgram, "vertex_color");
	glVertexAttribPointer(
		vertexColor, 
		3, 
		GL_FLOAT, 
		GL_FALSE, sizeof(TriangleVertex), // distance between two vertexColor 
		BUFFER_OFFSET(sizeof(float)*3)	// note, the first color starts after the first vertex.
	);

	GLint myAttrLoc = glGetAttribLocation(gShaderProgram, "myAttr");
	glVertexAttribPointer(myAttrLoc, 1, GL_FLOAT, GL_FALSE, sizeof(TriangleVertex), BUFFER_OFFSET(sizeof(float)*6));
}

void SetViewport()
{
	// usually (not necessarily) this matches with the window size
	glViewport(0, 0, WIDTH, HEIGHT);
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	if (key == GLFW_KEY_W)
		keys[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_A)
		keys[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_S)
		keys[2] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	if (key == GLFW_KEY_D)
		keys[3] = (action == GLFW_PRESS || action == GLFW_REPEAT);
	*/
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	// Main window and renderer object
	Renderer mainRenderer;
	ShaderHandler shaderHandler;
	SimpleShape triangle;

	glfwSetKeyCallback(mainRenderer.getWindow(), keyboard);
	bool shutdown = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(mainRenderer.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();

	shaderHandler.CreateShaders(&gShaderProgram);
	shaderHandler.CreateFSShaders(&gShaderProgramFS);

	if (CreateFrameBuffer() != 0)
		shutdown = true;

	CreateTriangleData(); //6. Definiera triangelvertiser, 7. Skapa vertex buffer object (VBO), 8.Skapa vertex array object (VAO)
	triangle.CreateTriangleData(gShaderProgram, gVertexAttribute, gVertexBuffer);
	CreateFullScreenQuad();

	gUniformColourLoc = glGetUniformLocation(gShaderProgram, "colourFromImGui");
	while (!glfwWindowShouldClose(mainRenderer.getWindow()))
	{
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(mainRenderer.getWindow(), GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(mainRenderer.getWindow(), 1);
		}

		// first pass
		// render all geometry to a framebuffer object
		glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
		glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], gClearColour[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(gShaderProgram);
		glBindVertexArray(gVertexAttribute);
		glEnable(GL_DEPTH_TEST);

		float deltaTime = ImGui::GetIO().DeltaTime;
		// move along X
		gIncrement += 1.0f * deltaTime;
		gOffsetX = sin(gIncrement);
		glUniform1f(10, gOffsetX);

		// prepare IMGUI output
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::SliderFloat("float", &gFloat, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", gClearColour); // Edit 3 floats representing a color
		ImGui::ColorEdit3("triangle color", gUniformColour);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::SliderAngle("RotateZ", &gRotateZ);
		static float gRotate2Z = 0;
		ImGui::SliderAngle("RotateFrame", &gRotate2Z);
		static float gTx[2] = {0,0};
		ImGui::DragFloat2("Translate", gTx, 0.1f, -0.5f, 0.5f);
		static float scale = 1.0f;
		ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
		static bool renderDepth = false;
		ImGui::Checkbox("Show DepthMap", &renderDepth);
		ImGui::End();

		const glm::mat4 identity = glm::mat4(1.0f);
		//gRotate2D = identity;
		gRotate2D = glm::rotate(identity, gRotateZ, glm::vec3(0.0f,0.0f,1.0f));
		glUniformMatrix4fv(11, 1, GL_TRUE, &gRotate2D[0][0]);
		//glm::value_ptr(gRotate2D));

		// Render vertexbuffer at gVertexAttribute in gShaderProgram
		mainRenderer.Render(gShaderProgram, gVertexAttribute, gClearColour, gUniformColour, gUniformColourLoc);

		// first pass is done!
		// now render a second pass
		// bind default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(gShaderProgramFS);
		glBindVertexArray(gVertexAttributeFS);

		glDisable(GL_DEPTH_TEST);

		// bind texture drawn in the first pass!
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[0]);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[1]);
		
		glm::mat4 translate = glm::translate(identity, glm::vec3(gTx[0], gTx[1], 0.0f));
		glm::mat4 rotation = glm::rotate(identity, gRotate2Z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMat = glm::scale(identity, glm::vec3(scale, scale, scale));
		glm::mat4 transform = translate * rotation * scaleMat;
		glUniformMatrix4fv(5, 1, GL_TRUE, &transform[0][0]);

		// false
		glUniform1i(3, renderDepth);  // 0 == false
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(mainRenderer.getWindow());
	}

	// SHUTDOWN
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glDeleteFramebuffers(1, &gFbo);
	glDeleteTextures(2, gFboTextureAttachments);
	glDeleteVertexArrays(1, &gVertexAttribute);
	glDeleteVertexArrays(1, &gVertexAttributeFS);
	glDeleteBuffers(1, &gVertexBuffer);
	glDeleteBuffers(1, &gVertexBufferFS);
	glfwTerminate();

	return 0;

}

