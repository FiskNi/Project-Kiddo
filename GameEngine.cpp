#include "GameEngine.h"



GameEngine::GameEngine()
{
}

GameEngine::~GameEngine()
{
}

void GameEngine::Run()
{
	//glfwSetKeyCallback(mainRenderer.getWindow(), keyboard);
	bool shutdown = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(mainRenderer.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();

	// Create Shaders
	ShaderHandler basicShader;
	basicShader.CreateShaders("VertexShader.glsl", "Fragment.glsl");
	ShaderHandler fsqShader;
	shaderHandler.CreateFSShaders(&gShaderProgramFS);


	if (CreateFrameBuffer() != 0)
		shutdown = true;

	// Create primitive
	CreatePrimitive trianglePrimitive;
	CreatePrimitive trianglePrimitive2;

	std::vector<CreatePrimitive> objects;

	trianglePrimitive.CreateTriangleData(basicShader.getShader(), -0.5f);
	trianglePrimitive2.CreateTriangleData(basicShader.getShader(), 0.3f);
	objects.push_back(trianglePrimitive);
	objects.push_back(trianglePrimitive2);

	CreateFullScreenQuad();

	gUniformColourLoc = glGetUniformLocation(basicShader.getShader(), "colourFromImGui");
	while (!glfwWindowShouldClose(mainRenderer.getWindow()))
	{
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(mainRenderer.getWindow(), GLFW_KEY_ESCAPE)) 
		{
			glfwSetWindowShouldClose(mainRenderer.getWindow(), 1);
		}

		// First render pass
		firstPassRenderTemp(basicShader.getShader(), trianglePrimitive.getVertexAttribute());

		// Load imGui content	
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
		static float gTx[2]{ 0, 0 };
		ImGui::DragFloat2("Translate", gTx, 0.1f, -0.5f, 0.5f);
		static float scale = 1.0f;
		ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
		static bool renderDepth = false;
		ImGui::Checkbox("Show DepthMap", &renderDepth);
		ImGui::End();

		//ImGuis uniform buffer for rotating all vertices in the VertexShader
		glm::mat4 identity = glm::mat4(1.0f);
		//gRotate2D = identity;
		gRotate2D = glm::rotate(identity, gRotateZ, glm::vec3(0.0f, 0.0f, 1.0f));
		glUniformMatrix4fv(11, 1, GL_TRUE, &gRotate2D[0][0]);
		//glm::value_ptr(gRotate2D));

		// Render vertexbuffer at gVertexAttribute in gShaderProgram
		mainRenderer.Render(basicShader.getShader(), objects, gClearColour, gUniformColour, gUniformColourLoc);

		// Render a second pass (temporary)
		secondPassRenderTemp();

		// Prepares matrices for usage with imGui
		glm::mat4 translate = glm::translate(identity, glm::vec3(gTx[0], gTx[1], 0.0f));
		glm::mat4 rotation = glm::rotate(identity, gRotate2Z, glm::vec3(0.0f, 0.0f, 1.0f));
		glm::mat4 scaleMat = glm::scale(identity, glm::vec3(scale, scale, scale));
		glm::mat4 transform = translate * rotation * scaleMat;

		glUniformMatrix4fv(5, 1, GL_TRUE, &transform[0][0]);
		// Draw fullscreen quad
		glUniform1i(3, renderDepth);  // 0 == false
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Render ImGui
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
}

void GameEngine::firstPassRenderTemp(GLuint gShaderProgram, GLuint gVertexAttribute)
{

	// first pass
	// render all geometry to a framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], gClearColour[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgram);
	glBindVertexArray(gVertexAttribute);
	glEnable(GL_DEPTH_TEST);
}

void GameEngine::secondPassRenderTemp()
{
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
}

int GameEngine::CreateFrameBuffer() {
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
		err = 0;
	else
		err = -1;

	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return err;
}

void GameEngine::CreateFullScreenQuad()
{
	struct Pos2UV {
		float x, y;
		float u, v;
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Pos2UV), BUFFER_OFFSET(sizeof(float) * 2));
};

//static void GameEngine::keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
//{
//	/*
//	if (key == GLFW_KEY_W)
//		keys[0] = (action == GLFW_PRESS || action == GLFW_REPEAT);
//	if (key == GLFW_KEY_A)
//		keys[1] = (action == GLFW_PRESS || action == GLFW_REPEAT);
//	if (key == GLFW_KEY_S)
//		keys[2] = (action == GLFW_PRESS || action == GLFW_REPEAT);
//	if (key == GLFW_KEY_D)
//		keys[3] = (action == GLFW_PRESS || action == GLFW_REPEAT);
//	*/
//}
