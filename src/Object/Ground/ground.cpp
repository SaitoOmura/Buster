#include "ground.h"

ground::ground()
{

}

ground::~ground()
{
	
}

void ground::Initialize()
{
	color[0] = 0.5f;
	color[1] = 0.5f;
	color[2] = 0.5f;
	color[3] = 0.1f;
	collision.type = ObjectType::ground;
	HP = 1;
}

void ground::Update(double delta_second)
{

}

void ground::Draw()
{
	__super::Draw();
	
}

void ground::Finalize()
{

}

bool ground::CheckPlayerEnterArea(Object* player, std::vector<Vertex> area)
{
	Collision col;
	if (area.empty()) return false;
	for (auto& tri : area)
	{
		if (col.CheckInArea(player, tri))
		{
			return true; // “–‚½‚Á‚½
		}
	}
	return false;
}

bool ground::CheckPlayerCameraChange(Object* player)
{
	Collision col;
	if (camera.empty()) return false;
	for (auto& tri : camera)
	{
		if (col.CheckInArea(player, tri))
		{
			return true; // “–‚½‚Á‚½
		}
	}
	return false;
}

void ground::ClearArea(int num)
{
	switch (num)
	{
	case 1:
		enemy.clear();
		break;
	case 2:
		enemy2.clear();
		break;
	case 3:
		enemy3.clear();
		break;
	}
}

void ground::SetHelp(bool torf)
{
	this_help = torf;
	__super::Initialize();
}