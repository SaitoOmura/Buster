#pragma once
#include <GL/glut.h>
#include "../GameLibrary/Mathematics/Vector2.h"

#define BUTTON_START (128)
#define BUTTON_BACK (64)

enum class Direction
{
	center,
	front,
	fleft,
	left,
	bleft,
	fright,
	right,
	bright,
	back
};

class InputManager
{
private:
	static double dir;
	static bool move;
	static GameLib::Vector2 velocity;

public:
	static bool front;
	static bool back;
	static bool right;
	static bool left;
	static bool jump;
	static bool attack;
	static bool reset;
	static bool start;
	static bool end;


public:
	static void myKeyboard(unsigned char key, int x, int y);
	static void myKeyboardUp(unsigned char key, int x, int y);
	static bool KeyboardCheck();
	static void JoyStick(unsigned int buttonMask, int x, int y, int z);
	static Direction GetDir();
	static const double GetDirection();
	static const bool GetMoveFlag();
	static const GameLib::Vector2 GetVelocity();
	static const void ResetBool();
};

