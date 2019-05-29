#include "GameEngine.h"



GameEngine::GameEngine()
{
	mainSceneVertexData = nullptr;
	mainMenuVertexData = nullptr;
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
	//meshCount = mainScene.GetMeshData().size();
	//vertexCount = 0;
	//for (int i = 0; i < meshCount; i++)
	//{
	//	vertexCount += mainScene.GetMeshData()[i].GetVertices().size();
	//}
	//// Allocated memory
	//mainSceneVertexData = new vertexPolygon[vertexCount];

	//int vertexIndex = 0;
	//for (int i = 0; i < meshCount; i++)
	//{
	//	int meshVtxCount = mainScene.GetMeshData()[i].GetVertices().size();
	//	for (int j = 0; j < meshVtxCount; j++)
	//	{
	//		mainSceneVertexData[vertexIndex] = mainScene.GetMeshData()[i].GetVertices()[j];
	//		vertexIndex++;
	//	}
	//}
	//mainRenderer.CompileVertexData(vertexCount, mainSceneVertexData);



	//menuIsRunning = true;

	musicEngine = irrklang::createIrrKlangDevice();
	if (musicEngine)
		musicEngine->play2D("irrKlang/media/sad-music-box.mp3", true);

	soundEffectEngine = irrklang::createIrrKlangDevice();

}

GameEngine::~GameEngine()
{
	if (mainSceneVertexData)
		delete mainSceneVertexData;
	if (mainMenuVertexData)
		delete mainMenuVertexData;
	if (pauseMenuVertexData)
		delete pauseMenuVertexData;
	if (collectibleMenuVertexData)
		delete collectibleMenuVertexData;
	if (htpMenuVertexData)
		delete htpMenuVertexData;
	if (musicEngine)
		musicEngine->drop();
	if (soundEffectEngine)
		soundEffectEngine->drop();
}

void GameEngine::CompileRoomData()
{
	if (mainSceneVertexData)
		delete[] mainSceneVertexData;

	mainSceneVertexData = nullptr;

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

void GameEngine::CompileMenuData(ACTIVEMENU activeMenu)
{
	int nrOfMenuButtons = 0;
	int vtxCountButtons = 0;
	int vertexIndex = 0;

	if (activeMenu == MAINACTIVE) {
		nrOfMenuButtons = mainMenu.GetNrOfMenuButtons();
		vtxCountButtons = mainMenu.GetVertexCountMainTotal();

		mainMenuVertexData = new ButtonVtx[vtxCountButtons];

		for (int i = 0; i < nrOfMenuButtons; i++)
		{
			int buttonVtxCount = mainMenu.GetMainMenuButtons()[i].GetVertexCount();
			for (int j = 0; j < buttonVtxCount; j++)
			{
				mainMenuVertexData[vertexIndex] = mainMenu.GetMainMenuButtonVertices(i)[j];
				vertexIndex++;
			}
		}
		mainRenderer.CompileMenuVertexData(vtxCountButtons, mainMenuVertexData, MAINACTIVE);

	}
	else if (activeMenu == PAUSEACTIVE) {
		nrOfMenuButtons = mainMenu.GetNrOfPauseButtons();
		vtxCountButtons = mainMenu.GetVertexCountPauseTotal();

		pauseMenuVertexData = new ButtonVtx[vtxCountButtons];

		for (int i = 0; i < nrOfMenuButtons; i++)
		{
			int buttonVtxCount = mainMenu.GetPauseMenuButtons()[i].GetVertexCount();
			for (int j = 0; j < buttonVtxCount; j++)
			{
				pauseMenuVertexData[vertexIndex] = mainMenu.GetPauseMenuButtonVertices(i)[j];
				vertexIndex++;
			}
		}
		mainRenderer.CompileMenuVertexData(vtxCountButtons, pauseMenuVertexData, PAUSEACTIVE);

	}
	else if (activeMenu == COLLECTIBLEACTIVE) {
		nrOfMenuButtons = mainMenu.GetNrOfCollectibleButtons();
		vtxCountButtons = mainMenu.GetVertexCountCollectibleTotal();

		collectibleMenuVertexData = new ButtonVtx[vtxCountButtons];

		for (int i = 0; i < nrOfMenuButtons; i++)
		{
			int buttonVtxCount = mainMenu.GetCollectibleMenuButtons()[i].GetVertexCount();
			for (int j = 0; j < buttonVtxCount; j++)
			{
				collectibleMenuVertexData[vertexIndex] = mainMenu.GetCollectibleMenuButtonVertices(i)[j];
				vertexIndex++;
			}
		}
		mainRenderer.CompileMenuVertexData(vtxCountButtons, collectibleMenuVertexData, COLLECTIBLEACTIVE);

	}
	else if(activeMenu == HTPACTIVE){
		nrOfMenuButtons = mainMenu.GetNrOfHtpButtons();
		vtxCountButtons = mainMenu.GetVertexCountHtpTotal();

		htpMenuVertexData = new ButtonVtx[vtxCountButtons];

		for (int i = 0; i < nrOfMenuButtons; i++)
		{
			int buttonVtxCount = mainMenu.GetHtpMenuButtons()[i].GetVertexCount();
			for (int j = 0; j < buttonVtxCount; j++)
			{
				htpMenuVertexData[vertexIndex] = mainMenu.GetHtpMenuButtonVertices(i)[j];
				vertexIndex++;
			}
		}
		mainRenderer.CompileMenuVertexData(vtxCountButtons, htpMenuVertexData, HTPACTIVE);

	}

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

	// Cursor creation
	//GLFWcursor* mainCursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
	GLFWcursor* handCursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
	// Sets cursor to the OG cursor arrow
	glfwSetCursor(mainRenderer.getWindow(), NULL);

	// Load ImGui content
	static bool renderDepth = false;
	ImGuiInit();

	// Compile Menu data vertex data
	CompileMenuData(MAINACTIVE);
	CompileMenuData(PAUSEACTIVE);
	CompileMenuData(COLLECTIBLEACTIVE);
	CompileMenuData(HTPACTIVE);

	// Framebuffer for the main renderer
	if (mainRenderer.CreateFrameBuffer() != 0)
		shutdown = true;

	while (!glfwWindowShouldClose(mainRenderer.getWindow()))
	{
		glfwPollEvents();
		if (mainMenu.GetHasButtonActionExecuted() == false) 
		{
			if (mainMenu.GetUpdateState() == PLAYING) 
			{
				mainScene.ResumeGame();
				mainMenu.SetIsMenuRunning(false);
				mainMenu.SetButtonActionExecuted(true);
				mainMenu.ResetUpdateState();
				switchCursorOnce = false;
				if (musicEngine)
					musicEngine->stopAllSounds();
			}
		}
		else if (mainScene.GetExit())
		{
			mainMenu.SetActiveMenu(MAINACTIVE);
			mainMenu.SetIsMenuRunning(true);
			mainScene.Exited();

		}


		// Deltatime via ImGui
		float deltaTime = ImGui::GetIO().DeltaTime;
		if (deltaTime > 1.0f)
			deltaTime = 0.0f;


		if (mainMenu.GetIsMenuRunning() == true)
		{
			// RENDER CALL FOR MAIN MENU HERE
			//mainMenu.SetActiveMenu(MAINACTIVE);
			mainMenu.MenuUpdate(mainRenderer.getWindow(), deltaTime);

			if (mainMenu.CheckButtonHovering(mainRenderer.getWindow()) == true && switchCursorOnce != true) {
				glfwSetCursor(mainRenderer.getWindow(), handCursor);
				//std::cout << "Cursor switched HAND" << std::endl;
				switchCursorOnce = true;
			}
			else if (mainMenu.CheckButtonHovering(mainRenderer.getWindow()) == false && switchCursorOnce == true) {
				glfwSetCursor(mainRenderer.getWindow(), NULL);
				//std::cout << "Cursor switched REGULAR" << std::endl;
				switchCursorOnce = false;
			}

			// Switch from pause menu to main menu here to avoid accidental "double tap" with the input buffer saving the position collision from pause menu for main menu
			if (mainMenu.GetActiveMenu() == PAUSEACTIVE) {
				mainMenu.SetActiveMenu(MAINACTIVE);
			}
			mainRenderer.RenderMenu(mainScene.GetShader(3), mainMenu.GetMainMenuButtons(), gClearColour, mainMenu.GetButtonTextures(), MAINACTIVE);
			// If Collectible Menu is active, render te Collectible menu over the main menu
			if (mainMenu.GetActiveMenu() == COLLECTIBLEACTIVE) {
				mainRenderer.RenderMenu(mainScene.GetShader(3), mainMenu.GetCollectibleMenuButtons(), gClearColour, mainMenu.GetCollectibleTextures(), COLLECTIBLEACTIVE);
			}
			else if (mainMenu.GetActiveMenu() == HTPACTIVE) {
				mainRenderer.RenderMenu(mainScene.GetShader(3), mainMenu.GetHtpMenuButtons(), gClearColour, mainMenu.GetHtpTextures(), HTPACTIVE);
			}

			glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glfwSwapBuffers(mainRenderer.getWindow());
		}
		else if (mainMenu.GetIsMenuRunning() == false && mainScene.GetRoomLoaded())
		{
			// Main updates to a scene
			// Includes all interactions in the game world
			mainScene.Update(mainRenderer.getWindow(), deltaTime);

			// PrePass render for Shadow mapping 
			mainRenderer.ShadowmapRender(mainScene.GetShader(1), mainScene.GetMeshData(), mainScene.GetCamera(), gClearColour, mainScene.GetDirectionalLights());
			mainRenderer.SetViewport();	//resets the viewport
			// First render pass
			mainRenderer.firstPassRenderTemp(mainScene.GetShader(2), gClearColour);
			
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
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwSwapBuffers(mainRenderer.getWindow());
			}
			else
			{
				// Pause Menu Render Call
				mainMenu.SetActiveMenu(PAUSEACTIVE);
				//std::cout << mainScene.GetCurrentState() << std::endl;

				if (mainMenu.CheckButtonHovering(mainRenderer.getWindow()) == true && switchCursorOnce != true) {
					glfwSetCursor(mainRenderer.getWindow(), handCursor);
					//std::cout << "Cursor switched HAND" << std::endl;
					switchCursorOnce = true;
				}
				else if (mainMenu.CheckButtonHovering(mainRenderer.getWindow()) == false && switchCursorOnce == true) {
					glfwSetCursor(mainRenderer.getWindow(), NULL);
					//std::cout << "Cursor switched REGULAR" << std::endl;
					switchCursorOnce = false;
				}

				// Checks for clicking on the pause menu
				glfwPollEvents();
				if (glfwGetMouseButton(mainRenderer.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS && printMouseClickOnce == false)
				{
					// Gets the clicked cursor position and checks for collision with any of the buttons for Pause Menu
					double x, y;
					glfwGetCursorPos(mainRenderer.getWindow(), &x, &y);
					//std::cout << "Current Cursor Position: " << x << "  " << y << std::endl;
					if (mainMenu.CheckCollision(x, y, true) == true) {
						int clickedButton = mainMenu.GetLastClickedButton();

						if (soundEffectEngine)
							soundEffectEngine->play2D("irrKlang/media/paper.mp3", false);

						if (clickedButton == 1) {
							// RESUME GAME	
							mainScene.ResumeGame();
							//std::cout << "RESUME" << std::endl;
						}
						else if (clickedButton == 2) {
							// RESTART
							mainScene.RestartGame();
							//std::cout << "Restarting level" << std::endl;
						}
						else if (clickedButton == 3) {
							// Quit to Main Menu (START WILL WORK AS RESUME)
							//mainMenu.SetActiveMenu(MAINACTIVE);
							mainMenu.ResetUpdateState();
							mainScene.ExitToMainMenu();
							mainMenu.SetIsMenuRunning(true);
							if (musicEngine)
								musicEngine->play2D("irrKlang/media/sad-music-box.mp3", true);
							//std::cout << "MAIN MENU" << std::endl;
						}
						mainMenu.ResetLastClickedButton();
						switchCursorOnce = false;
						printMouseClickOnce = true;
						mainMenu.SetButtonActionExecuted(false);
					}
				}
				else if (glfwGetMouseButton(mainRenderer.getWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
					printMouseClickOnce = false;
				}

				mainMenu.MenuUpdate(mainRenderer.getWindow(), deltaTime);

				mainRenderer.RenderMenu(mainScene.GetShader(3), mainMenu.GetPauseMenuButtons(), gClearColour, mainMenu.GetPauseButtonTextures(), PAUSEACTIVE);
				mainRenderer.secondPassRenderTemp(mainScene.GetShader(2));

				//glUniform1i(3, renderDepth);  // Boolean for the shadowmap toggle
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

	// Delete cursors
	//glfwDestroyCursor(mainCursor);
	glfwDestroyCursor(handCursor);

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
