#include "Renderer.h"



Renderer::Renderer()
{

	initWindow(WIDTH, HEIGHT);

	// Creates the frame buffer for shadow mapping
	shadowMap.CreateFrameBufferSM();
}


Renderer::~Renderer()
{
	glDeleteFramebuffers(1, &gFbo);
	glDeleteTextures(2, gFboTextureAttachments);
	glDeleteVertexArrays(1, &gVertexAttribute);
	glDeleteBuffers(1, &gVertexBuffer);

}

GLFWwindow* Renderer::getWindow()
{
	return gWindow;
}

//=============================================================
//	From template - Needs explanation
//	Handles stuff for the fullscreen quad.
//	Usefull for rendering techniques.
//=============================================================
void Renderer::firstPassRenderTemp(Shader gShaderProgram, std::vector<Mesh> objects, 
	float gClearColour[])
{
	// first pass
	// render all geometry to a framebuffer object
	glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], gClearColour[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(gShaderProgram.getShader());
	glBindVertexArray(gShaderProgram.getVertexAttributes());
	glEnable(GL_DEPTH_TEST);
}

//=============================================================
//	From template - Needs explanation
//	Handles stuff for the fullscreen quad.
//	Usefull for rendering techniques.
//=============================================================
void Renderer::secondPassRenderTemp(Shader gShaderProgram)
{
	// first pass is done!
	// now render a second pass
	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gShaderProgram.getShader());
	glBindVertexArray(gShaderProgram.getVertexAttributes());
	glDisable(GL_DEPTH_TEST);
	// bind texture drawn in the first pass!
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[0]);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[1]);
	glBindTexture(GL_TEXTURE_2D, shadowMap.getDepthMapAttachment());
}

void Renderer::secondPassRenderPauseOverlay(Shader gShaderProgram, GLuint pauseOverlayTexture)
{

	// Renders alternative Full Screen Quad to cover the screen with a Pause Screen Overlay
	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(gShaderProgram.getShader());
	glBindVertexArray(gShaderProgram.getVertexAttributes());
	glDisable(GL_DEPTH_TEST);
	// bind texture drawn in the first pass!
	glActiveTexture(GL_TEXTURE0);
	// Binds the pauseOverlayTexture instead of gFboTextureAttachments[0], this is the overlay texture
	glBindTexture(GL_TEXTURE_2D, pauseOverlayTexture);

	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[1]);
	glBindTexture(GL_TEXTURE_2D, shadowMap.getDepthMapAttachment());
}



//=============================================================
//	Pre pass render needed to generate depth map for shadows.
//=============================================================
void Renderer::prePassRender(Shader gShaderProgram, 
	std::vector<Mesh> objects, 
	Camera camera, 
	float gClearColour[3], 
	std::vector<DirectionalLight> dirLightArr)
{
	// Position in shader
	int model_matrix = 1;
	int shadow_matrix = 2;

	//PrePass render for Shadow mapping 
	shadowMap.bindForWriting();

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram.getShader());
	glBindVertexArray(gVertexAttribute);
	// tell opengl we are going to use the VAO we described earlier
	unsigned int startIndex = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		shadowMap.CreateShadowMatrixData(dirLightArr[0].GetPos(), gShaderProgram.getShader());

		CreateModelMatrix(objects[i].GetPosition(), objects[i].GetRotation(), objects[i].GetScale(), gShaderProgram.getShader());
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		glUniformMatrix4fv(shadow_matrix, 1, GL_FALSE, glm::value_ptr(shadowMap.getShadowMatrix()));

		glDrawArrays(GL_TRIANGLES, startIndex, objects[i].GetVertexCount());
		startIndex += objects[i].GetVertexCount();
	}
}

//=============================================================
//	Main render pass
//=============================================================
void Renderer::Render(Shader gShaderProgram, std::vector<Mesh> objects, Camera camera, 
	float gClearColour[3], std::vector<Light> lightArr, 
	std::vector<DirectionalLight> dirLightArr, std::vector<Material> materials)
{
	// Position in shader
	int view_matrix = 5;
	int projection_matrix = 6;
	int model_matrix = 7;
	int shadow_matrix = 8;
	int cam_pos = 9;
	int has_normal = 10;
	int has_albedo = 11;
	int ambient = 12;
	int diffuse = 13;
	int specular = 14;
	int emissive = 15;

	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram.getShader());

	// Shadowmap ViewProjection matrix
	shadowMap.CreateShadowMatrixData(dirLightArr[0].GetPos(), gShaderProgram.getShader());

	// Per shader uniforms
	glUniformMatrix4fv(view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(projection_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	glUniformMatrix4fv(shadow_matrix, 1, GL_FALSE, glm::value_ptr(shadowMap.getShadowMatrix()));
	glUniform3fv(cam_pos, 1, glm::value_ptr(camera.GetPosition()));

	dirLightArr[0].SendToShader(gShaderProgram);
	//Sending all the lights to shader.
	for (int i = 0; i < nr_P_LIGHTS; i++)
	{
		lightArr[i].SendToShader(gShaderProgram, i);
	}

	// Main render queue
	// Currently the render swaps buffer for every object which could become slow further on
	// If possible the rendercalls could be improved

	glBindVertexArray(gVertexAttribute);

	unsigned int startIndex = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		// Per object uniforms
		CreateModelMatrix(objects[i].GetPosition(), objects[i].GetRotation(), objects[i].GetScale(), gShaderProgram.getShader());
		glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		glUniform1ui(has_normal, materials[objects[i].GetMaterialID()].hasNormal());
		glUniform1ui(has_albedo, materials[objects[i].GetMaterialID()].hasAlbedo());

		// Binds the VAO of an object to be renderer. Could become slow further on.

		// Binds the albedo texture from a material
		passTextureData(GL_TEXTURE0,
			materials[objects[i].GetMaterialID()].getAlbedo(),
			gShaderProgram.getShader(),
			"diffuseTex", 0);

		// Binds the normal texture from a material
		if (materials[objects[i].GetMaterialID()].hasNormal())
		{
			passTextureData(GL_TEXTURE1,
				materials[objects[i].GetMaterialID()].getNormal(),
				gShaderProgram.getShader(),
				"normalTex", 1);
		}

		glUniform3fv(ambient, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getAmbient()));
		glUniform3fv(diffuse, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getDiffuse()));
		glUniform3fv(specular, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getSpecular()));
		glUniform3fv(emissive, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getEmissive()));

		// Binds the shadowmap (handles by the renderer)
		passTextureData(GL_TEXTURE2,
			shadowMap.getDepthMapAttachment(),
			gShaderProgram.getShader(),
			"shadowMap", 2);

		// Draw call
		// As the buffer is swapped for each object the drawcall currently always starts at index 0
		// This is what could be improved with one large buffer and then advance the start index for each object
		glDrawArrays(GL_TRIANGLES, startIndex, objects[i].GetVertexCount());

		startIndex += objects[i].GetVertexCount();
	}

}


//=============================================================
//	Creates a vertexbuffer from all the recieved vertex data
//=============================================================
void Renderer::CompileVertexData(int vertexCount, vertexPolygon* vertices)
{
	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);

	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);

	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(vertexPolygon), vertices, GL_STATIC_DRAW);

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		0,							// location in shader
		3,							// how many elements of type (see next argument)
		GL_FLOAT,					// type of each element
		GL_FALSE,					// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(vertexPolygon),		// distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)			// offset of FIRST vertex in the list.
	);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

	glVertexAttribPointer(
		2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 5)
	);

	glVertexAttribPointer(
		3,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 8)
	);

	glVertexAttribPointer(
		4,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 11)
	);
}

//= ============================================================
//	Render pass for the main menu ( eventually pause menu as well )
//=============================================================
void Renderer::RenderMainMenu(Shader gShaderProgram, std::vector<MenuButton> objects, Camera camera, float gClearColour[3], GLuint texture )
{
	// Position in shader
	int view_matrix = 5;
	int projection_matrix = 6;
	int model_matrix = 7;
	//int shadow_matrix = 8;
	int cam_pos = 9;
	int has_normal = 10;
	int has_albedo = 11;
	int ambient = 12;
	int diffuse = 13;
	int specular = 14;
	int emissive = 15;

	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);													// MAYBE CLEAR THE COLOUR BUT MAYBE NOT

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram.getShader());


	// Per shader uniforms
	glUniformMatrix4fv(view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(projection_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));
	glUniform3fv(cam_pos, 1, glm::value_ptr(camera.GetPosition()));


	// Main render queue
	// Currently the render swaps buffer for every object which could become slow further on
	// If possible the rendercalls could be improved

	glBindVertexArray(gVertexAttributeMenu);

	unsigned int startIndex = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		/*// Per object uniforms
		//CreateModelMatrix(objects[i].GetPosition(), objects[i].GetRotation(), objects[i].GetScale(), gShaderProgram.getShader());
		//glUniformMatrix4fv(model_matrix, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		//glUniform1ui(has_normal, materials[objects[i].GetMaterialID()].hasNormal());
		//glUniform1ui(has_albedo, materials[objects[i].GetMaterialID()].hasAlbedo());

		// Binds the VAO of an object to be renderer. Could become slow further on.

		// Binds the albedo texture from a material
		//passTextureData(GL_TEXTURE0,
		//	materials[objects[i].GetMaterialID()].getAlbedo(),
		//	gShaderProgram.getShader(),
		//	"diffuseTex", 0);

		//// Binds the normal texture from a material
		//if (materials[objects[i].GetMaterialID()].hasNormal())
		//{
		//	passTextureData(GL_TEXTURE1,
		//		materials[objects[i].GetMaterialID()].getNormal(),
		//		gShaderProgram.getShader(),
		//		"normalTex", 1);
		//}

		//glUniform3fv(ambient, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getAmbient()));
		//glUniform3fv(diffuse, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getDiffuse()));
		//glUniform3fv(specular, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getSpecular()));
		//glUniform3fv(emissive, 1, glm::value_ptr(materials[objects[i].GetMaterialID()].getEmissive()));

		*/
		// Draw call
		// As the buffer is swapped for each object the drawcall currently always starts at index 0
		// This is what could be improved with one large buffer and then advance the start index for each object
		glDrawArrays(GL_TRIANGLES, startIndex, (int)objects[i].GetSize() );

		startIndex += objects[i].GetVertexCount();
	}

}

//=============================================================
//	Creates a vertexbuffer from the menu data to be rendered
//=============================================================
void Renderer::CompileMenuVertexData(int vertexCount, ButtonVtx* vertices)
{
	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttributeMenu);

	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttributeMenu);

	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBufferMenu);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferMenu);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(ButtonVtx), vertices, GL_STATIC_DRAW);

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		0,							// location in shader
		3,							// how many elements of type (see next argument)
		GL_FLOAT,					// type of each element
		GL_FALSE,					// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(ButtonVtx),		// distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)			// offset of FIRST vertex in the list.
	);

	glVertexAttribPointer(
		1,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(ButtonVtx),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

}

//=============================================================
//	From template - Needs explanation
//	Has to do with the fullscreen quad
//=============================================================
int Renderer::CreateFrameBuffer() 
{
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

//=============================================================
//	Initialized the opengl window
//	Could be a window class instead
//=============================================================
void Renderer::initWindow(unsigned int w, unsigned int h)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	gWindow = glfwCreateWindow(w, h, "Project Kiddo", NULL, NULL);
	if (!gWindow) {
		fprintf(stderr, "error creating window\n");
		exit(-1);
	}
	glfwMakeContextCurrent(gWindow);
	glewExperimental = GL_TRUE;

	// mouse callback
	// glfwSetCursorPosCallback(gWindow, mouseMoved);

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		fprintf(stderr, "Error GLEW: %s\n", glewGetErrorString(err));
	}
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	//fprintf(stderr, "Renderer: %s\n", renderer);
	//fprintf(stderr, "OpenGL version %s\n", version);
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version: " << version << endl;

	// start up time
	// timerStart = glfwGetTime();
	glClearColor(0.9f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
	glViewport(0, 0, w, h);

	return;
}

//=============================================================
//	Sets the main viewport, usually used after swapping shaders
//=============================================================
void Renderer::SetViewport()
{
	glViewport(0, 0, WIDTH, HEIGHT);
}

//=============================================================
//	Updates the model matrix for an object
//	Need to lookup quick matrix multiplication etc for rotation stuff
//	and what not.
//=============================================================
void Renderer::CreateModelMatrix(glm::vec3 translation, glm::quat rotation, glm::vec3 scale, GLuint shaderProg)
{
	// This calculation assumes the vertice position (not the vec3 position) are in their global 0 position
	MODEL_MAT = glm::mat4(1.0f);

	glm::mat4 translationMatrix = glm::translate(MODEL_MAT, translation);
	glm::mat4 rotationMatrix = glm::mat4_cast(rotation);
	glm::mat4 scaleMatrix = glm::scale(MODEL_MAT, scale);

	MODEL_MAT = translationMatrix * rotationMatrix * scaleMatrix;
}

//=============================================================
//	Used to activate and bind the generated texture.
//	Called during the render loop of objects.
//	Sends the information of texture to specified shader program.
//=============================================================
void Renderer::passTextureData(GLuint TextureUnit, GLuint texID, GLuint shaderProg,
	GLchar* uniformName, int loc)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shaderProg, uniformName), loc);
}