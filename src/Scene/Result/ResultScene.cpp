#include "ResultScene.h"


void ResultScene::Initialize()
{
    rm = ResourceManager::Get();
    sounds.push_back(rm->GetSounds("Resource/Sounds/BGM/Result.mp3", true));
    sounds.push_back(rm->GetSounds("Resource/Sounds/SE/start.mp3", false));
    sounds.push_back(rm->GetSounds("Resource/Sounds/SE/cursor_move.mp3", false));
    sounds.push_back(rm->GetSounds("Resource/Sounds/SE/end.mp3", false));
    rm->PlayBackGroundMusic(sounds[0], -1);
    image = rm->GetImages("Resource/Images/Result_space.png")[0];
    images = rm->GetImages("Resource/Images/UI/select_2.png", 6, 6, 1, 150, 150);
    player_camera = true;
}

SceneType ResultScene::Update(double deltaTime)
{
    Anim_Select(deltaTime);
    if (InputManager::GetMoveFlag())
    {
        if (InputManager::GetDirection() <= 60 && InputManager::GetDirection() >= -60)
        {
            if (select_num >= 1)
            {
                rm->PlaySoundEffect(sounds[2], 0, true);
                select_num--;
            }

        }
        else if (InputManager::GetDirection() >= 120 && InputManager::GetDirection() <= 240)
        {
            if (select_num <= 0)
            {
                rm->PlaySoundEffect(sounds[2], 0, true);
                select_num++;
            }
        }
    }
    if (InputManager::start) {
        if (select_num == 0)
        {
            ResourceManager* rm = ResourceManager::Get();
            rm->PlaySoundEffect(sounds[1], 0, false);
            return SceneType::title;
        }
        else
        {
            ResourceManager* rm = ResourceManager::Get();
            rm->PlaySoundEffect(sounds[3], 0, false);
            return SceneType::end;
        }
    }
    return SceneType::result;
}

void ResultScene::Draw()
{

    ResourceManager* rm = ResourceManager::Get();
    rm->Begin2DDraw(Window_w, Window_h);
    rm->DrawTexture(image, 0, 0, Window_w, Window_h);
    rm->End2DDraw();
    glDisable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);


    glPushMatrix();
    glTranslatef(INIT_POSITION_X, 0, INIT_POSITION_Z + (select_num * POSITION_Z_SHIFT));
    DrawBillboardQuad(images[anim_num], 0.1f);
    glPopMatrix();
    glDepthMask(GL_TRUE);
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void ResultScene::Finalize()
{
    ResourceManager* rm = ResourceManager::Get();
    rm->StopBackGroundMusic();
    InputManager::ResetBool();
}

// 現在のシーンタイプ情報を取得する
const SceneType ResultScene::GetNowSceneType() const
{
    return SceneType::result;
}

void ResultScene::Anim_Select(double deltaTime)
{
    anim_time += deltaTime;
    if (anim_time >= 0.1)
    {
        anim_num++;
        if (anim_num >= images.size())
        {
            anim_num = 0;
        }
        anim_time = 0.0;
    }
}

void ResultScene::DrawBillboardQuad(GLuint tex, float size)
{
    GLfloat modelview[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, modelview);

    // Z-up用
    GameLib::Vector3 right(modelview[0], modelview[4], modelview[8]);
    GameLib::Vector3 up(modelview[1], modelview[5], modelview[9]);

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