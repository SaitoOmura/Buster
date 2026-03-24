#include "EndScene.h"
#include <GL/freeglut.h>

EndScene::EndScene()
{

}

EndScene::~EndScene()
{

}

void EndScene::Initialize()
{
    time = 0.0;
    player_camera = true;
}

SceneType EndScene::Update(double deltaTime)
{
    time += deltaTime;
    if (time >= 2.0)
    {
        glutLeaveMainLoop();
    }
    return SceneType::end;
}

void EndScene::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    render_string(0.3, 0.0, "EndScene");
}

void EndScene::Finalize()
{

}

// 現在のシーンタイプ情報を取得する
const SceneType EndScene::GetNowSceneType() const
{
    return SceneType::end;
}

void  EndScene::render_string(float  x, float  y, const  char* str) {
    glRasterPos2f(x, y);

    const  char* c = str;
    while (*c) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c++);
    }
}