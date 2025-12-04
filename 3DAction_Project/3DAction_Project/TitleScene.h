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
class TitleScene : public BaseSceneState
{
private:
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	TitleScene() = default;
	~TitleScene() = default;

	// シーンの描画
	void Draw();
	// シーンの終了処理
	void Uninit();
};

