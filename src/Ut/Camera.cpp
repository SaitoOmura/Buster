#include "Camera.h"
#include <iostream>
#include <math.h>

Camera::Camera()
{

}

Camera::~Camera()
{

}

void Camera::Initialize()
{
	camera_pos = GameLib::Vector3(offset_x, offset_y, offset_z);
	target_pos = GameLib::Vector3(2.0, -1.0, 0.0);

    camera_type = CameraType::defo;
    change = false;
}

void Camera::Update(double delta_second)
{
    if(player)
   playerPos = player->GetLocation();

    switch (camera_type) 
    { 
    case CameraType::defo: 
    case CameraType::up: 
        target_off = GameLib::Vector3(offset_x, 0.0f, 1.5f);
        target_pos = GameLib::Vector3(playerPos.x + offset_x, playerPos.y, playerPos.z + 1.5f); 
        break; 
    case CameraType::side: 
        target_off = GameLib::Vector3(0.0f, offset_y2, 01.5f);
        if(!change)
        target_pos = GameLib::Vector3(playerPos.x, playerPos.y + offset_y2, playerPos.z + 1.5f); 
        break; 
    } 

    switch (camera_type)
    {
    case CameraType::defo:
        camera_off = GameLib::Vector3(offset_x, offset_y, offset_z);
        break;
    case CameraType::side:
        camera_off = GameLib::Vector3(offset_x2, offset_y2, offset_z);
        break;
    case CameraType::up:
        camera_off = GameLib::Vector3(offset_x, offset_y, offset_z2 );
        break;
    }

    // オフセットを補間

    float posSmooth = change ? 0.02f * delta_second * 60.0f
        : 0.20f * delta_second * 60.0f;

    if (posSmooth > 1.0f) posSmooth = 1.0f;

    current_off = current_off * (1.0f - posSmooth)
        + camera_off * posSmooth;

    target_current = target_current * (1.0f - posSmooth)
        + target_off * posSmooth;

    camera_pos = playerPos + current_off;
    target_pos = playerPos + target_current;

    //切替終了
    if (change)
    {
        if (fabs(current_off.x - camera_off.x) <= 0.1 && fabs(current_off.y - camera_off.y) <= 0.1f && fabs(current_off.z - camera_off.z) <= 0.1f) 
         change = false;
    }
}

void Camera::Finalize()
{

}

void Camera::SetPlayer(Player* p)
{
	player = p;
}

const GameLib::Vector3 Camera::GetCameraPos() const
{
	return camera_pos;
}

const GameLib::Vector3 Camera::GetTargetPos() const
{
	return target_pos;
}

void Camera::DeletePlayer()
{
    player = nullptr;
}

void Camera::ChangeCameraType(CameraType type)
{
    camera_type = type;
    change = true;
    time = 2.0;
    if (camera_type == CameraType::side)
    {
        player->SetChange(true);
    }
    else if (camera_type == CameraType::defo)
    {
        player->SetChange(false);
    }
}

CameraType Camera::GetCamaeraType() const
{
    return camera_type;
}