#include "Player.h"
#include "Bullet.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include "../../Ut/Camera.h"
#include "../../Ut/ResourceManager.h"
#include "../Enemy/Enemy.h"

Player::Player():
	om(nullptr),
	direction (Direction::center),
	cycle (0.0f),
	old_state(),
	walk_speed(0.0),
	change(false)
{

}

Player::~Player()
{

}

void Player::Initialize()
{
	state = State::move;
	dir = 0.0;
	color[0] = 0.8f;
	color[1] = 0.2f;
	color[2] = 0.2f;
	color[3] = 1.0f;
	collision.type = ObjectType::player;
	collision.hit_type.push_back(ObjectType::ground);
	collision.hit_type.push_back(ObjectType::enemy);
	HP = 3;
	om = ObjectManager::Get();
	direction = Direction::front;
	ResourceManager* rm = ResourceManager::Get();
	area_point1 = GameLib::Vector3(15.0f, 24.0f, 5.0f);
	area_point2 = GameLib::Vector3(25.5f, 11.0f, 5.0f);
	sounds.push_back(rm->GetSounds("Resource/Sounds/SE/jump.mp3",false));
	sounds.push_back(rm->GetSounds("Resource/Sounds/SE/hit.mp3", false));
	sounds.push_back(rm->GetSounds("Resource/Sounds/SE/shot.mp3", false));
	damage_second = 0.0;
	__super::Initialize();
}

void Player::Update(double delta_second)
{
	if (change)
	{
		velocity.x = 0.0;
		velocity.y = 0.0;
		velocity.z = 0.0;
	}
	if (velocity.z == 0.0f) jump_flag = false;	
	else jump_flag = true;
	ResourceManager* rm = ResourceManager::Get();
	if (direction != InputManager::GetDir() && InputManager::GetDir() != Direction::center)
		direction = InputManager::GetDir();

	if (damage_second > 0.0)
	{
		damage_second -= delta_second;
	}

	if (state != State::damage && !change)
	{
		velocity.x = walk_speed * cos((dir + 90.0) / 180.0 * M_PI);
		velocity.y = walk_speed * sin((dir + 90.0) / 180.0 * M_PI);
		walk_speed = 0;
		if (this_help)
		{
			HP = 3;
		}
	}

	if (InputManager::KeyboardCheck() && !change)
	{
		walk_speed = MOVEPOWER;
		switch (InputManager::GetDir())
		{
		case Direction::front: dir = 180;break;
		case Direction::fleft: dir = 135;break;
		case Direction::left: dir = 90;break;
		case Direction::bleft: dir = 45;break;
		case Direction::fright: dir = 225;break;
		case Direction::right: dir = 270;break;
		case Direction::bright: dir = 315;break;
		case Direction::back: dir = 0;break;
		}
	}
	else if (InputManager::GetMoveFlag() && !change)
	{
		walk_speed = MOVEPOWER;
		dir = InputManager::GetDirection();
	}
	switch (state)
	{
	case State::move:
		if (old_state != state) old_state = state;

		if (InputManager::jump && !jump_flag)
		{
			rm->PlaySoundEffect(sounds[0], 0, false);
			velocity.z = JUMPPOWER;
		}
		if (InputManager::attack && cycle <= 0.0f)
		{
			state = State::attack;
		}

		break;
	case State::damage:
		if (old_state != state)
		{
			old_state = state;
			rm->PlaySoundEffect(sounds[1], 0, false);
		}
		if (damage_second <= 0.0) damage_second = 1.1;
		velocity.x *= DECELERATION_RATE;
		velocity.y *= DECELERATION_RATE;
		if (fabs(velocity.x) < 0.01f && fabs(velocity.y) < 0.01f)
		{
			state = State::move;
		}
		break;
	case State::deth:
		break;
	case State::attack:
		if (InputManager::jump && !jump_flag)
		{
			rm->PlaySoundEffect(sounds[0], 0, false);
			velocity.z = JUMPPOWER;
			jump_flag = true;
		}
		if (old_state != state)
		{
			rm->PlaySoundEffect(sounds[2], 0, false);
			om->CreateObject<Bullet>(GameLib::Vector3(location.x, location.y, location.z + (size.z / 2)), GameLib::Vector3(0.3, 0.3, 0.3))->SetPlayer(this);
			old_state = state;
			cycle = 0.6;
		}
		if (cycle <= 0)
		{
			state = State::move;
		}
		break;
	default:
		break;
	}
	velocity.z -= GRAVITY * delta_second;
	location += velocity  * delta_second;

	if (cycle > 0)
	{
		cycle -= delta_second;
	}
	else
	{
		cycle = 0;
	}
}

void Player::Draw()
{
	if (damage_second > 0.0)
	{
		if (fmod(damage_second, 0.2) >= 0.1)
		__super::Draw();
	}
	else
	{
		__super::Draw();
	}	
}

void Player::Finalize()
{
	
}


double Player::GetDirection()
{
	return dir;
}

void Player::SetChange(bool truefalse)
{
	change = truefalse;
}

bool Player::GetChange()
{
	return change;
}

void Player::SetHelp(bool torf)
{
	this_help = torf;
}