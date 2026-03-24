#pragma once
#include "../SceneBase.h"

class EndScene : public SceneBase
{
private:
    double time;

public:
    EndScene();
    ~EndScene();


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
    void render_string(float  x, float  y, const  char* str);
};

