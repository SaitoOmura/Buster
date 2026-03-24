#include "Collision.h"
#include "../Object/Object.h"

bool Collision::IsCheckHitTarget(ObjectType hit_object) const
{
	// 適用するオブジェクトタイプなら、true
	for (ObjectType type : hit_type)
	{
		if (type == hit_object)
		{
			return true;
		}
	}

	return false;
}

void Collision::HitCheckChar(Object* target, Object* partner)
{
	GameLib::Vector3 diff = target->GetLocation() - partner->GetLocation();
	GameLib::Vector3 tsize = target->GetSize() / 2;
	GameLib::Vector3 psize = partner->GetSize() / 2;

	float disX = (tsize.x + psize.x) - std::fabs(diff.x);
	float disY = (tsize.y + psize.y) - std::fabs(diff.y);
	float disZ = (tsize.z + psize.z) - std::fabs(diff.z);

	// 重なりが無いなら何もしない
	if (disX < 0 || disY < 0 || disZ < 0)
		return;

	target->SetVelocity(GameLib::Vector3(diff.x * 50, diff.y * 50, diff.z * 10));
	target->DecHP();
	partner->SetVelocity(GameLib::Vector3(-diff.x * 50, -diff.y * 50, -diff.z * 10));
	partner->DecHP();
}

bool Collision::CheckSphereTriangle(Object* obj, const Vertex& tri, GameLib::Vector3& pushOutDir)
{
	const float radius = obj->GetSize().z / 2;
    const GameLib::Vector3 sphereCenter = obj->GetLocation();

    // 三角形平面との距離（正なら法線方向）
    float d = GameLib::Vector3::Dot(tri.normal, sphereCenter - tri.v0);

    // 球が平面から離れすぎていたら衝突なし
    if (fabs(d) > radius) return false;

    // 球の中心を平面上に射影
    GameLib::Vector3 projected = sphereCenter - tri.normal * d;

    // 内部判定（バリセントリック座標）
    GameLib::Vector3 v0v1 = tri.v1 - tri.v0;
    GameLib::Vector3 v0v2 = tri.v2 - tri.v0;
    GameLib::Vector3 v0p  = projected - tri.v0;

    float dot00 = GameLib::Vector3::Dot(v0v2, v0v2);
    float dot01 = GameLib::Vector3::Dot(v0v2, v0v1);
    float dot02 = GameLib::Vector3::Dot(v0v2, v0p);
    float dot11 = GameLib::Vector3::Dot(v0v1, v0v1);
    float dot12 = GameLib::Vector3::Dot(v0v1, v0p);

    float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

    if (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f)
    {
       
        float penetration = radius - fabs(d);
	
		pushOutDir = tri.normal * penetration * (d > 0 ? 1.0f : -1.0f);

		
		if (tri.normal.z > 0.5f)
		{
			pushOutDir = tri.normal * penetration;
			obj->SetVelocity(GameLib::Vector3(obj->GetVelocity().x, obj->GetVelocity().y, 0.0));
		}

		return true;
    }

    return false;
}

bool Collision::CheckInArea(Object* obj, const Vertex& tri)
{
	const float radius = obj->GetSize().x / 2;
	const GameLib::Vector3 sphereCenter = obj->GetLocation();

	// 三角形平面との距離（正なら法線方向）
	float d = GameLib::Vector3::Dot(tri.normal, sphereCenter - tri.v0);

	// 球が平面から離れすぎていたら衝突なし
	if (fabs(d) > radius) return false;

	// 球の中心を平面上に射影
	GameLib::Vector3 projected = sphereCenter - tri.normal * d;

	// 内部判定（バリセントリック座標）
	GameLib::Vector3 v0v1 = tri.v1 - tri.v0;
	GameLib::Vector3 v0v2 = tri.v2 - tri.v0;
	GameLib::Vector3 v0p = projected - tri.v0;

	float dot00 = GameLib::Vector3::Dot(v0v2, v0v2);
	float dot01 = GameLib::Vector3::Dot(v0v2, v0v1);
	float dot02 = GameLib::Vector3::Dot(v0v2, v0p);
	float dot11 = GameLib::Vector3::Dot(v0v1, v0v1);
	float dot12 = GameLib::Vector3::Dot(v0v1, v0p);

	float invDenom = 1.0f / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	if (u >= 0.0f && v >= 0.0f && (u + v) <= 1.0f)
	{
		return true;
	}

	return false;
}

