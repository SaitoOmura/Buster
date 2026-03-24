#include <stdlib.h>
#include <stdio.h>
#define _USE_MATH_DEFINES
#pragma warning(disable : 4996)
#include <math.h>
#include <chrono>
#include <GL/glut.h>
#include "Ut/InputManager.h"
#include "Ut/Camera.h"
#include "Scene/SceneManager.h"
#include "Ut/ResourceManager.h"
#include <fbxsdk.h>

#define	D_FAILURE		(-1)	// 失敗
#define MAX_DELTA (1.0 / 60.0)
Camera* camera = Camera::Get();
double deltaTime = 0.0;
auto lastTime = std::chrono::high_resolution_clock::now();
SceneManager* gScene = SceneManager::Get();

void myDisplay()
{
	GameLib::Vector3 c_pos = camera->GetCameraPos();
	GameLib::Vector3 t_pos = camera->GetTargetPos();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (double)Window_w / (double)Window_h, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (gScene->GetSceneCamera())
	{
		gluLookAt(0.0, 2.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
	}
	else
	{
		gluLookAt(c_pos.x, c_pos.y, c_pos.z, t_pos.x, t_pos.y, t_pos.z, 0.0, 0.0, 1.0);
	}
	glClearColor(0.2, 0.2, 0.2, 1.0);
	gScene->Draw();
	glutSwapBuffers();

}

void Reset()
{

	gScene->ChangeScene(SceneType::restart);

	lastTime = std::chrono::high_resolution_clock::now();
}

void myTimer()
{
	auto currentTime = std::chrono::high_resolution_clock::now();
	deltaTime = std::chrono::duration<double>(currentTime - lastTime).count();
	lastTime = currentTime;
	deltaTime = std::min(deltaTime, MAX_DELTA);

	if (InputManager::reset && gScene->GetNowScene() == SceneType::ingame)
	{
		Reset();
		return;
	}

	gScene->Update(deltaTime);
	
	glutPostRedisplay();
}

void myReshape(int width, int height)
{
	glViewport(0, 0, Window_w, Window_h);
}

void myInit(char* progname)
{
	ResourceManager* rm = ResourceManager::Get();
	rm->SoundInit();
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(Window_w, Window_h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(progname);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	myInit(argv[0]);
	//glutKeyboardFunc(InputManager::myKeyboard);
	//glutKeyboardUpFunc(InputManager::myKeyboardUp);
	glutJoystickFunc(InputManager::JoyStick, 100);
	glutIdleFunc(myTimer);
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	gScene->Initialize();
	glutMainLoop();
	return 0;
}