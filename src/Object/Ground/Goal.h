#pragma once
#include "../Object.h"

class Goal : public Object
{
public:
	Goal();
	virtual ~Goal() override;

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

	/// <summary>
	/// クリア範囲に入ったか
	/// </summary>
	/// <param name="player">確認するオブジェクト</param>
	/// <returns></returns>
	bool CheckPlayerEnterCrea(Object* player);

};

