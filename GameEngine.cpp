#include "GameEngine.h"

void GameEngine::roomSwitched()
{
	meshCount = mainScene.GetMeshData().size();
	vertexCount = 0;
	for (int i = 0; i < meshCount; i++)
	{
		vertexCount += mainScene.GetMeshData()[i].GetVertices().size();
	}
	// Allocated memory
	mainSceneVertexData = new vertexPolygon[vertexCount];

	int vertexIndex = 0;
	for (int i = 0; i < meshCount; i++)
	{
		int meshVtxCount = mainScene.GetMeshData()[i].GetVertices().size();
		for (int j = 0; j < meshVtxCount; j++)
		{
			mainSceneVertexData[vertexIndex] = mainScene.GetMeshData()[i].GetVertices()[j];
			vertexIndex++;
		}
	}
	mainRenderer.CompileVertexData(vertexCount, mainSceneVertexData);

	mainScene.SetIsSwitched(false);
}

GameEngine::GameEngine()
{
	// Load vertex data for the main scene
	// Could possibly be handled inside the scene or inside the renderer
	// Not dynamic (in constructor) but creates one large render buffer rather than
	// relying on the mesh to have these. This buffer goes into the renderer and would be swapped between rooms.
	// The scene handles what data this recieves
	meshCount = mainScene.GetMeshData().size();
	vertexCount = 0;
	for (int i = 0; i < meshCount; i++)
	{
		vertexCount += mainScene.GetMeshData()[i].GetVertices().size();
	}
	// Allocated memory
	mainSceneVertexData = new vertexPolygon[vertexCount];

	int vertexIndex = 0;
	for (int i = 0; i < meshCount; i++)
	{
		int meshVtxCount = mainScene.GetMeshData()[i].GetVertices().size();
		for (int j = 0; j < meshVtxCount; j++)
		{
			mainSceneVertexData[vertexIndex] = mainScene.GetMeshData()[i].GetVertices()[j];
			vertexIndex++;
		}
	}
	mainRenderer.CompileVertexData(vertexCount, mainSceneVertexData);
}

GameEngine::~GameEngine()
{
	if (mainSceneVertexData)
		delete mainSceneVertexData;
}

void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
	//	if (mainScene.getState() == enum STATE PAUSED) {
	//		mainScene.setState(enum STATE PLAYING);
	//	}
	//	else mainScene.setState(enum STATE PAUSED);
	//}
	
}

//=============================================================
//	Main engine loop
//=============================================================
void GameEngine::Run()
{
	// Keyboard callback reference, should be changed when if keyboard callbacks are added
	glfwSetKeyCallback(mainRenderer.getWindow(), keyboard);
	glfwSwapInterval(0);

	// If this becomes true the program will have failed in someway or been manually shut down
	bool shutdown = false;

	// Load ImGui content
	static bool renderDepth = false;
	ImGuiInit();

	// Framebuffer for the main renderer
	if (mainRenderer.CreateFrameBuffer() != 0)
		shutdown = true;

	while (!glfwWindowShouldClose(mainRenderer.getWindow()))
	{
		glfwPollEvents();
		//if (GLFW_PRESS == glfwGetKey(mainRenderer.getWindow(), GLFW_KEY_ESCAPE))
		//{
		//	glfwSetWindowShouldClose(mainRenderer.getWindow(), 1);
		//}

		// Deltatime via ImGui
		float deltaTime = ImGui::GetIO().DeltaTime;

		// Main updates to a scene
		// Includes all interactions in the game world
		mainScene.Update(mainRenderer.getWindow(), deltaTime);

		// PrePass render for Shadow mapping 
		mainRenderer.prePassRender(mainScene.GetShader(1), mainScene.GetMeshData(), mainScene.GetCamera(), gClearColour, mainScene.GetDirectionalLights());
		mainRenderer.SetViewport();	//resets the viewport

		// First render pass
		mainRenderer.firstPassRenderTemp(mainScene.GetShader(2), mainScene.GetMeshData(), gClearColour);

		// Update ImGui content
		UpdateImGui(renderDepth);

		// ---- Main render call --- ///
		mainRenderer.Render(mainScene.GetShader(0), 
			mainScene.GetMeshData(), 
			mainScene.GetCamera(), 
			gClearColour, 
			mainScene.GetPointLights(), 
			mainScene.GetDirectionalLights(), 
			mainScene.GetMaterials());

		// Render a textured full screen quad if game is paused (DOES NOT WORK PROPERLY RN AND TEXTURE HAS MEMORY LEAKS)
		if (mainScene.GetCurrentState() == PAUSED) {
			mainRenderer.secondPassRenderPauseOverlay(mainScene.GetShader(2), mainMenu.GetPauseOverlay());
		}
		else {
			// Render a second pass for the fullscreen quad
			mainRenderer.secondPassRenderTemp(mainScene.GetShader(2));
		}
	
		// Draw fullscreen quad
		// Could be moved to the renderer
		glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Render ImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (mainScene.GetIsSwitched())
		{
			roomSwitched();
		}

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
