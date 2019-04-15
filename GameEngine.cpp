#include "GameEngine.h"

GameEngine::GameEngine()
{

}

GameEngine::~GameEngine()
{
}

static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	cout << key << endl;

	if (key == GLFW_KEY_SPACE)
	{
		switch (action)
		{
		case GLFW_PRESS:
			cout << "Space key is pressed" << endl;
		break;
		case GLFW_REPEAT:
			cout << "Space is held down" << endl;
			break;
		case GLFW_RELEASE:
			cout << "Space is released" << endl;
			break;
		default:
			break;
		}
	}
	
}

//=============================================================
//	Main engine loop
//=============================================================
void GameEngine::Run()
{
	// Keyboard callback reference, should be changed when if keyboard callbacks are added
	glfwSetKeyCallback(mainRenderer.getWindow(), keyboard);

	// If this becomes true the program will have failed in someway or been manually shut down
	bool shutdown = false;

	// ImGui initialization, should be moved to keep ImGui systems together
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(mainRenderer.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();

	// Load and initialize game content
	LoadContent();

	// Should be moved to class privates
	DirLight aDirLight;
	Camera newCam;
	Light lightArr[nr_P_LIGHTS];
	Light newLight;
	Light twoLight;
	Light threeLight;

	twoLight.setLightPos(glm::vec3(4, 1, 0));
	threeLight.setLightPos(glm::vec3(-4, 1, 0));

	newLight.setPower(1.0);
	twoLight.setPower(1.0);
	threeLight.setPower(1.0);

	lightArr[0] = newLight;
	lightArr[1] = twoLight;
	lightArr[2] = threeLight;

	// Framebuffer for the main renderer
	if (mainRenderer.CreateFrameBuffer() != 0)
		shutdown = true;

	// Creates the frame buffer for shadow mapping
	if (shadowMap.CreateFrameBufferSM() != 0)
		shutdown = true;

	gUniformColourLoc = glGetUniformLocation(basicShader.getShader(), "colourFromImGui");

	while (!glfwWindowShouldClose(mainRenderer.getWindow()))
	{
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(mainRenderer.getWindow(), GLFW_KEY_ESCAPE)) 
		{
			glfwSetWindowShouldClose(mainRenderer.getWindow(), 1);
		}
		// Deltatime via ImGui
		float deltaTime = ImGui::GetIO().DeltaTime;

		// Main updates to loaded data
		updateContent(deltaTime, newCam, newLight);

		//---------
		//PrePass render for Shadow mapping 
		shadowMap.bindForWriting();
		mainRenderer.prePassRender(gShaderSM, objects, mainCamera, gClearColour, gUniformColour, gUniformColourLoc, shadowMap);
		mainRenderer.SetViewport();	//resets the viewport
		//--------

		// First render pass
		mainRenderer.firstPassRenderTemp(fsqShader, objects, gClearColour);

		// ---- Below is ImGui content that should be looked over and organized better	
		// Load imGui content	
		//deltaTime = ImGui::GetIO().DeltaTime;
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
		ImGui::SliderFloat("float", &gFloat, 0.0f, 1.0f);       // Edit 1 float using a slider from 0.0f to 1.0f    
		ImGui::ColorEdit3("clear color", gClearColour);			// Edit 3 floats representing a color
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
		// ---- Above is ImGui content that should be looked over and organized better	



		// ---- Main render call --- ///
		// Currently takes in additional ImGui content that should be looked over
		mainRenderer.SetViewport();
		mainRenderer.Render(basicShader, objects, mainCamera, gClearColour, gUniformColour, gUniformColourLoc, shadowMap, lightArr, aDirLight);

		// Render a second pass for the fullscreen quad
		mainRenderer.secondPassRenderTemp(fsqShader);

		// Prepares matrices for usage with imGui, needs to be moved with ImGui stuff
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
	

	// *******************************
	// MEMORY NEEDS TO BE LOOKED OVER! - No memory leaks!
	// *******************************
	glDeleteVertexArrays(1, &gVertexAttributeFS);
	glDeleteBuffers(1, &gVertexBufferFS);
	glfwTerminate();
}

//=============================================================
//	Updates engine content here
//=============================================================
void GameEngine::updateContent(float deltaTime, Camera &newCam, Light &newLight)
{
	// Updates camera position (movement)
	mainCamera.FPSCamControls(mainRenderer.getWindow(), deltaTime);

	// Could be turned into a for-loop
	for (int i = 0; i < Entities.size(); i++)
	{
		objects[entityIndex[i]] = Entities[i].getMeshData();
	}

	// Very basic collision check with movement limiter
	glm::vec3 oldPos = playerCharacter.getPosition();
	glm::vec3 newPos = playerCharacter.Move(mainRenderer.getWindow(), deltaTime);
	playerCharacter.setPosition(newPos);

	for (int i = 0; i < Entities.size(); i++)
	{
		if (playerCharacter.CheckCollision(Entities[i]))
		{
			playerCharacter.setPosition(oldPos);

			newPos = Entities[i].getPosition() - newPos;
			Entities[i].setPosition(Entities[i].getPosition() + newPos);
		}
	}
	objects[playerIndex] = playerCharacter.getMeshData();

	// **** Needs to be moved to the renderer
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(newCam.GetViewMatrix()));
	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(newCam.GetProjectionMatrix()));
	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(15, 1, glm::value_ptr(newLight.getLightPos()));
	glUniform3fv(16, 1, glm::value_ptr(newCam.camPos));
}

//=============================================================
//	Load engine content here
//=============================================================
void GameEngine::LoadContent()
{
	// Load shaders
	basicShader.CreateShader("VertexShader.glsl", "Fragment.glsl");
	gShaderSM.CreateShader("VertexShaderSM.glsl", "FragmentSM.glsl");

	// Load fullscreen quad vertices
	// Right now the fullscreen quad is coded into the shader handler.
	// Could be moved and better organized
	fsqShader.CreateFSShaders();
	fsqShader.CreateFullScreenQuad();

	// Initialize 1 cube primitive and duplicate it by pushing it back into a vector
	// "objects" is currently what can be seen as the renderqueue
	cubePrimitive.CreateCubeData();
	cubePrimitive.setTextureID(cubeMat.createTexture("Resources/Textures/boxTexture.png"));
	objects.push_back(cubePrimitive);
	objects.push_back(cubePrimitive);
	
	// Initialize plane (ground)
	groundPlane.CreatePlaneData();
	groundPlane.setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	groundPlane.setTextureID(planeMat.createTexture("Resources/Textures/mudTexture.jpg"));
	objects.push_back(groundPlane);

	// Entity creations
	Entity cubeEntity;
	glm::vec3 startPos;
	
	startPos = glm::vec3(-4.0f, 0.0f, -3.0f);
	cubeEntity.setPosition(startPos);
	Entities.push_back(cubeEntity);

	startPos = glm::vec3(-4.0f, 0.0f, 3.0f);
	cubeEntity.setPosition(startPos);
	Entities.push_back(cubeEntity);

	startPos = glm::vec3(-2.0f, 0.0f, 3.0f);
	cubeEntity.setPosition(startPos);
	Entities.push_back(cubeEntity);

	for (int i = 0; i < Entities.size(); i++)
	{
		objects.push_back(Entities[i].getMeshData());
		entityIndex[i] = objects.size() - 1;
	}

	objects.push_back(playerCharacter.getMeshData());
	playerIndex = objects.size() - 1;
	// ^^^^ Additional render objects should be placed above ^^^^ //
}


// **** Temporary keyboard callback reference **** ///
// 
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
