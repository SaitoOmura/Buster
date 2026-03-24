#pragma once
#include <vector>
#include "../GameLibrary/Mathematics/Vector3.h"

enum class ObjectType
{
	player,
	ground,
	enemy,
	wepon,
	e_wepon,
	goal
};

struct Vertex {
	GameLib::Vector3 v0, v1, v2;
	GameLib::Vector3 normal;
	int materialID = 0;
};

class Object;

class Collision
{
public:
	ObjectType type;
	std::vector<ObjectType> hit_type;

public:
	//当たり判定があるかどうかの確認
	bool IsCheckHitTarget(ObjectType hit_object) const;

	//キャラクター同士の当たり判定
	void HitCheckChar(Object* target, Object* partner);

	//fbxで読み込んだ地面とキャラクターの当たり判定
	bool CheckSphereTriangle(Object* obj, const Vertex& tri, GameLib::Vector3& pushOutDir);

	bool CheckInArea(Object* obj, const Vertex& tri);

};

