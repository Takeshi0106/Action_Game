#pragma once

// ========================================
// シーンの基底クラス
// ========================================


// ========================================
// ヘッダー
// ========================================
// 入力
#include "input.h"
// 描画マネージャー
#include "BaseDrawManager.h"
// カーソル
#include "CursorController.h"
// シーンイベント
#include "SceneEvent.h"


// ========================================
// クラス
// ========================================
class BaseSceneState
{
protected:
	// マネージャー
	BaseDrawManager* m_DrawManager = nullptr;
	Input* m_Input = nullptr;
	CursorController* m_CursorController = nullptr;

	// シーンイベント
	SceneEvent m_SceneEvent = SCENE_EVENT_NONE;

	// 派生初期化
	virtual bool DerivativeInit() = 0;

public:
	// コンストラクタ・デストラクタ
	BaseSceneState() = default;
	virtual ~BaseSceneState() = default;
	
	// シーンの初期化
	bool Init(BaseDrawManager* drawManager, Input* input, CursorController* cursorController) {
		// 各情報初期化
		m_DrawManager = drawManager;
		m_Input = input;
		m_CursorController = cursorController;

		// 派生クラスの初期化
		return DerivativeInit();
	}

	// シーンの更新
	virtual void Update(float _deltaTime) = 0;
	// シーンの描画
	virtual void Draw() = 0;
	// シーンの終了処理
	virtual void Uninit() = 0;

	// シーンイベント取得
	SceneEvent GetSceneEvent() const { return m_SceneEvent; }
};

