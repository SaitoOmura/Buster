#pragma once
#include "../Object.h"
#include "../../Ut/InputManager.h"
#include "../../Ut/ResourceManager.h"
#include <vector>

class Bullet : public Object
{
private:
	double life_time;
	double walk_speed;
	std::vector<GLuint> images;
	GameLib::Vector3 draw_loc;
	class Player* player;
	GLuint image;
	double anim_time;
	int anim_num;
	bool hit;
	ResourceManager* rm;

public:
	Bullet();
	~Bullet();

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

	/// <summary>
	/// プレイヤーのむいている方向をもとに飛んでいく方向を指定する
	/// </summary>
	/// <param name="direction">方向</param>
	void SetDir(double direction);

	/// <summary>
	/// HP減算処理
	/// </summary>
	virtual void DecHP() override;

	void SetPlayer(class Player* obj);

private:
	void CreateSphere();

	void Animation_Hit(double delta_second);

	/// <summary>
	/// エフェクト描画用の四角を作る
	/// </summary>
	/// <param name="tex">描画する画像ハンドル</param>
	/// <param name="size">大きさ</param>
	void DrawBillboardQuad(GLuint tex, float size);
	
};

