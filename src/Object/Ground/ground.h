#pragma once
#include "../Object.h"

class ground : public Object
{
public:
	ground();
	virtual ~ground() override;

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
	/// 敵生成エリアに入っているかどうか
	/// </summary>
	/// <param name="player">確認するおbじぇct</param>
	/// <param name="area">生成エリアの頂点情報</param>
	/// <returns></returns>
	bool CheckPlayerEnterArea(Object* player, std::vector<Vertex> area);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="player"></param>
	/// <returns></returns>
	bool CheckPlayerCameraChange(Object* player);

	/// <summary>
	/// 使用した判定エリアを削除する
	/// </summary>
	/// <param name="num">判定エリアの種類</param>
	void ClearArea(int num);

	/// <summary>
	/// ヘルプシーンかどうか
	/// </summary>
	/// <param name="torf">真偽</param>
	void SetHelp(bool torf);
};

