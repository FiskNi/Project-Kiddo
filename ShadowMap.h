#pragma once
#include "Headers.h"
class ShadowMap
{
private:
	unsigned int SHADOW_WIDTH;
	unsigned int SHADOW_HEIGHT;

	unsigned int shadowDepthMapFbo;
	unsigned int depthMapAttachment[1];

	GLuint shadow_id;
	glm::mat4 shadow_matrix;

public:
	ShadowMap();
	~ShadowMap();

	int CreateFramebufferSM();
	void CreateShadowMatrixData(glm::vec3 lightPos, GLuint shaderProg);
	void bindForWriting();
	void bindForReading(GLenum textureUnit, GLuint shaderProg);

	unsigned int getDepthMapAttachment() const; 

};

