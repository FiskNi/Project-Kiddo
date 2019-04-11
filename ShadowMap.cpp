#include "ShadowMap.h"



ShadowMap::ShadowMap()
{
	SHADOW_WIDTH = 2048;
	SHADOW_HEIGHT = 2048;

	shadow_id = -1;
}


ShadowMap::~ShadowMap()
{
	glDeleteFramebuffers(1, &shadowDepthMapFbo);
	glDeleteTextures(1, depthMapAttachment);
}

//Creates the framebuffer with one depthMapAttchment
int ShadowMap::CreateFramebufferSM()
{
	int err = 0;

	glGenTextures(1, depthMapAttachment);
	glBindTexture(GL_TEXTURE_2D, depthMapAttachment[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glGenFramebuffers(1, &shadowDepthMapFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapAttachment[0], 0);

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

//Created the shadow matrix based on input light position, and which shader program it will be used in. 
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
}

//Used when pre-rendering the scene.
void ShadowMap::bindForWriting()
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowDepthMapFbo); 
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}

//Used during the main render loop, needs the texture unit and main shader program.
void ShadowMap::bindForReading(GLenum textureUnit, GLuint shaderProg)
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, depthMapAttachment[0]); 

	glUniform1i(glGetUniformLocation(shaderProg, "shadowMap"), 2);
}

//Access to the depthMapAttachment
unsigned int ShadowMap::getDepthMapAttachment() const
{
	return depthMapAttachment[0];
}
