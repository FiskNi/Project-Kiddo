#include "Material.h"



Material::Material(unsigned int id)
{
	materialID = id;
	albedo = 0;
	normal = 0;
	hasNormalmap = 0;
}


Material::~Material()
{
}

void Material::createAlbedo(std::string path)
{

	//Generate Texture, 1:st argument is amt of textures second is where to store them
	glGenTextures(1, &albedo);
	glBindTexture(GL_TEXTURE_2D, albedo);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widht,
		height,
		colourChannels;
	
	const char* filePath = path.c_str();
	
	unsigned char* data = stbi_load(filePath, &widht, &height, &colourChannels, 0);
	if (data)
	{
		//Function arguments:  | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Data type | Image data |
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widht, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
		std::cout << "Failed to load texture. Reason: " << stbi_failure_reason() << std::endl;
	stbi_image_free(data);
}

void Material::createNormal(std::string path)
{

	//Generate Texture, 1:st argument is amt of textures second is where to store them
	glGenTextures(1, &normal);
	glBindTexture(GL_TEXTURE_2D, normal);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int widht,
		height,
		colourChannels;

	const char* filePath = path.c_str();

	unsigned char* data = stbi_load(filePath, &widht, &height, &colourChannels, 0);
	if (data)
	{
		//Function arguments:  | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Data type | Image data |
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widht, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		hasNormalmap = true;
	}
	else
		std::cout << "Failed to load texture. Reason: " << stbi_failure_reason() << std::endl;
	stbi_image_free(data);


}

unsigned int Material::getMaterialID() const
{
	return materialID;
}

GLuint Material::getAlbedo() const
{
	return albedo;
}

GLuint Material::getNormal() const
{
	return normal;
}

bool Material::hasNormal() const
{
	return hasNormalmap;
}


