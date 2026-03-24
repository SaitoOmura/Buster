#include "InGameScene.h"
#include "../SceneManager.h"

InGameScene::InGameScene()
{

}

InGameScene::~InGameScene()
{
	Finalize();
}

void InGameScene::Initialize()
{
	camera->DeletePlayer();
	for (Object* target : om->GetObjectsList())
	{
		om->DestroyObject(target);
	}
	om->CheckDestroyObject();
	camera->Initialize();
	om->CreateObject<ground>(GameLib::Vector3(0.0, 0.0, 0.0), GameLib::Vector3(1.0, 1.0, 1.0))->SetHelp(false);
	player = om->CreateObject<Player>(GameLib::Vector3(0.0, 0.0, 4.0), GameLib::Vector3(1.0, 1.0, 1.0));
	om->CreateObject<Goal>(GameLib::Vector3(79.0, 31.5, 6.5), GameLib::Vector3(1.0, 1.0, 2.0));
	camera->SetPlayer(player);
	sounds.push_back(rm->GetSounds("Resource/Sounds/BGM/InGame.mp3",true));
	rm->PlayBackGroundMusic(sounds[0], -1);
	images.push_back(rm->GetImages("Resource/Images/BackGround_Image.png")[0]);
	images.push_back(rm->GetImages("Resource/Images/UI/HP.png")[0]);
	images.push_back(rm->GetImages("Resource/Images/UI/goal.png")[0]);
	player_camera = false;
}

SceneType InGameScene::Update(double deltaTime)
{

	om->CheckCreateObject();
	om->CheckDestroyObject();
	camera->Update(deltaTime);
	for (Object* target : om->GetObjectsList())
	{
		for (Object* partner : om->GetObjectsList())
		{
			if (target == partner)
			{
				continue;
			}

			Collision tc = target->GetCollision();
			Collision pc = partner->GetCollision();
			// 攻撃判定が有効か
			if (tc.IsCheckHitTarget(pc.type))
			{
				if (pc.type == ObjectType::player || pc.type == ObjectType::enemy) tc.HitCheckChar(target, partner);
				else  if (tc.type == ObjectType::player ||
					tc.type == ObjectType::enemy)
				{
					GameLib::Vector3 tloc = target->GetLocation();
					GameLib::Vector3 maxPush(0, 0, 0);
					float maxPenetration = 0.0f;
					GameLib::Vector3 push;
					for (auto& tri : partner->GetVertices())
					{

						if (tc.CheckSphereTriangle(target, tri, push))
						{
							float pen = push.Length();
							if (pen > maxPenetration)
							{
								maxPenetration = pen;
								maxPush = push;
							}

						}
					}
					if (maxPenetration > 0.0f)
					{
						tloc += maxPush;
					}
					target->SetLocation(tloc);
				}
				else
				{
					GameLib::Vector3 push;
					for (auto& tri : partner->GetVertices())
					{
						if (tc.CheckSphereTriangle(target, tri, push))
						{
							target->DecHP();

						}
					}
				}
			}

		}
		if (target->GetCollision().type == ObjectType::ground)
		{
			// groundにFBX areaがあると想定
			//敵生成エリアに入った際発生条件となるエリアを削除して敵を生成
			if (dynamic_cast<ground*>(target)->CheckPlayerEnterArea(player,target->GetEnemyArea(ENEMYAREA_1)))
			{
				om->CreateObject<Enemy>(GameLib::Vector3(15.0, 0.0, 2.0), GameLib::Vector3(1.0, 1.0, 1.0))->EnemyType(ENEMYAREA_1);
				dynamic_cast<ground*>(target)->ClearArea(ENEMYAREA_1);
			}
			else if (dynamic_cast<ground*>(target)->CheckPlayerEnterArea(player, target->GetEnemyArea(ENEMYAREA_2)))
			{
				om->CreateObject<Enemy>(GameLib::Vector3(36.0, 25.0, 5.0), GameLib::Vector3(1.0, 1.0, 1.0))->EnemyType(ENEMYAREA_2);
				dynamic_cast<ground*>(target)->ClearArea(ENEMYAREA_2);
			}
			else if (dynamic_cast<ground*>(target)->CheckPlayerEnterArea(player, target->GetEnemyArea(ENEMYAREA_3)))
			{
				om->CreateObject<Enemy>(GameLib::Vector3(70.0, 35.0, 6.5), GameLib::Vector3(1.0, 1.0, 1.0))->EnemyType(ENEMYAREA_3);
				om->CreateObject<Enemy>(GameLib::Vector3(70.0, 32.0, 6.5), GameLib::Vector3(1.0, 1.0, 1.0))->EnemyType(ENEMYAREA_3);
				om->CreateObject<Enemy>(GameLib::Vector3(70.0, 29.0, 6.5), GameLib::Vector3(1.0, 1.0, 1.0))->EnemyType(ENEMYAREA_3);
				dynamic_cast<ground*>(target)->ClearArea(ENEMYAREA_3);
			}
			if (dynamic_cast<ground*>(target)->CheckPlayerCameraChange(player))
			{
				if (camera->GetCamaeraType() != CameraType::side)
				camera->ChangeCameraType(CameraType::side);
			}
		}
		else if (target->GetCollision().type == ObjectType::goal)
		{
			if (dynamic_cast<Goal*>(target)->CheckPlayerEnterCrea(player) && !player->GetChange())
			{
				player->SetChange(true);
				draw_goal = true;
				draw_goal_time = 1.0;
				for (Object* target : om->GetObjectsList())
				{
					if (target->GetCollision().type == ObjectType::enemy || target->GetCollision().type == ObjectType::e_wepon)
					{
						om->DestroyObject(target);
					}
				}
			}
		}

		if (target->GetHP() <= 0)
		{
			Collision tc = target->GetCollision();
			if (tc.type == ObjectType::player)
			{
				return SceneType::result;
			}
			om->DestroyObject(target);
		}
		target->Update(deltaTime);
	}
	if (player->GetLocation().z <= LIMIT)
	{
		player->DecHP();
		player->SetLocation(GameLib::Vector3(0.0, 0.0, 1.0));
		if(camera->GetCamaeraType() != CameraType::defo)
		camera->ChangeCameraType(CameraType::defo);
	}
	if (camera->GetCamaeraType() == CameraType::up && Enemy::GetCount() == 0)
	{
		camera->ChangeCameraType(CameraType::defo);
	}
	if (draw_goal)
	{
		draw_goal_time -= deltaTime;
		if (draw_goal_time <= 0.0)
		{
			if (player->GetChange())
			{
				camera->DeletePlayer();
				return SceneType::result;
			}
		}
	}
	return SceneType::ingame;
}

void InGameScene::Draw()
{
	ResourceManager* rm = ResourceManager::Get();
	rm->Begin2DDraw(Window_w, Window_h);
	rm->DrawTexture(images[0], -player->GetLocation().x, player->GetLocation().z, IMAGE_SIZE_WEIGHT, IMAGE_SIZE_HIGH);
	rm->End2DDraw();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	float ground_z;
	std::vector<Vertex> ground_triangles;
	for (Object* obj : om->GetObjectsList())
	{
		if (obj->GetCollision().type == ObjectType::ground)
		{
			ground_triangles = obj->GetVertices();
			break;
		}
	}
	for (Object* obj : om->GetObjectsList())
	{

		if (obj->GetCollision().type != ObjectType::ground && obj->GetCollision().type != ObjectType::wepon)
		{
			obj->Draw();	
		}
	}
	for (Object* obj : om->GetObjectsList())
	{
		if (obj->GetCollision().type == ObjectType::ground)
			obj->Draw();
	}
	for (Object* obj : om->GetObjectsList())
	{
		if (obj->GetCollision().type == ObjectType::wepon)
		{
			obj->Draw();
			ground_z = obj->FindGroundZFromFBX(ground_triangles);
			obj->DrawShadow(ground_z);
		}
		else if (obj->GetCollision().type != ObjectType::ground)
		{
			ground_z = obj->FindGroundZFromFBX(ground_triangles);
			obj->DrawShadow(ground_z);
		}
	}
	if (draw_goal)
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glDepthMask(GL_FALSE);


		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTranslatef(player->GetLocation().x, player->GetLocation().y, player->GetLocation().z + Z_SHIFT);
		DrawBillboardQuad(images[2], 1.0f);
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}

	rm->Begin2DDraw(Window_w, Window_h);
	for (int i = 0; i < player->GetHP(); i++)
	{
		rm->DrawTexture(images[1], i * UI_SIZE, 0, UI_SIZE, UI_SIZE);
	}
	rm->End2DDraw();
	glRasterPos3f(player->GetLocation().x - 1.0f, player->GetLocation().y + 1.0f, player->GetLocation().z + 1.5f);

}

void InGameScene::Finalize()
{
	camera->DeletePlayer();
	for (Object* target : om->GetObjectsList())
	{
		om->DestroyObject(target);
	}
	om->CheckDestroyObject();
	ResourceManager* rm = ResourceManager::Get();
	rm->StopBackGroundMusic();
	InputManager::ResetBool();
}

// 現在のシーンタイプ情報を取得する
const SceneType InGameScene::GetNowSceneType() const
{
	return SceneType::ingame;
}

void InGameScene::DrawBillboardQuad(GLuint tex, float size)
{
	GLfloat modelview[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

	// Z-up用
	GameLib::Vector3 right(modelview[0], modelview[4], modelview[8]);
	GameLib::Vector3 up(modelview[2], modelview[6], modelview[10]);

	float h = size * 1.0f;

	glBindTexture(GL_TEXTURE_2D, tex);

	glBegin(GL_QUADS);
	//左下
	glTexCoord2f(0, 1);
	glVertex3f(-right.x * h - up.x * h,
		-right.y * h + up.y * h,
		-right.z * h - up.z * h);

	//右下
	glTexCoord2f(1, 1);
	glVertex3f(right.x * h + up.x * h,
		right.y * h + up.y * h,
		right.z * h - up.z * h);

	//右上
	glTexCoord2f(1, 0);
	glVertex3f(right.x * h + up.x * h,
		right.y * h - up.y * h,
		right.z * h + up.z * h);

	//左上
	glTexCoord2f(0, 0);
	glVertex3f(-right.x * h - up.x * h,
		-right.y * h - up.y * h,
		-right.z * h + up.z * h);
	glEnd();
}

