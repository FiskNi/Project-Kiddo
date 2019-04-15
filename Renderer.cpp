#include "Renderer.h"



Renderer::Renderer()
{
	initWindow(WIDTH, HEIGHT);
}


Renderer::~Renderer()
{
	glDeleteFramebuffers(1, &gFbo);
	glDeleteTextures(2, gFboTextureAttachments);
}

GLFWwindow* Renderer::getWindow()
{
	return gWindow;
}

//=============================================================
//	From template - Needs explanation
//=============================================================
void Renderer::firstPassRenderTemp(ShaderHandler gShaderProgram, std::vector<Primitive> objects, float gClearColour[])
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
//=============================================================
void Renderer::secondPassRenderTemp(ShaderHandler gShaderProgram)
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
}


//=============================================================
//	Pre pass render needed to generate depth map for shadows.
//=============================================================
void Renderer::prePassRender(ShaderHandler gShaderProgram, std::vector<Primitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, ShadowMap SM)
{
	// set the color TO BE used (this does not clear the screen right away)
	//glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	//glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram.getShader());

	// tell opengl we are going to use the VAO we described earlier
	for (int i = 0; i < objects.size(); i++)
	{
		SM.CreateShadowMatrixData(glm::vec3(4.0, 6.0, 2.0), gShaderProgram.getShader());
		CreateModelMatrix(objects[i].getPosition(), objects[i].getWorldRotation(), gShaderProgram.getShader());
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		glBindVertexArray(objects[i].getVertexAttribute());

		passTextureData(GL_TEXTURE0, objects[i].getTextureID(), gShaderProgram.getShader(),
			"diffuseTex", 0);
		// ask OpenGL to draw 3 vertices starting from index 0 in the vertex array 
		// currently bound (VAO), with current in-use shader. Use TOPOLOGY GL_TRIANGLES,
		// so for one triangle we need 3 vertices!

		glDrawArrays(GL_TRIANGLES, 0, 100);
	}
}

//=============================================================
//	Main render pass
//=============================================================
void Renderer::Render(ShaderHandler gShaderProgram, std::vector<Primitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, ShadowMap SM, Light lightArr[])
{
	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram.getShader());

	// ImGui uniform
	glUniform3fv(gUniformColourLoc, 1, &gUniformColour[0]);

	// Shadowmap ViewProjection matrix
	SM.CreateShadowMatrixData(glm::vec3(4.0, 6.0, 2.0), gShaderProgram.getShader());

	// Camera uniforms
	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

	glm::mat4 model = glm::mat4(1.0f);
	glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(model));

	glUniform3fv(16, 1, glm::value_ptr(camera.camPos));

	//Add for-loop Here Later.
	for (int i = 0; i < nr_P_LIGHTS; i++)
	{
		lightArr[i].sendToShader(gShaderProgram, i);
	}
	
	// Main render queue
	// Currently the render swaps buffer for every object which could become slow further on
	// If possible the rendercalls could be improved
	unsigned int startIndex = 0;
	for (int i = 0; i < objects.size(); i++)
	{
		// Updates the world matrix for object positioning and orientation
		CreateModelMatrix(objects[i].getPosition(), objects[i].getWorldRotation(), gShaderProgram.getShader());
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));

		// Binds the VAO of an object to be renderer. Could become slow further on.
		glBindVertexArray(objects[i].getVertexAttribute());

		// Bind an objects texture for the shader
		passTextureData(GL_TEXTURE0, objects[i].getTextureID(), gShaderProgram.getShader(),
			"diffuseTex", 0);

		// Shadowmap
		//SM.bindForReading(GL_TEXTURE2, gShaderProgram.getShader()); //ADD "shadowMap" in main shader.
		passTextureData(GL_TEXTURE2, SM.getDepthMapAttachment(), gShaderProgram.getShader(),
			"shadowMap", 2);

		// Draw call
		// As the buffer is swapped for each object the drawcall currently always starts at index 0
		// This is what could be improved with one large buffer and then advance the start index for each object
		glDrawArrays(GL_TRIANGLES, 0, objects[i].getPolygonCount());
	}
	
}

//=============================================================
//	From template - Needs explanation
//=============================================================
int Renderer::CreateFrameBuffer() {
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

	gWindow = glfwCreateWindow(w, h, "basic GLFW window", NULL, NULL);
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
	fprintf(stderr, "Renderer: %s\n", renderer);
	fprintf(stderr, "OpenGL version %s\n", version);

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
//=============================================================
void Renderer::CreateModelMatrix(glm::vec3 translation, float rotation, GLuint shaderProg)
{
	glm::mat4 ID_MAT = glm::mat4(1.0f);
	MODEL_MAT = glm::translate(ID_MAT, translation);
	MODEL_MAT = glm::rotate(MODEL_MAT, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}


/*
=============================================================
Used to activate and bind the generated texture.
Called during the render loop of objects.
Sends the information of texture to specified shader program.
=============================================================
*/
void Renderer::passTextureData(GLuint TextureUnit, GLuint texID, GLuint shaderProg,
	GLchar* uniformName, int loc)
{
	glActiveTexture(TextureUnit);
	glBindTexture(GL_TEXTURE_2D, texID);
	glUniform1i(glGetUniformLocation(shaderProg, uniformName), loc);
}