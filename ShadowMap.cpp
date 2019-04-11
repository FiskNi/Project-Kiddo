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

//Create framebuffer with one depth attachment. 
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

//Creates the shadow matrix from the lights position, and what shader program needs the info 
void ShadowMap::CreateShadowMatrixData(glm::vec3 lightPos, GLuint shaderProg)
{
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20); //An orthographic matrix 
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
}

//Bind fbo before pre pass render, to generate depth map for shadow mapping 
void ShadowMap::bindForWriting()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); //Set the viewport to the same resolution as the framebuffer to be able to render shadows correctly 
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo); //PF 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //PF 
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

// Use during the render loop, sends the texture to specific shader program. 
void ShadowMap::bindForReading(GLenum textureUnit, GLuint shaderProg)
{
	glActiveTexture(textureUnit); //PF 
	glBindTexture(GL_TEXTURE_2D, depthMapAttachments[0]); //PF 

	glUniform1i(glGetUniformLocation(shaderProg, "shadowMap"), 2); //PF 
}

unsigned int ShadowMap::getDepthMapAttachment() const
{
	return depthMapAttachments[0];
}

unsigned int ShadowMap::getShadowID() const
{
	return shadow_id;
}

glm::mat4 ShadowMap::getShadowMatrix() const
{
	return shadow_matrix;
}
