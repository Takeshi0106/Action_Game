#pragma once

// ========================================
// シーンマネージャー
// ========================================


// ========================================
// ヘッダー
// ========================================
// シーンの基底クラス
#include "BaseSceneState.h"
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
	// マネージャー
	GameModules* m_Modules = nullptr;

	// 現在のシーン
	std::unique_ptr<BaseSceneState> m_CurrentSceneState;

public:
	// シーンの初期化
	bool Init(GameModules* _modules);
	// シーンの更新
	bool Update(float time);
	// シーンの描画
	void Draw();
	// シーンの終了処理
	void Uninit();

	// シーンの切り替え
	bool ChangeScene(SceneEvent event);
};

