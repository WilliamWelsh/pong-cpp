#pragma once

#include "stdafx.h"
#include <string>
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "GL/freeglut.h"
#pragma comment(lib, "OpenGL32.lib")

class DrawUtils
{
public:
	// Draw text to the screen
	static void drawText(float x, float y, std::string text, void *font);

	// Draw rectangles (for the pongs)
	static void drawRect(float x, float y, float width, float height);
};