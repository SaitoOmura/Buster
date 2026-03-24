#pragma once
#include "../SceneBase.h"
#include "../../GameLibrary/Mathematics/Vector3.h"
#include "../../Object/ObjectManager.h"
#include "../../Ut/InputManager.h"
#include "../../GameLibrary/Mathematics/Matrix.h"
#include "../../GameLibrary/Mathematics/Plane.h"
#include "../../Object/Ground/ground.h"
#include "../../Object/Player/Player.h"
#include "../../Object/Enemy/Enemy.h"
#include "../../Object/Player/Bullet.h"
#include "../../Object/Ground/Goal.h"
#include "../../Ut/Camera.h"
#include "../../Ut/ResourceManager.h"

#define HELP_AREA1 (1)
#define HELP_AREA2 (2)
#define HELP_AREA3 (3)
#define HELP_ENEMY (4)
#define DRAW_HELP_Z_SHIFT (2.5f)    //説明をプレイヤーの頭上に表示させるためのずらし

class HelpScene : public SceneBase
{
private:
    GLuint image;
    Player* player;
    Camera* camera = Camera::Get();
    ObjectManager* om = ObjectManager::Get();
    ResourceManager* rm = ResourceManager::Get();
    double deltaTime = 0.0;
    std::vector<GLuint> images;
    bool draw_help;
    double draw_help_time;

public:
    HelpScene();
    ~HelpScene();

    virtual void Initialize() override;

    virtual SceneType Update(double deltaTime) override;

    virtual void Draw() override;

    virtual void Finalize() override;

    /// <summary>
    /// 現在のシーンタイプ情報を取得する
    /// </summary>
    /// <returns>現在のシーンタイプ</returns>
    virtual const SceneType GetNowSceneType() const override;

    void DrawBillboardQuad(GLuint tex, float size);

};

