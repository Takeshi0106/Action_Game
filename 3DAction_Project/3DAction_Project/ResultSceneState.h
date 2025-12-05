#pragma once

// ========================================
// クラス概要
// リザルトシーン
// ========================================


// ========================================
// ヘッダー
// ========================================
#include "BaseSceneState.h"


// ========================================
// クラス
// ========================================
class ResultSceneState final : public BaseSceneState
{
private:
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	// コンストラクタ・デストラクタ
	ResultSceneState() = default;
	~ResultSceneState() = default;

	// シーンの描画
	void Draw();
	// シーンの終了処理
	void Uninit();

};

