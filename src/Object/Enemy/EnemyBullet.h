#pragma once
#pragma once
#include "../Object.h"
#include "../../Ut/InputManager.h"
#include <vector>

class EnemyBullet : public Object
{
private:
	double life_time;
	double walk_speed;
	std::vector<GLuint> images;
	GLuint image;

public:
	EnemyBullet();
	~EnemyBullet();

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

	void SetDir(double direction);

private:
	void CreateSphere();
};

