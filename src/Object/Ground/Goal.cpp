#include "Goal.h"

Goal::Goal()
{

}

Goal::~Goal()
{

}

void Goal::Initialize()
{
	color[0] = 1.0f;
	color[1] = 1.0f;
	color[2] = 1.0f;
	color[3] = 1.0f;
	collision.type = ObjectType::goal;
	HP = 1;
	__super::Initialize();
}

void Goal::Update(double delta_second)
{

}

void Goal::Draw()
{
	__super::Draw();

}

void Goal::Finalize()
{

}

bool Goal::CheckPlayerEnterCrea(Object* player)
{
	GameLib::Vector3 diff = location - player->GetLocation();
	GameLib::Vector3 gsize = size / 2;
	GameLib::Vector3 psize = player->GetSize() / 2;

	float disX = (gsize.x + psize.x) - std::fabs(diff.x);
	float disY = (gsize.y + psize.y) - std::fabs(diff.y);
	float disZ = (gsize.z + psize.z) - std::fabs(diff.z);

	// èdÇ»ÇËÇ™ñ≥Ç¢Ç»ÇÁâΩÇ‡ÇµÇ»Ç¢
	if (disX < 0 || disY < 0 || disZ < 0)
		return false;

	return true;
}