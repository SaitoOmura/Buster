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

#define ENEMYAREA_1 (1)
#define ENEMYAREA_2 (2)
#define ENEMYAREA_3 (3)
#define IMAGE_SIZE_WEIGHT (1920)
#define IMAGE_SIZE_HIGH (1080)
#define Z_SHIFT (2.5f)

class InGameScene : public SceneBase
{
private:
    Player* player;
    Camera* camera = Camera::Get();
    ObjectManager* om = ObjectManager::Get();
    ResourceManager* rm = ResourceManager::Get();
    double deltaTime = 0.0;
    std::vector<GLuint> images;
    bool draw_goal;
    double draw_goal_time;

public:
    InGameScene();
    ~InGameScene();


    virtual void Initialize() override;

    virtual SceneType Update(double deltaTime) override;

    virtual void Draw() override;

    virtual void Finalize() override;

    /// <summary>
/// 現在のシーンタイプ情報を取得する
/// </summary>
/// <returns>現在のシーンタイプ</returns>
    virtual const SceneType GetNowSceneType() const override;

private:
    /// <summary>
/// エフェクト描画用の四角を作る
/// </summary>
/// <param name="tex">描画する画像ハンドル</param>
/// <param name="size">大きさ</param>
    void DrawBillboardQuad(GLuint tex, float size);
};

