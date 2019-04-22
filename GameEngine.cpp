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

	// Load ImGui
	static bool renderDepth = false;
	ImGuiInit();

	// Load and initialize game content
	LoadContent();

	// Framebuffer for the main renderer
	if (mainRenderer.CreateFrameBuffer() != 0)
		shutdown = true;

	// Creates the frame buffer for shadow mapping
	if (shadowMap.CreateFrameBufferSM() != 0)
		shutdown = true;

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
		// Will be moved to the scene updates
		updateContent(deltaTime);

		//---------
		//PrePass render for Shadow mapping 
		shadowMap.bindForWriting();
		mainRenderer.prePassRender(shadowmapShader, objects, mainCamera, gClearColour, shadowMap, aDirLight);
		mainRenderer.SetViewport();	//resets the viewport
		//--------

		// First render pass
		mainRenderer.firstPassRenderTemp(fsqShader, objects, gClearColour);

		// Update ImGui content
		UpdateImGui(renderDepth);

		// ---- Main render call --- ///
		// Currently takes in additional ImGui content that should be looked over
		mainRenderer.Render(basicShader, objects, mainCamera, gClearColour, shadowMap, lights, aDirLight, materials);

		// Render a second pass for the fullscreen quad
		mainRenderer.secondPassRenderTemp(fsqShader, shadowMap);
	

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

void GameEngine::ImGuiInit()
{
	// ImGui initialization, should be moved to keep ImGui systems together
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui_ImplGlfw_InitForOpenGL(mainRenderer.getWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 130");
	ImGui::StyleColorsDark();
}

void GameEngine::UpdateImGui(bool &renderDepth)
{
	// Modifiable variables, currently they do nothing
	static float gRotate2Z = 0;
	static float gTx[2]{ 0, 0 };
	static float scale = 1.0f;

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Debug window");								// Create a window called "Hello, world!" and append into it.
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("This is some useful text.");					// Display some text (you can use a format strings too)
	ImGui::SliderFloat("float", &gFloat, 0.0f, 1.0f);			// Edit 1 float using a slider from 0.0f to 1.0f    
	ImGui::ColorEdit3("Clear color", gClearColour);				// Edit 3 floats representing a color
	ImGui::SliderAngle("RotateFrame", &gRotate2Z);
	ImGui::DragFloat2("Translate", gTx, 0.1f, -0.5f, 0.5f);
	ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
	ImGui::Checkbox("Show DepthMap", &renderDepth);
	ImGui::End();
}

//=============================================================
//	Updates engine content here
//=============================================================
void GameEngine::updateContent(float deltaTime)
{
	// Updates camera position (movement)
	mainCamera.FPSCamControls(mainRenderer.getWindow(), deltaTime);

	// Check a potential new position
	glm::vec3 newPos = playerCharacter.Move(mainRenderer.getWindow(), deltaTime);

	// Check new positions collision	
	bool collision = false;
	int dominatingBox = -1;
	for (int i = 0; i < entities.size(); ++i)
	{
		if (playerCharacter.CheckCollision(entities[i]))
		{
			collision = true;
			// Reset player position (new position is inside a collision this the character has to be moved back again)
			glm::vec3 pushDir = entities[i].getPosition() - newPos;
			if (abs(pushDir.x) >= abs(pushDir.z))
				pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
			else
				pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);

			pushDir = glm::normalize(pushDir);
			pushDir *= 0.15f;
			entities[i].setPosition(entities[i].getPosition() + pushDir);
			dominatingBox = i;
		}
	}

	// Could possibly be done with recursion to check subsequent collisions
	// Could be made better with proper physic calculations
	for (int i = 0; i < entities.size(); ++i)
	{
		for (int j = 0; j < entities.size(); ++j)
		{
			if (i != j && entities[i].CheckCollision(entities[j]) && j != dominatingBox)
			{
				glm::vec3 pushDir = entities[j].getPosition() - entities[i].getPosition();
				if (abs(pushDir.x) >= abs(pushDir.z))
					pushDir = glm::vec3(pushDir.x, 0.0f, 0.0f);
				else
					pushDir = glm::vec3(0.0f, 0.0f, pushDir.z);
				pushDir = glm::normalize(pushDir);
				pushDir *= 0.15f;
				entities[j].setPosition(entities[j].getPosition() + pushDir);
			}
		}
	}

	if (!collision)
	{
		playerCharacter.setPosition(newPos);
		collision = false;
	}

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[0].CheckCollision(entities[i]))
		{
			cout << "Solved" << endl;  
		}
	}

	// Update all render mesh data
	objects.clear();

	objects.push_back(groundPlane);
	for (int i = 0; i < entities.size(); i++)
		objects.push_back(entities[i].getMeshData());

	for (int i = 0; i < nodes.size(); i++)
		objects.push_back(nodes[i].getMeshData());

	// Update playermesh in the renderer
	objects.push_back(playerCharacter.getMeshData());
}

//=============================================================
//	Load engine content here
//=============================================================
void GameEngine::LoadContent()
{
	// Load shaders
	basicShader.CreateShader("VertexShader.glsl", "Fragment.glsl");
	shadowmapShader.CreateShader("VertexShaderSM.glsl", "FragmentSM.glsl");

	// Initialize textures
	Material planeMat("Plane Material", 0);
	planeMat.createAlbedo("Resources/Textures/brickwall.jpg");
	planeMat.createNormal("Resources/Textures/brickwall_normal.jpg");
	materials.push_back(planeMat);

	Material cubeMat("Plane Cube Material", 1);
	cubeMat.createAlbedo("Resources/Textures/boxTexture.png");
	materials.push_back(cubeMat);

	Material playerMat("Player Material", 2);
	playerMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(playerMat);

	Material nodeMat("Node Material", 3);
	nodeMat.createAlbedo("Resources/Textures/broken.png");
	materials.push_back(nodeMat);

	// Initialize lights
	Light light;
	light.setDiffuse(glm::vec3(1.0f, 0.3f, 0.5f));
	light.setSpecular(glm::vec3(1.0f, 0.3f, 0.5f));

	light.setLightPos(glm::vec3(3.0f, 1.0f, -3.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 2.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(3.0f, 1.0f, 7.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, -3.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 2.0f));
	lights.push_back(light);

	light.setLightPos(glm::vec3(-3.0f, 1.0f, 7.0f));
	lights.push_back(light);

	// Initialize fullscreen quad vertices
	// Right now the fullscreen quad is coded into the shader handler.
	// Could be moved and better organized
	fsqShader.CreateFSShaders();
	fsqShader.CreateFullScreenQuad();

	// Initialize plane (ground)
	groundPlane.CreatePlaneData();
	groundPlane.setPosition(glm::vec3(0.0f, -0.5f, 0.0f));
	groundPlane.setMaterial(materials[0].getMaterialID());
	objects.push_back(groundPlane);

	// Initialize Entities
	Entity cubeEntity;
	cubeEntity.setMaterialID(materials[0].getMaterialID());

	cubeEntity.setPosition(glm::vec3(3.0f, 0.0f, -3.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(3.0f, 0.0f, 2.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(3.0f, 0.0f, 7.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(-3.0f, 0.0f, -3.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(-3.0f, 0.0f, 2.0f));
	entities.push_back(cubeEntity);

	cubeEntity.setPosition(glm::vec3(-3.0f, 0.0f, 7.0f));
	entities.push_back(cubeEntity);

	puzzleNode winNode;
	winNode.setMaterialID(materials[3].getMaterialID());

	winNode.setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
	nodes.push_back(winNode);

	for (int i = 0; i < entities.size(); i++)
	{
		objects.push_back(entities[i].getMeshData());
	}

	playerCharacter.setMaterialID(2);
	objects.push_back(playerCharacter.getMeshData());

	for (int i = 0; i < nodes.size(); i++)
	{
		objects.push_back(nodes[i].getMeshData());
	}
	// ^^^^ Additional render objects should be placed above ^^^^ //
}
