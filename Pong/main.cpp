#include "utilities.h"
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup") // No console please

constexpr int VK_W = 0x57; // Define W key
constexpr int VK_S = 0x53; // Define S key
constexpr int VK_ENTER = 0x0D; // Define Enter key
constexpr int VK_EE1 = 0x31; // 1 Key
constexpr int VK_EE2 = 0x32; // 2 key

bool hasStarted = false; // Have the players pressed ENTER to start the game?

// Width and height of the application
int width = 500;
int height = 200;

// Scores
int score_left = 0;
int score_right = 0;

// Racket stuff
int racket_width = 7;
int racket_height = 45;
int racket_speed = 3;
float racket_left_x = 10.0f;
float racket_left_y = 75;
float racket_right_x = width - racket_width - 20;
float racket_right_y = 75;

// Ball, I mean Square, stuff
float ball_pos_x = width / 2;
float ball_pos_y = height / 2;
float ball_dir_x = -1.0f;
float ball_dir_y = 0.0f;

typedef void(*vFunctionCall)(int args);
vFunctionCall WhatToDraw;
vFunctionCall SecondInput; // Either AI or a second player

void DrawPlayerOneStartScreen()
{
	Utilities::drawText(width / 2 - 40, height / 4 * 3 + 15, "Pong!", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 100, height / 2 + 25, "READY PLAYER ONE", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 110, height / 2 - (height / 4) + 50, "'W' for Up, 'S' for Down", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 110, height / 2 - (height / 4) + 25, "Press ESCAPE at any time to quit.", GLUT_BITMAP_HELVETICA_12);
	Utilities::drawText(width / 2 - 65, height / 2 - (height / 4) - 25, "Press ENTER to begin.", GLUT_BITMAP_HELVETICA_10);
}

void DrawTwoPlayerStartScreen()
{
	Utilities::drawText(width / 2 - 40, height / 4 * 3 + 15, "Pong!", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 150, height / 2 + 25, "PLAYER ONE: 'W' for Up, 'S' for Down", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 245, height / 2 - (height / 4) + 50, "PLAYER TWO: 'Up Arrow' for Up, 'Down Arrow' for Down", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 110, height / 2 - (height / 4) + 25, "Press ESCAPE at any time to quit.", GLUT_BITMAP_HELVETICA_12);
	Utilities::drawText(width / 2 - 65, height / 2 - (height / 4) - 25, "Press ENTER to begin.", GLUT_BITMAP_HELVETICA_10);
}

void DrawMainMenu()
{
	Utilities::drawText(width / 2 - 40, height / 4 * 3 + 15, "Pong!", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 110, height / 2 + 25, "Press 1 for One Player", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 115, height / 2 - (height / 4) + 50, "Press 2 for Two Players", GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 2 - 110, height / 2 - (height / 4) + 25, "Press ESCAPE at any time to quit.", GLUT_BITMAP_HELVETICA_12);
	Utilities::drawText(width / 2 - 65, height / 2 - (height / 4) - 25, "Press 1 or 2 to begin.", GLUT_BITMAP_HELVETICA_10);
}

void DrawGame()
{
	// Draw rackets
	Utilities::drawRect(racket_left_x, racket_left_y, racket_width, racket_height);
	Utilities::drawRect(racket_right_x, racket_right_y, racket_width, racket_height);

	// Draw scores
	Utilities::drawText(width / 4, height - 35, std::to_string(score_left), GLUT_BITMAP_HELVETICA_18);
	Utilities::drawText(width / 4 + (width / 4 * 2), height - 35, std::to_string(score_right), GLUT_BITMAP_HELVETICA_18);

	// Draw ball
	Utilities::drawRect(ball_pos_x - 8 / 2, ball_pos_y - 8 / 2, 8, 8);
}

void Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	WhatToDraw(0);
	glutSwapBuffers();
}

void RealSecondPlayerInput()
{
	// Right player's controls
	if (GetAsyncKeyState(VK_UP) && racket_right_y < 143)
		racket_right_y += racket_speed;
	if (GetAsyncKeyState(VK_DOWN) && racket_right_y > 1)
		racket_right_y -= racket_speed;
}

void AIInput()
{
	//if (racket_right_y == ball_pos_y) return;
	if (racket_right_y + 22.5f < ball_pos_y && racket_right_y < 143)
		racket_right_y += racket_speed;
	else if (racket_right_y > ball_pos_y && racket_right_y > 1)
		racket_right_y -= racket_speed;
}

void HandleKeyboardInput()
{
	if (hasStarted)
	{
		// Left player's controls
		if (GetAsyncKeyState(VK_W) && racket_left_y < 143)
			racket_left_y += racket_speed;
		if (GetAsyncKeyState(VK_S) && racket_left_y > 1)
			racket_left_y -= racket_speed;

		SecondInput(0);
	}
	else
	{
		// Enter to start the game (after pressing 1 or 2 on the main screen)
		if (GetAsyncKeyState(VK_ENTER) && (WhatToDraw == (vFunctionCall)DrawPlayerOneStartScreen || WhatToDraw == (vFunctionCall)DrawTwoPlayerStartScreen))
		{
			hasStarted = true;
			WhatToDraw = (vFunctionCall)DrawGame;
		}

		// Pressing 1 on the main menu
		if (GetAsyncKeyState(VK_EE1))
		{
			WhatToDraw = (vFunctionCall)DrawPlayerOneStartScreen;
			SecondInput = (vFunctionCall)AIInput;
		}

		// Pressing 2 on the main menu
		if (GetAsyncKeyState(VK_EE2))
		{
			WhatToDraw = (vFunctionCall)DrawTwoPlayerStartScreen;
			SecondInput = (vFunctionCall)RealSecondPlayerInput;
		}
	}

	// Escape key to quit
	if (GetAsyncKeyState(VK_ESCAPE))
		exit(EXIT_SUCCESS);
}

void CalculateBallMovement()
{
	ball_pos_x += ball_dir_x * 2;
	ball_pos_y += ball_dir_y * 2;

	// Left racket collision
	if (ball_pos_x < racket_left_x + racket_width && ball_pos_x > racket_left_x && ball_pos_y < racket_left_y + racket_height && ball_pos_y > racket_left_y)
	{
		float f = ((ball_pos_y - racket_left_y) / racket_height) - 0.5f;
		ball_dir_x = fabs(ball_dir_x);
		ball_dir_y = f;
	}

	// Right racket collision
	if (ball_pos_x > racket_right_x && ball_pos_x < racket_right_x + racket_width && ball_pos_y < racket_right_y + racket_height && ball_pos_y > racket_right_y)
	{
		float f = ((ball_pos_y - racket_right_y) / racket_height) - 0.5f;
		ball_dir_x = -fabs(ball_dir_x);
		ball_dir_y = f;
	}

	// Went past left racket
	if (ball_pos_x < 0) {
		++score_right;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = fabs(ball_dir_x);
		ball_dir_y = 0;
	}

	// Went past right racket
	if (ball_pos_x > width)
	{
		++score_left;
		ball_pos_x = width / 2;
		ball_pos_y = height / 2;
		ball_dir_x = -fabs(ball_dir_x);
		ball_dir_y = 0;
	}

	// Top wall thing collision
	if (ball_pos_y > height)
		ball_dir_y = -fabs(ball_dir_y);

	// Bottom wall thing collision
	if (ball_pos_y < 0)
		ball_dir_y = fabs(ball_dir_y);
}

// Calculate stuff
void Update(int value)
{
	HandleKeyboardInput();
	if (hasStarted) CalculateBallMovement();
	glutTimerFunc(1000 / 60, Update, 0);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	// Initialize
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2); // Center our screen
	glutInitWindowSize(width, height);
	glutCreateWindow("Pong!");

	// Set up callback functions
	glutDisplayFunc(Draw);
	glutTimerFunc(1000 / 60, Update, 0);

	// Set it up for 2D
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0f, width, 0.0f, height, 0.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set window colors
	glColor3f(1.0f, 1.0f, 1.0f);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	WhatToDraw = (vFunctionCall)DrawMainMenu;
	glutMainLoop();
	return 0;
}