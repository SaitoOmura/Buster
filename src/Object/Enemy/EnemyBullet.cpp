#include "EnemyBullet.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include "../../Ut/ResourceManager.h"

EnemyBullet::EnemyBullet()
{

}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Initialize()
{
	HP = 1;
	collision.type = ObjectType::e_wepon;
	collision.hit_type.push_back(ObjectType::player);
	collision.hit_type.push_back(ObjectType::ground);
	life_time = 3.0;
	walk_speed = 10.0;
	color[0] = 0.6f;
	color[1] = 0.6f;
	color[2] = 0.1f;
	color[3] = 1.0f;
	ResourceManager* rm = ResourceManager::Get();
	images = rm->GetImages("Resource/Images/Effect/hit_effect.png", 5, 5, 1, 32, 32);
	image = images[0];
}

void EnemyBullet::Update(double delta_second)
{
	velocity.x = walk_speed * cos((dir + 90.0) / 180.0 * M_PI);
	velocity.y = walk_speed * sin((dir + 90.0) / 180.0 * M_PI);
	life_time -= delta_second;
	location += velocity * delta_second;
	if (life_time <= 0.0) HP = 0;
}

void EnemyBullet::Draw()
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	CreateSphere();
}

void EnemyBullet::Finalize()
{

}

void EnemyBullet::SetDir(double direction)
{
	dir = direction;
}

void EnemyBullet::CreateSphere()
{
	glPushMatrix();
	glTranslated(location.x, location.y, location.z);
	glutSolidSphere(size.x, DIVISION_NUM, DIVISION_NUM);
	glPopMatrix();
}