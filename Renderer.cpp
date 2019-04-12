#include "Renderer.h"



Renderer::Renderer()
{
	initWindow(WIDTH, HEIGHT);
}


Renderer::~Renderer()
{

}

GLFWwindow* Renderer::getWindow()
{
	return gWindow;
}

void Renderer::prePassRender(GLuint gShaderProgram, std::vector<CreatePrimitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, ShadowMap SM)
{
	// set the color TO BE used (this does not clear the screen right away)
	//glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	//glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram);

	// tell opengl we are going to use the VAO we described earlier
	for (int i = 0; i < objects.size(); i++)
	{
		SM.CreateShadowMatrixData(glm::vec3(4.0, 6.0, 2.0), gShaderProgram);
		CreateModelMatrix(objects[i].getWorldPosition(), objects[i].getWorldRotation(), gShaderProgram);
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		glBindVertexArray(objects[i].getVertexAttribute());
		// ask OpenGL to draw 3 vertices starting from index 0 in the vertex array 
		// currently bound (VAO), with current in-use shader. Use TOPOLOGY GL_TRIANGLES,
		// so for one triangle we need 3 vertices!

		glDrawArrays(GL_TRIANGLES, 0, 100);
	}
}

void Renderer::Render(GLuint gShaderProgram, std::vector<CreatePrimitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, ShadowMap SM)
{
	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram);

	glUniform3fv(gUniformColourLoc, 1, &gUniformColour[0]);


	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

	// tell opengl we are going to use the VAO we described earlier
	for (int i = 0; i < objects.size(); i++)
	{
		
		CreateModelMatrix(objects[i].getWorldPosition(), objects[i].getWorldRotation(), gShaderProgram);
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		glBindVertexArray(objects[i].getVertexAttribute());
		// ask OpenGL to draw 3 vertices starting from index 0 in the vertex array 
		// currently bound (VAO), with current in-use shader. Use TOPOLOGY GL_TRIANGLES,
		// so for one triangle we need 3 vertices!

		//SM.bindForReading(GL_TEXTURE2, gShaderProgram); ADD "shadowMap" in main shader.

		glDrawArrays(GL_TRIANGLES, 0, 100);
	}
	
}
void Renderer::Render(GLuint gShaderProgram, std::vector<CreatePrimitive> objects, Camera camera, float gClearColour[3], float gUniformColour[3], GLint gUniformColourLoc, float int)
{
	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], 1.0f);
	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);

	// tell opengl we want to use the gShaderProgram
	glUseProgram(gShaderProgram);

	glUniform3fv(gUniformColourLoc, 1, &gUniformColour[0]);


	glUniformMatrix4fv(12, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
	glUniformMatrix4fv(13, 1, GL_FALSE, glm::value_ptr(camera.GetProjectionMatrix()));

	// tell opengl we are going to use the VAO we described earlier
	for (int i = 0; i < objects.size(); i++)
	{

		CreateModelMatrix(objects[i].getWorldPosition(), objects[i].getWorldRotation(), gShaderProgram);
		glUniformMatrix4fv(14, 1, GL_FALSE, glm::value_ptr(MODEL_MAT));
		glBindVertexArray(objects[i].getVertexAttribute());
		// ask OpenGL to draw 3 vertices starting from index 0 in the vertex array 
		// currently bound (VAO), with current in-use shader. Use TOPOLOGY GL_TRIANGLES,
		// so for one triangle we need 3 vertices!
		glDrawArrays(GL_TRIANGLES, 0, 100);
	}

}


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

void Renderer::SetViewport()
{
	// usually (not necessarily) this matches with the window size
	glViewport(0, 0, WIDTH, HEIGHT);
}

void Renderer::CreateModelMatrix(glm::vec3 translation, float rotation, GLuint shaderProg)
{
	glm::mat4 ID_MAT = glm::mat4(1.0f);
	MODEL_MAT = glm::translate(ID_MAT, translation);
	MODEL_MAT = glm::rotate(MODEL_MAT, rotation, glm::vec3(0.0f, 1.0f, 0.0f));
}