#pragma once

// ========================================
// シーンマネージャー
// ========================================


// ========================================
// ヘッダー
// ========================================
// シーンの基底クラス
#include "BaseSceneState.h"
// フェードマネージャー
#include "FadeManager.h"
// モジュール
#include "GameModule.h"
// 動的確保
#include <memory>


// ========================================
// クラス
// ========================================
class SceneManager
{
private:
	// 上の階層の抽象化モジュール
	GameModules* m_Modules = nullptr;
	// フェードマネージャー
	FadeManager m_FadeManager;

	// 現在のシーン
	std::unique_ptr<BaseSceneState> m_CurrentSceneState;


	// シーンの切り替え
	bool ChangeScene(SceneEventID event);

public:
	// コンストラクタ・デストラクタ
	SceneManager() = default;
	~SceneManager() = default;

	// シーンの初期化
	bool Init(GameModules* _modules);
	// シーンの更新 ゲームを終了するべき場合はfalseを返す
	bool Update(float time);
	// シーンの描画
	void Draw();
	// シーンの終了処理
	void Uninit();
};

