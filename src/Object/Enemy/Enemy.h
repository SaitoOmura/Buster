#pragma once
#include "../Object.h"
#include "../ObjectManager.h"

enum EnemyType
{
	walk,
	shot,
	final,
	help
};

class Enemy :public Object
{
private:
	ObjectManager* om;
	static size_t count;
	float log_veloY;
	GameLib::Vector3 area_point1;
	GameLib::Vector3 area_point2;
	double walk_speed;
	double damage_second;
	float cycle;
	EnemyType e_type;

public:
	Enemy();
	~Enemy();

public:
	static size_t GetCount();
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレームあたりの時間</param>
	virtual void Update(double delta_second) override;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() override;
	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize() override;
	
	void EnemyType(int Type);
};

