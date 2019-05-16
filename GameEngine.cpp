#include "GameEngine.h"



GameEngine::GameEngine()
{

	// Supreme edition loading screen that shows up too late (should play before the scene loads stuff)
	mainRenderer.CreateFrameBuffer();
	mainRenderer.SetViewport();
	//mainRenderer.firstPassRenderTemp(mainScene.GetShader(2), mainScene.GetMeshData(), gClearColour);
	mainRenderer.secondPassRenderPauseOverlay(mainScene.GetShader(2), mainMenu.GetLoadingTexture());
	glUniform1i(3, false);  // Boolean for the shadowmap toggle
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glfwSwapBuffers(mainRenderer.getWindow());


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



	menuIsRunning = true;
}

GameEngine::~GameEngine()
{
	if (mainSceneVertexData)
		delete mainSceneVertexData;
	if (mainMenuVertexData)
		delete mainMenuVertexData;
}

void GameEngine::CompileRoomData()
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
}

void GameEngine::CompileMainMenuData()
{
	//meshCount = mainScene.GetMeshData().size();
	int nrOfMenuButtons = mainMenu.GetNrOfMenuButtons();
	int vtxCountButtons = mainMenu.GetVertexCountTotal();
	//std::cout << vtxCountButtons << std::endl;
	//std::cout << mainMenu.GetVertexCountTotal() << std::endl;
	//for (int i = 0; i < nrOfMenuButtons; i++)
	//{
	//	vertexCount += mainMenu.GetVertexCountTotal();
	//}
	// Allocated memory
	//mainSceneVertexData = new vertexPolygon[vertexCount];

	mainMenuVertexData = new ButtonVtx[vtxCountButtons];

	int vertexIndex = 0;
	for (int i = 0; i < nrOfMenuButtons; i++)
	{
		int buttonVtxCount = mainMenu.GetMenuButtons()[i].GetVertexCount();//mainMenu.GetButtonVertices(i).size();//mainScene.GetMeshData()[i].GetVertices().size();
		for (int j = 0; j < buttonVtxCount; j++)
		{
			//mainMenuVertexData[vertexIndex] = mainScene.GetMeshData()[i].GetVertices()[j];
			mainMenuVertexData[vertexIndex] = mainMenu.GetButtonVertices(i)[j];
			vertexIndex++;
		}
	}
	mainRenderer.CompileMenuVertexData(vtxCountButtons, mainMenuVertexData);
}

//=============================================================
//	Main engine loop
//=============================================================
void GameEngine::Run()
{
	// Unlimit fps
	glfwSwapInterval(0);

	// If this becomes true the program will have failed in someway or been manually shut down
	bool shutdown = false;

	// Load ImGui content
	static bool renderDepth = false;
	ImGuiInit();

	// Compile Main Menu vertex data (is this really the best spot? no idea rn)
	CompileMainMenuData();

	// Framebuffer for the main renderer
	if (mainRenderer.CreateFrameBuffer() != 0)
		shutdown = true;




	while (!glfwWindowShouldClose(mainRenderer.getWindow()))
	{
		glfwPollEvents();
		if (glfwGetKey(mainRenderer.getWindow(), GLFW_KEY_1) == GLFW_PRESS)
		{
			menuIsRunning = false;
		}
		else if (mainScene.GetExit())
		{
			menuIsRunning = true;
			mainScene.Exited();
		}

		// Deltatime via ImGui
		float deltaTime = ImGui::GetIO().DeltaTime;
		if (deltaTime > 1.0f)
			deltaTime = 0.0f;


		if (menuIsRunning == true)
		{
			// RENDER CALL FOR MAIN MENU HERE
			mainMenu.MenuUpdate(mainRenderer.getWindow(), deltaTime);
			Camera temp;

			mainRenderer.RenderMainMenu(mainScene.GetShader(3), mainMenu.GetMenuButtons(), temp, gClearColour, mainMenu.GetButtonTexture());

			glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glfwSwapBuffers(mainRenderer.getWindow());
		}
		else if (menuIsRunning == false && mainScene.GetRoomLoaded())
		{
			// Main updates to a scene
			// Includes all interactions in the game world
			mainScene.Update(mainRenderer.getWindow(), deltaTime);

			// PrePass render for Shadow mapping 
			mainRenderer.prePassRender(mainScene.GetShader(1), mainScene.GetMeshData(), mainScene.GetCamera(), gClearColour, mainScene.GetDirectionalLights());
			mainRenderer.SetViewport();	//resets the viewport
			// First render pass
			mainRenderer.firstPassRenderTemp(mainScene.GetShader(2), mainScene.GetMeshData(), gClearColour);
			
			if (!mainScene.GetCurrentState() == PAUSED)
			{
				// ---- Main render call --- ///
				mainRenderer.Render(mainScene.GetShader(0),
					mainScene.GetMeshData(),
					mainScene.GetCamera(),
					gClearColour,
					mainScene.GetPointLights(),
					mainScene.GetDirectionalLights(),
					mainScene.GetMaterials());

				// Render a second pass for the fullscreen quad
				// Important for rendering the scene
				mainRenderer.secondPassRenderTemp(mainScene.GetShader(2));
				// Draw call for fsq and imgui
				glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
				glDrawArrays(GL_TRIANGLES, 0, 6);
				UpdateImGui(renderDepth);
				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
				glfwSwapBuffers(mainRenderer.getWindow());
			}
			else
			{
				// Pause screen draw call
				mainRenderer.secondPassRenderPauseOverlay(mainScene.GetShader(2), mainMenu.GetPauseOverlay());
				glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glfwSwapBuffers(mainRenderer.getWindow());
				// Pause screen draw call
			}		
		}
		else if(!mainScene.GetRoomLoaded())
		{	
			// Loading screen draw call
			mainRenderer.secondPassRenderPauseOverlay(mainScene.GetShader(2), mainMenu.GetLoadingTexture());

			// Draw call for fsq and imgui
			glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
			glDrawArrays(GL_TRIANGLES, 0, 6);
			UpdateImGui(renderDepth);
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			glfwSwapBuffers(mainRenderer.getWindow());
			// Draw call for fsq and imgui

			// Heavy loading work
			mainScene.LoadRoom();
			CompileRoomData();
		}
	


		// Draw fullscreen quad
		// Could be moved to the renderer
		//glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
		//glDrawArrays(GL_TRIANGLES, 0, 6);

		//// Render ImGui
		//// Update ImGui content
		//UpdateImGui(renderDepth);
		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


		//glfwSwapBuffers(mainRenderer.getWindow());
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
