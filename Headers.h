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
#include <gl/GL.h>

#include "stb_image.h"

#include "mehImporterLib/Loader.h"

// Include FreeType for fonts and text
#include <ft2build.h>
#include FT_FREETYPE_H

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

#define WIDTH 1920
#define HEIGHT 1080
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

#define nr_P_LIGHTS 6

using namespace std;

// Should be renamed to just vertex or similar since this is only one vertex not a polygon
struct vertexPolygon
{
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normals;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};