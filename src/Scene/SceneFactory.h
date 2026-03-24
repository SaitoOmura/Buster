#pragma once
#include "SceneBase.h"
#include "Title/TitleScene.h"
#include "InGame/InGameScene.h"	
#include "Result/ResultScene.h"
#include "End/EndScene.h"
#include "Help/HelpScene.h"

class SceneFactory
{
public:

	/// <summary>
	/// シーン生成処理
	/// </summary>
	/// <param name="next_type">次のシーンタイプ</param>
	/// <returns>生成したシーン情報のポインタ</returns>
	static SceneBase* CreateScene(SceneType next_type)
	{
		// シーンタイプによって、生成するシーンを切り替える
		switch (next_type)
		{
			// タイトル
		case SceneType::title:
			return dynamic_cast<SceneBase*>(new TitleScene());
			// インゲーム、リスタート
		case SceneType::ingame:
		case SceneType::restart:
			return dynamic_cast<SceneBase*>(new InGameScene());
			// リザルト
		case SceneType::result:
			return dynamic_cast<SceneBase*>(new ResultScene());
			// エンド
		case SceneType::end:
			return dynamic_cast<SceneBase*>(new EndScene());
			//操作説明
		case SceneType::help:
			return dynamic_cast<SceneBase*>(new HelpScene());
		default:
			return nullptr;
		}
	}
};