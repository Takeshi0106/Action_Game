#pragma once

// ========================================
// タイトルシーン
// ========================================


// ========================================
// ヘッダー
// ========================================
// 基底クラス
#include "BaseSceneState.h"

// ========================================
// クラス
// ========================================
class TitleSceneState final : public BaseSceneState
{
private:
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	// コンストラクタ・デストラクタ
	TitleSceneState() = default;
	~TitleSceneState() = default;

	// シーンの描画
	void Draw() override final;
	// シーンの終了処理
	void Uninit() override final;
};

