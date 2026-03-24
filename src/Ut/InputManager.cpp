#include "InputManager.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include "../Scene/SceneManager.h"

bool InputManager::front = false;
bool InputManager::back = false;
bool InputManager::right = false;
bool InputManager::left = false;
bool InputManager::jump = false;
bool InputManager::attack = false;
bool InputManager::reset = false;
bool InputManager::move = false;
bool InputManager::start = false;
bool InputManager::end = false;
double InputManager::dir = 0.0;
GameLib::Vector2 InputManager::velocity = GameLib::Vector2(0.0f,0.0f);

void InputManager::myKeyboard(unsigned char key, int x, int y)
{
    SceneManager* sm = SceneManager::Get();
    switch (sm->GetNowScene())
    { 
    case SceneType::title:
        switch (key)
        {
        case 0x1B: exit(0); break;
        default: start = true; break;
        }
        break;
    case SceneType::ingame:
        switch (key)
        {
        case 's': front = true; break;
        case 'w': back = true; break;
        case 'a': left = true; break;
        case 'd': right = true; break;
        case ' ': jump = true; break;
        case 'j': attack = true; break;
        case 'r': reset = true; break;
        case 0x1B: exit(0); break;
        }
        break;
    case SceneType::help:
        switch (key)
        {
        case 's': front = true; break;
        case 'w': back = true; break;
        case 'a': left = true; break;
        case 'd': right = true; break;
        case ' ': jump = true; break;
        case 'j': attack = true; break;
        case 'r': reset = true; break;
        case 0x1B: exit(0); break;
        }
        break;
    case SceneType::result:
        switch (key)
        {
        case 0x1B: exit(0); break;
        case ' ': reset = true; break;
        default:  end = true; break;
        }
        break;
    default:
        break;
    }

}

void InputManager::myKeyboardUp(unsigned char key, int x, int y)
{
    SceneManager* sm = SceneManager::Get();
    switch (sm->GetNowScene())
    {
    case SceneType::title:
        switch (key)
        {
        default: start = false; break;
        }
        break;
    case SceneType::ingame:
        switch (key)
        {
        case 's': front = false; break;
        case 'w': back = false; break;
        case 'a': left = false; break;
        case 'd': right = false; break;
        case ' ': jump = false; break;
        case 'j': attack = false; break;
        case 'r': reset = false; break;
        case 0x1B: exit(0); break;
        }
        break;
    case SceneType::help:
        switch (key)
        {
        case 's': front = false; break;
        case 'w': back = false; break;
        case 'a': left = false; break;
        case 'd': right = false; break;
        case ' ': jump = false; break;
        case 'j': attack = false; break;
        case 'r': reset = false; break;
        case 0x1B: exit(0); break;
        }
        break;
    case SceneType::result:
        switch (key)
        {
        case ' ': reset = false; break;
        default:  end = false; break;
        }
    default:
        break;
    }
}

bool InputManager::KeyboardCheck()
{
    if (front) return true;
    if (back) return true;
    if (right) return true;
    if (left) return true;
    return false;

}

void InputManager::JoyStick(unsigned int buttonMask, int x, int y, int z)
{
    SceneManager* sm = SceneManager::Get();
    jump = false;
    attack = false;
    move = false;
    start = false;
    reset = false;
    end = false;
    if (abs(x) > 300 || abs(y) > 300)
    {
        move = true;
    }
    if (move)
    {
        velocity = GameLib::Vector2(-x, y);
        velocity.Normalize();
        dir = (atan2(velocity.y, velocity.x) * 180 / M_PI);
        dir += 90;
    }
    switch (sm->GetNowScene())
    {
    case SceneType::title:
        switch (buttonMask)
        {
        case GLUT_JOYSTICK_BUTTON_A:
            start = true;  break;
        case BUTTON_START:
            end = true; break;
        }
        break;
    case SceneType::ingame:
        switch (buttonMask)
        {
        case GLUT_JOYSTICK_BUTTON_A:
            jump = true;
            break;
        case GLUT_JOYSTICK_BUTTON_B:
            attack = true;
            break;
        case GLUT_JOYSTICK_BUTTON_C:
            break;
        case GLUT_JOYSTICK_BUTTON_D:
            break;
        }
        break;
    case SceneType::result:
        switch (buttonMask)
        {
        case GLUT_JOYSTICK_BUTTON_A:
            start = true;
            break;
        }
        break;
    case SceneType::help:
        switch (buttonMask)
        {
        case GLUT_JOYSTICK_BUTTON_A:
            jump = true;
            break;
        case GLUT_JOYSTICK_BUTTON_B:
            attack = true;
            break;
        default:
            break;
        }
        break;
    }
  
}

Direction InputManager::GetDir()
{
    if (front)
    {
        if (left) return Direction::fleft;
        if (right) return Direction::fright;
        return Direction::front;
    }
    if (back)
    {
        if (left) return Direction::bleft;
        if (right) return Direction::bright;
        return Direction::back;
    }
    if (left) return Direction::left;
    if (right) return Direction::right;
    return Direction::center;
}

const double InputManager::GetDirection()
{
    return dir;
}

const bool InputManager::GetMoveFlag()
{
    return move;
}

const GameLib::Vector2 InputManager::GetVelocity()
{
    return velocity;
}

const void InputManager::ResetBool()
{
    front = false;
    back = false;
    right = false;
    left = false;
    jump = false;
    attack = false;
    reset = false;
    move = false;
    start = false;
    end = false;
}