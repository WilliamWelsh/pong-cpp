#include <string>
#include <GL/freeglut.h>

class Utilities
{
public:
	// Draw text to the screen
	static void drawText(float x, float y, std::string text, void *font);

	// Draw rectangles (for the pongs)
	static void drawRect(float x, float y, float width, float height);
};