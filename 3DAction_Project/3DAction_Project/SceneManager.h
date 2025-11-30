#pragma once

// ========================================
// シーンマネージャー
// ========================================


// ========================================
// ヘッダー
// ========================================
// シーンの基底クラス
#include "BaseSceneState.h"
// 入力
#include "input.h"
// 描画マネージャー
#include "BaseDrawManager.h"
// カーソル
#include "CursorController.h"
// 動的確保
#include <memory>


// ========================================
// クラス
// ========================================
class SceneManager
{
private:
	// マネージャー
	BaseDrawManager* m_DrawManager = nullptr;
	Input* m_Input = nullptr;
	CursorController* m_CursorController = nullptr;

	// 現在のシーン
	std::unique_ptr<BaseSceneState> m_CurrentSceneState;

public:
	// シーンの初期化
	void Init(BaseDrawManager* _drawManager, Input* _input, CursorController* _cursor);
	// シーンの更新
	void Update(float time);
	// シーンの描画
	void Draw();
	// シーンの終了処理
	void Uninit();

	// シーンの切り替え
	void ChangeScene(SceneEvent event);
};

