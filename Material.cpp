#include "Material.h"
// Check header for implementation details


Material::Material(string name, unsigned int id)
{
	this->name = name;
	materialID = id;	// The ID to match with an object comes in with the constuctor
	albedo = 0;
	normal = 0;

	hasAlbedoMap = false;
	hasNormalMap = false;

	this->ambient = glm::vec3(0);
	this->diffuse = glm::vec3(0);
	this->specular = glm::vec3(0);
	this->emissive = glm::vec3(0);
}

Material::Material(PhongMaterial material, unsigned int id)
{
	this->name = (string)material.name;
	materialID = id;	// The ID to match with an object comes in with the constuctor
	albedo = 0;
	normal = 0;

	hasAlbedoMap = false;
	hasNormalMap = false;

	for (int i = 0; i < 3; i++)
		this->ambient[i] = material.ambient[i];
	for (int i = 0; i < 3; i++)
		this->diffuse[i] = material.diffuse[i];
	for (int i = 0; i < 3; i++)
		this->specular[i] = material.specular[i];
	for (int i = 0; i < 3; i++)
		this->emissive[i] = material.emissive[i];
	this->opacity = material.opacity;



	createAlbedo(material.albedo);
	createNormal(material.normal);
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
	// Flips the imported texture according to OpenGL UVs
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(filePath, &widht, &height, &colourChannels, 3);
	if (data)
	{
		//Function arguments:  | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Data type | Image data |
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widht, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		hasAlbedoMap = true;
	}
	else
	{
		std::cout << "Failed to load albedo for material " << name << ". Reason: " << stbi_failure_reason();
		std::cout << ". No material applied or file missing?" << std::endl;
	}

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
	// Flips the imported texture according to OpenGL UVs
	stbi_set_flip_vertically_on_load(false);
	unsigned char* data = stbi_load(filePath, &widht, &height, &colourChannels, 0);
	if (data)
	{
		//Function arguments:  | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Data type | Image data |
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widht, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		hasNormalMap = true;
	}
	else
	{
		std::cout << "Failed to load normal for material " << name << ". Reason: " << stbi_failure_reason();
		std::cout << "No material applied or file missing?" << std::endl;
	}

	stbi_image_free(data);
}



