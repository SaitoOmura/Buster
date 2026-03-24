#include "HelpScene.h"
#include "../../Ut/ResourceManager.h"
#include "../SceneManager.h"

HelpScene::HelpScene()
{

}

HelpScene::~HelpScene()
{

}

void HelpScene::Initialize()
{
	camera->DeletePlayer();
	for (Object* target : om->GetObjectsList())
	{
		om->DestroyObject(target);
	}
	om->CheckDestroyObject();
	camera->Initialize();
	om->CreateObject<ground>(GameLib::Vector3(0.0, 0.0, 0.0), GameLib::Vector3(1.0, 1.0, 1.0))->SetHelp(true);
	player = om->CreateObject<Player>(GameLib::Vector3(0.0, 0.0, 2.0), GameLib::Vector3(1.0, 1.0, 1.0));
	om->CreateObject<Goal>(GameLib::Vector3(25.0, 0.0, 1.0), GameLib::Vector3(1.0, 1.0, 2.0));
	camera->SetPlayer(player);
	sounds.push_back(rm->GetSounds("Resource/Sounds/BGM/InGame.mp3", true));
	rm->PlayBackGroundMusic(sounds[0], -1);
	images.push_back(rm->GetImages("Resource/Images/UI/help_move.png")[0]);
	images.push_back(rm->GetImages("Resource/Images/UI/help_jump.png")[0]);
	images.push_back(rm->GetImages("Resource/Images/UI/help_attack.png")[0]);
	images.push_back(rm->GetImages("Resource/Images/UI/goal.png")[0]);
	player_camera = false;
	draw_help = false;
	draw_help_time = 0.0;
}

SceneType HelpScene::Update(double deltaTime)
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
			if (dynamic_cast<ground*>(target)->CheckPlayerEnterArea(player, target->GetEnemyArea(HELP_AREA1)))
			{
				image = images[0];
				draw_help = true;
				draw_help_time = 2.0;
				dynamic_cast<ground*>(target)->ClearArea(HELP_AREA1);
			}
			else if (dynamic_cast<ground*>(target)->CheckPlayerEnterArea(player, target->GetEnemyArea(HELP_AREA2)))
			{
				image = images[1];
				draw_help = true;
				draw_help_time = 2.0;
				dynamic_cast<ground*>(target)->ClearArea(HELP_AREA2);
			}
			else if (dynamic_cast<ground*>(target)->CheckPlayerEnterArea(player, target->GetEnemyArea(HELP_AREA3)))
			{
				image = images[2];
				draw_help = true;
				draw_help_time = 2.0;
				om->CreateObject<Enemy>(GameLib::Vector3(20.0, 0.0, 1.0), GameLib::Vector3(1.0, 1.0, 1.0))->EnemyType(HELP_ENEMY);
				dynamic_cast<ground*>(target)->ClearArea(HELP_AREA3);
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
				image = images[3];
				draw_help = true;
				draw_help_time = 1.0;
			}
		}

		if (target->GetHP() <= 0)
		{
			Collision tc = target->GetCollision();
			om->DestroyObject(target);
		}
		target->Update(deltaTime);
	}
	if (player->GetLocation().z <= LIMIT)
	{
		player->SetLocation(GameLib::Vector3(0.0, 0.0, 1.0));
	}
	if (draw_help)
	{
		draw_help_time -= deltaTime;
		if (draw_help_time <= 0.0)
		{
			draw_help_time = 0.0;
			draw_help = false;
			if (player->GetChange())
			{
				return SceneType::title;
			}
		}
	}
	return SceneType::help;
}

void HelpScene::Draw()
{
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
		if (obj->GetCollision().type == ObjectType::ground)
			obj->Draw();
	}
	for (Object* obj : om->GetObjectsList())
	{

		if (obj->GetCollision().type != ObjectType::ground)
		{
			obj->Draw();
			ground_z = obj->FindGroundZFromFBX(ground_triangles);
			obj->DrawShadow(ground_z);
		}
	}
	if (draw_help)
	{
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_FALSE);


		glPushMatrix();
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glTranslatef(player->GetLocation().x, player->GetLocation().y, player->GetLocation().z + DRAW_HELP_Z_SHIFT);
		DrawBillboardQuad(image, 1.0f);
		glPopMatrix();
		glDepthMask(GL_TRUE);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
	}
}

void HelpScene::Finalize()
{
    ResourceManager* rm = ResourceManager::Get();
    rm->StopBackGroundMusic();
    InputManager::ResetBool();
}

// 現在のシーンタイプ情報を取得する
const SceneType HelpScene::GetNowSceneType() const
{
    return SceneType::help;
}

void HelpScene::DrawBillboardQuad(GLuint tex, float size)
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