#pragma once

// ========================================
// シーンの基底クラス
// ========================================


// ========================================
// ヘッダー
// ========================================
// モジュール
#include "GameModule.h"
// シーンイベント
#include "SceneIDSetting.h"


// ========================================
// クラス
// ========================================
class BaseSceneState
{
protected:
	// マネージャー
	GameModules* m_Modules = nullptr;

	// シーンイベント
	SceneEventID m_SceneEvent = NONE;

	// 派生初期化・更新
	virtual bool DerivativeInit() = 0;
	virtual void DerivatIveUpdate(float _delta) = 0;

public:
	// コンストラクタ・デストラクタ
	BaseSceneState() = default;
	virtual ~BaseSceneState() = default;
	
	// シーンの初期化
	bool Init(GameModules* _modules) {
		// 各情報初期化
		m_Modules = _modules;

		// 派生クラスの初期化
		return DerivativeInit();
	}

	// シーンの更新
	void Update(float _deltaTime) {
		// 派生クラスの更新
		DerivatIveUpdate(_deltaTime);
	}

	// シーンの描画
	virtual void Draw() = 0;
	// シーンの終了処理
	virtual void Uninit() = 0;

	// シーンイベント取得
	SceneEventID GetSceneEvent() const { return m_SceneEvent; }
};

