#pragma once
#include "Headers.h"

// NOT FINISHED, AS CURRENTLY UNSURE IF WE CAN USE THIS OR NEED IT

class Text {
private:
	FT_Library freeType;
	FT_Face font;

	void LoadFont();
public:
	Text();
	~Text();

};