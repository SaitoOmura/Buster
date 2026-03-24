#pragma once
#include "../Object.h"
#include "../../Ut/InputManager.h"
#include "../ObjectManager.h"

#define UI_SIZE (48)
#define MOVEPOWER (5.0)
#define JUMPPOWER (6.0f)

class Player : public Object
{
private:
	Direction direction;
	ObjectManager* om;
	float cycle;
	State old_state;
	double walk_speed;
	bool change;
	GameLib::Vector3 area_point1;
	GameLib::Vector3 area_point2;
	GameLib::Vector3 dirVec;
	double damage_second;
	bool jump_flag;
	std::vector<int> sounds;
	int image;

public:
	Player();
	~Player();

public:
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

	double GetDirection();

	/// <summary>
	/// 入力受付するかどうか
	/// </summary>
	/// <param name="truefalse"></param>
	void SetChange(bool truefalse);

	/// <summary>
	/// 停止中かどうか取得処理
	/// </summary>
	/// <returns>真偽</returns>
	bool GetChange();

	/// <summary>
	/// ヘルプシーンかどうか
	/// </summary>
	/// <param name="torf">真偽</param>
	void SetHelp(bool torf);

};

