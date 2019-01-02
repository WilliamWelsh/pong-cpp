#include "utilities.h"

// Draw text to the screen
void Utilities::drawText(float x, float y, std::string text, void *font)
{
	glRasterPos2f(x, y);
	glutBitmapString(font, (const unsigned char*)text.c_str()); // ToDo: static cast
}

// Draw rectangles (for the pongs)
void Utilities::drawRect(float x, float y, float width, float height)
{
	// A rectangle is 4 points, 4 vertices
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}