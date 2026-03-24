#pragma once
#include "../SceneBase.h"
#include "../../Ut/ResourceManager.h"

#define INIT_POSX (0.5)     //横初期値
#define INIT_POSZ (0.35)    //高さ初期値
#define POSZ_SHIFT (-0.45)  //縦に移動する量

class TitleScene : public SceneBase
{
private:
    GLuint image;
    std::vector<GLuint> images;
    double anim_time;
    int anim_num;
    int select_num;
    ResourceManager* rm;

public:
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
    /// 選択している物を表すUIのアニメーション
    /// </summary>
    /// <param name="deltaTime"></param>
    void Anim_Select(double deltaTime);

    /// <summary>
    /// UI用の四角を作る
    /// </summary>
    /// <param name="tex">描画する画像ハンドル</param>
    /// <param name="size">大きさ</param>
    void DrawBillboardQuad(GLuint tex, float size);
};
