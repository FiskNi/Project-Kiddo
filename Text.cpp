#include "Text.h"


struct TextCharacter {
	GLuint TextureID;	//ID handle of the glyph texture
	glm::ivec2 Size;	// Size of glyph
	glm::ivec3 Bearing;	// Offset from baseline to left/top of glyph
	GLuint Advance;		// Offset to advance to next glyph
};

Text::Text() 
{
	// All this breaks, how lovely
	/*if (FT_Init_FreeType(&freeType)) {
		std::cout << "ERROR - FreeType: Could not initialise FreeType Library." << std::endl;
	}*/
	//LoadFont();
}

Text::~Text() 
{

}

void Text::LoadFont()
{
//	// this breaks yeehaw
//	if (FT_New_Face(freeType, "fonts/Arial/arial.ttf", 0, &font)) {
//		std::cout << "ERROR - FreeType: Failed to load font." << std::endl;
//	}
//	// Setting the font's width to 0 allows the width to dynamically resize based on the height
//	FT_Set_Pixel_Sizes(font, 0, 48);
//
//	if (FT_Load_Char(font, 'X', FT_LOAD_RENDER)) {
//		std::cout << "ERROR - FreeType: Failed to load Glyph." << std::endl;
//	}
//	// After this we can get the 8-bit greyscale bitmap
//	//font->glyph->bitmap;
}
