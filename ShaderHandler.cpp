#include "ShaderHandler.h"

ShaderHandler::ShaderHandler()
{
	gVertexBuffer = 0;
	gVertexAttribute = 0;


}


ShaderHandler::~ShaderHandler()
{
}

void ShaderHandler::CreateShaders(const char* vertexShader, const char* fragmentShader)
{
	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	//create vertex shader "name" and store it in "vs"
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	// open .glsl file and put it in a string
	//ifstream shaderFile("VertexShader.glsl");
	ifstream shaderFile(vertexShader);
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	// glShaderSource requires a double pointer.
	// get the pointer to the c style string stored in the string object.
	const char* shaderTextPtr = shaderText.c_str();

	// ask GL to use this string a shader code source
	glShaderSource(vs, 1, &shaderTextPtr, nullptr);

	// try to compile this shader source.
	glCompileShader(vs);

	// check for compilation error
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		glGetShaderInfoLog(vs, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	// repeat process for Fragment Shader (or Pixel Shader)
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	//shaderFile.open("Fragment.glsl");
	shaderFile.open(fragmentShader);
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(fs, 1, &shaderTextPtr, nullptr);
	glCompileShader(fs);
	// query information about the compilation (nothing if compilation went fine!)
	compileResult = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetShaderInfoLog(fs, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	//link shader program (connect vs and ps)
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, fs);
	glAttachShader(gShaderProgram, vs);
	glLinkProgram(gShaderProgram);

	// check once more, if the Vertex Shader and the Fragment Shader can be used
	// together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgram, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgram, vs);
	glDetachShader(gShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

}

void ShaderHandler::CreateFSShaders(GLuint* gShaderProgramFS)
{
	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	//create vertex shader "name" and store it in "vs"
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	// open .glsl file and put it in a string
	ifstream shaderFile("VertexShaderFS.glsl");
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	// glShaderSource requires a double pointer.
	// get the pointer to the c style string stored in the string object.
	const char* shaderTextPtr = shaderText.c_str();

	// ask GL to use this string a shader code source
	glShaderSource(vs, 1, &shaderTextPtr, nullptr);

	// try to compile this shader source.
	glCompileShader(vs);

	// check for compilation error
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		glGetShaderInfoLog(vs, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	// repeat process for Fragment Shader (or Pixel Shader)
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	shaderFile.open("FragmentFS.glsl");
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(fs, 1, &shaderTextPtr, nullptr);
	glCompileShader(fs);
	// query information about the compilation (nothing if compilation went fine!)
	compileResult = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetShaderInfoLog(fs, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	//link shader program (connect vs and ps)
	*gShaderProgramFS = glCreateProgram();
	glAttachShader(*gShaderProgramFS, fs);
	glAttachShader(*gShaderProgramFS, vs);
	glLinkProgram(*gShaderProgramFS);

	// check once more, if the Vertex Shader and the Fragment Shader can be used
	// together
	compileResult = GL_FALSE;
	glGetProgramiv(*gShaderProgramFS, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(*gShaderProgramFS, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(*gShaderProgramFS, vs);
	glDetachShader(*gShaderProgramFS, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void ShaderHandler::createVertexBuffer(std::vector<vertexPolygon> vertices)
{
	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttribute);
	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttribute);
	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBuffer);
	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertexPolygon), vertices.data(), GL_STATIC_DRAW);

	// query which "slot" corresponds to the input vertex_position in the Vertex Shader 
	// if this returns -1, it means there is a problem, and the program will likely crash.
	// examples: the name is different or missing in the shader
	GLint vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");
	if (vertexPos == -1)
		std::cout << "VertexPos is wrong" << std::endl;
	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(
		vertexPos,				// location in shader
		3,						// how many elements of type (see next argument)
		GL_FLOAT,				// type of each element
		GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
		sizeof(vertexPolygon), // distance between two vertices in memory (stride)
		BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	);

	GLint uvCoord = glGetAttribLocation(gShaderProgram, "vertex_uv");
	if (uvCoord == -1)
		std::cout << "uvCoord is wrong" << std::endl;

	glVertexAttribPointer(
		uvCoord,
		2,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 3)
	);

	GLint normals = glGetAttribLocation(gShaderProgram, "vertex_normal");
	if (uvCoord == -1)
		std::cout << "normal is wrong" << std::endl;

	glVertexAttribPointer(
		normals,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 5)
	);

	GLint tangent = glGetAttribLocation(gShaderProgram, "vertex_tangent");
	if (tangent == -1)
		std::cout << "Tangent is wrong" << std::endl;

	glVertexAttribPointer(
		tangent,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 8)
	);

	GLint biTangent = glGetAttribLocation(gShaderProgram, "vertex_bitangent");
	if (biTangent == -1)
		std::cout << "biTangent is wrong" << std::endl;

	glVertexAttribPointer(
		biTangent,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(vertexPolygon),
		BUFFER_OFFSET(sizeof(float) * 11)
	);
}

GLuint ShaderHandler::getVertexAttributes()
{
	return gVertexAttribute;
}

GLuint ShaderHandler::getShader()
{
	return gShaderProgram;
}