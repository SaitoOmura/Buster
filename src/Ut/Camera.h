#pragma once
#include "../GameLibrary/Utility/Singleton.h"
#include "../GameLibrary/Mathematics/Vector3.h"
#include "../Object/Player/Player.h"

#define Window_w 1280
#define Window_h 720
#define offset_x 2
#define offset_y -7
#define offset_z 7
#define offset_x2 -7
#define offset_y2 -2
#define offset_z2 10

enum class CameraType
{
	defo,
	side,
	up,
};

class Camera : public Singleton<Camera>
{
private:
	GameLib::Vector3 camera_pos;
	GameLib::Vector3 target_pos;
	GameLib::Vector3 camera_off;
	Player* player;
	GameLib::Vector3 playerPos;
	GameLib::Vector3 target_off;
	GameLib::Vector3 current_off;
	GameLib::Vector3 target_current;
	bool  initializedLook;
	CameraType camera_type;
	bool change;
	double time;
	
public:
	Camera();
	~Camera();

public:
	void Initialize();
	void Update(double delta_second);
	void Finalize();

	void SetPlayer(Player* p);

	const GameLib::Vector3 GetCameraPos() const;
	const GameLib::Vector3 GetTargetPos() const;

	void DeletePlayer();

	void ChangeCameraType(CameraType type);
	CameraType GetCamaeraType() const;
};

