#include "Enemy.h"
#include "EnemyBullet.h"
#include <GL/glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "../ObjectManager.h"
#include "../../Ut/ResourceManager.h"

size_t Enemy::count = 0;

size_t Enemy::GetCount()
{
	return count;
}

Enemy::Enemy()
{

}

Enemy::~Enemy()
{

}

void Enemy::Initialize()
{
	color[0] = 0.6f;
	color[1] = 0.6f;
	color[2] = 0.1f;
	color[3] = 1.0f;
	state = State::move;
	collision.type = ObjectType::enemy;
	collision.hit_type.push_back(ObjectType::player);
	collision.hit_type.push_back(ObjectType::ground);
	om = ObjectManager::Get();
	walk_speed = 2.0;
	HP = 3;
	ResourceManager* rm = ResourceManager::Get();
	cycle = 0.0;
	damage_second = 0.0;
	__super::Initialize();
}

void Enemy::Update(double delta_second)
{
	if (damage_second > 0.0)
	{
		damage_second -= delta_second;
	}
	if (state != State::damage)
	{
		if (e_type != EnemyType::shot && e_type != help)
		{
			velocity.x = walk_speed * cos((dir + 90) / 180.0 * M_PI);
			velocity.y = walk_speed * sin((dir + 90) / 180.0 * M_PI);

			if (location.x + 1.0f >= area_point1.x)
			{
				dir = 90;
			}
			else if (location.x - 1.0f <= area_point2.x)
			{
				dir = 270;
			}
		}
		else if(cycle <= 0.0f && e_type == EnemyType::shot)
		{
			om->CreateObject<EnemyBullet>(GameLib::Vector3(location.x, location.y, location.z + (size.z / 2)), GameLib::Vector3(0.3, 0.3, 0.3))->SetDir(dir);
			cycle = 0.8;
		}
				
	}
	else
	{
		if (damage_second <= 0.0) damage_second = 1.1;
		ResourceManager* rm = ResourceManager::Get();
		velocity.x *= DECELERATION_RATE;
		velocity.y *= DECELERATION_RATE;
		if (fabs(velocity.x) < 0.01f && fabs(velocity.y) < 0.01f)
		{
			state = State::move;
		}
	}
	velocity.z -= GRAVITY * delta_second;

	location += velocity * delta_second;

	if (cycle >= 0.0f)
	{
		cycle -= delta_second;
	}

	if (location.z <= LIMIT) HP = 0;
}

void Enemy::Draw()
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

void Enemy::Finalize()
{
	if (e_type == EnemyType::final)
	{
		count--;
	}
}

void Enemy::EnemyType(int Type)
{
	switch (Type)
	{
	case 1:
		e_type = EnemyType::walk;
		area_point1 = GameLib::Vector3(72.0f, 40.0f, 6.5f);
		area_point2 = GameLib::Vector3(-10.0f, 25.0f, 6.5f);
		dir = 90;
		break;
	case 2:
		e_type = EnemyType::shot;
		dir = 180;
		break;
	case 3:
		if (count % 2 == 0)
		{
			e_type = EnemyType::shot;
			dir = 90;
			count++;
		}
		else
		{
			e_type = EnemyType::final;
			dir = 90;
			area_point1 = GameLib::Vector3(72.0f, 40.0f, 6.5f);
			area_point2 = GameLib::Vector3(50.0f, 25.0f, 6.5f);
			count++;
		}
		break;
	case 4:
		e_type = EnemyType::help;
		dir = 90;
	default:
		break;
	}
}