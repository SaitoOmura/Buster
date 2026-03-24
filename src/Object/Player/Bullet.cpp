#include "Bullet.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <GL/glut.h>
#include "../../Ut/Camera.h"
#include "Player.h"

Bullet::Bullet()
{

}

Bullet::~Bullet()
{

}

void Bullet::Initialize()
{
	HP = 1;
	collision.type = ObjectType::wepon;
	collision.hit_type.push_back(ObjectType::enemy);
	collision.hit_type.push_back(ObjectType::ground);
	life_time = 1.0;
	walk_speed = 10.0;
	color[0] = 0.8f;
	color[1] = 0.2f;
	color[2] = 0.2f;
	color[3] = 1.0f;
	rm = ResourceManager::Get();
	sound = rm->GetSounds("Resource/Sounds/SE/hit_bullet.mp3", false);
	images = rm->GetImages("Resource/Images/Effect/hit_effect.png", 5, 5, 1, 32, 32);
	image = images[0];
	anim_time = 0.0;
	hit = false;
}

void Bullet::Update(double delta_second)
{
	if (!hit)
	{
		velocity.x = walk_speed * cos((dir + 90.0) / 180.0 * M_PI);
		velocity.y = walk_speed * sin((dir + 90.0) / 180.0 * M_PI);
		life_time -= delta_second;
		location += velocity * delta_second;

		if (life_time <= 0.0) HP = 0;
	}
	else
	{
		Animation_Hit(delta_second);
		
	}
}

void Bullet::Draw()
{
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	if (!hit)
	{
		CreateSphere();
	}
	else
	{

		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);


		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTranslatef(location.x, location.y, location.z);
		DrawBillboardQuad(image, 1.0f);
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);

	}
}

void Bullet::Finalize()
{

}

void Bullet::SetDir(double direction)
{
	dir = direction;
}

void Bullet::DecHP()
{
	hit = true;
	rm->PlaySoundEffect(sound, 0, false);
	draw_loc = location - player->GetLocation();
	collision.hit_type.clear();
}

void Bullet::SetPlayer(class Player*  obj)
{
	player = obj;
	dir = player->GetDirection();
}

void Bullet::CreateSphere()
{
	glPushMatrix();
	glTranslated(location.x, location.y, location.z);
	glutSolidSphere(size.x, DIVISION_NUM, DIVISION_NUM);
	glPopMatrix();
}

void Bullet::Animation_Hit(double delta_second)
{
	anim_time += delta_second;
	if (anim_time >= 0.1)
	{
		anim_num++;
		if (anim_num >= images.size())
		{
			anim_num = 0;
			HP = 0;
		}
		image = images[anim_num];
		anim_time = 0.0;
	}
}

void Bullet::DrawBillboardQuad(GLuint tex, float size)
{
	GLfloat modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	// Z-upóp
	GameLib::Vector3 right(modelview[0], modelview[4], modelview[8]);
	GameLib::Vector3 up(modelview[2], modelview[6], modelview[10]);

	float h = size * 1.0f;

	glBindTexture(GL_TEXTURE_2D, tex);

	glBegin(GL_QUADS);
	//ç∂â∫
	glTexCoord2f(0, 1);
	glVertex3f(-right.x * h - up.x * h,
		-right.y * h + up.y * h,
		-right.z * h - up.z * h);

	//âEâ∫
	glTexCoord2f(1, 1);
	glVertex3f(right.x * h + up.x * h,
		right.y * h + up.y * h,
		right.z * h - up.z * h);

	//âEè„
	glTexCoord2f(1, 0);
	glVertex3f(right.x * h + up.x * h,
		right.y * h - up.y * h,
		right.z * h + up.z * h);

	//ç∂è„
	glTexCoord2f(0, 0);
	glVertex3f(-right.x * h - up.x * h,
		-right.y * h - up.y * h,
		-right.z * h + up.z * h);
	glEnd();
}