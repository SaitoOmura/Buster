#pragma once

#include <vector>
#include <string>

#include "../Object/ObjectManager.h"
#include "../Ut/InputManager.h"
#include "../Ut/Camera.h"

enum class SceneType
{
	title,
	ingame,
	restart,
	result,
	end,
	help
};

class SceneBase
{
protected:
	std::vector<int> sounds;
	bool player_camera;

public:
	// コンストラクタ
	SceneBase()
	{

	}
	// デストラクタ
	virtual ~SceneBase()
	{
		// 解放忘れ防止
		Finalize();
	}

public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize()
	{

	}

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="delta_second">1フレーム当たりの時間</param>
	/// <returns>次のシーンタイプ情報</returns>
	virtual SceneType Update(double deltaTime)
	{


		// 現在のシーン情報を返却する
		return GetNowSceneType();
	}

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw()
	{

	}

	/// <summary>
	/// 終了時処理
	/// </summary>
	virtual void Finalize()
	{
		// オブジェクトマネージャーの情報を取得

	}

	/// <summary>
	/// 現在のシーンタイプ取得処理
	/// </summary>
	/// <returns>現在のシーンタイプ情報</returns>
	virtual const SceneType GetNowSceneType() const = 0;

	/// <summary>
	/// 固定カメラを使うかどうか
	/// </summary>
	/// <returns>真偽</returns>
	virtual const bool GetSceneCamera()
	{
		return player_camera;
	}
};