#include "ShadowMap.h" 



ShadowMap::ShadowMap()
{
	SHADOW_WIDTH = 2048;
	SHADOW_HEIGHT = 2048;

	shadow_id = -1;
}


ShadowMap::~ShadowMap()
{
}

/*
=============================================================
 Creates the frame buffer with one depth attachment to hold 
 the depth texture, that will be sampled from to create the 
 shadows.
=============================================================
*/
int ShadowMap::CreateFrameBufferSM()
{
	int err = 0;

	glGenTextures(1, depthMapAttachments);
	glBindTexture(GL_TEXTURE_2D, depthMapAttachments[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glGenFramebuffers(1, &depthMapFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapAttachments[0], 0);

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

/*
=============================================================
 Creates the shadow matrix, using the lights position. Add
 the shader program that needs the SHADOW_MAT sent to.
=============================================================
*/
void ShadowMap::CreateShadowMatrixData(glm::vec3 lightPos, GLuint shaderProg)
{
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-3, 3, -3, 3, -1, 10); //An orthographic matrix
	glm::mat4 depthViewMatrix = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //View from the light position towards origo
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

	glm::mat4 shadowBias = glm::mat4(0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0);


	shadow_matrix = depthMVP * shadowBias;
	shadow_id = glGetUniformLocation(shaderProg, "SHADOW_MAT");
	if (shadow_id == -1) {
		OutputDebugStringA("Error, cannot find 'shadow_id' attribute in Vertex shader SM\n");
		return;
	}

	glUniformMatrix4fv(shadow_id, 1, GL_FALSE, glm::value_ptr(shadow_matrix));
}

/*
=============================================================
This binds and prepares the frame buffer, called before 
rendering the pre-pass.
=============================================================
*/
void ShadowMap::bindForWriting()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); //Set the viewport to the same resolution as the frame buffer to be able to render shadows correctly 
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

/*
=============================================================
Used to activate and bind the depth texture generated. 
Called during the render loop of objects.
Sends the information of texture to specified shader program.
=============================================================
*/
void ShadowMap::bindForReading(GLenum textureUnit, GLuint shaderProg)
{
	glActiveTexture(textureUnit); //PF 
	glBindTexture(GL_TEXTURE_2D, depthMapAttachments[0]); //PF 

	glUniform1i(glGetUniformLocation(shaderProg, "shadowMap"), 2); //PF 
}

/*
=============================================================
 Get the depth map attachment at [0]. 
=============================================================
*/
unsigned int ShadowMap::getDepthMapAttachment() const
{
	return this->depthMapAttachments[0];
}

unsigned int ShadowMap::getShadowID() const
{
	return this->shadow_id;
}

glm::mat4 ShadowMap::getShadowMatrix() const
{
	return this->shadow_matrix;
}

/*
=============================================================
Set the size of the depth map for shadows,
default at 2048 x 2048.
=============================================================
*/
void ShadowMap::setTextureSize(unsigned int size)
{
	this->SHADOW_HEIGHT = size;
	this->SHADOW_WIDTH = size;
}
