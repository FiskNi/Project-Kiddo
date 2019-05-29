#pragma once
#include <windows.h>

#include <string>
#include <fstream>
#include <iostream>
#include <streambuf>
#include <vector>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

// read docs: https://github.com/g-truc/glm/blob/master/manual.md#section1
#include "glm/glm.hpp"
#include "glm/ext.hpp"

//#define GLM_ENABLE_EXPERIMENTAL
//#include <glm/gtx/quaternion.hpp>

#include <gl/GL.h>

#include "stb_image.h"

#include "mehImporterLib/Loader.h"

#include "irrKlang/include/irrKlang.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "irrKlang.lib") 

#define WIDTH 1920
#define HEIGHT 1080
#define MAXBONES 64
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))
#define POINTLIGHTS 6
#define NAME_SIZE 256

#define COLLECTEDCAP 9
using namespace std;

// Should be renamed to just vertex or similar since this is only one vertex not a polygon
struct vertexPolygon
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normals;
	glm::vec3 tangent;
	glm::vec3 bitangent;

	glm::vec4 weights;
	glm::ivec4 bones;
};

enum BOXTYPE 
{
	LIGHTWEIGHT = 0,
	EXPLOSIVE = 1,
	ICEBLOCK = 2,
	REGULAR = 3
};
enum ITEMTYPE 
{
	FEATHER = 0,
	BOMB = 1,
	SNOWFLAKE = 2,
	NONE = 3
};

enum GAMESTATE 
{
	PAUSED,
	PLAYING,
	MAINMENU
};