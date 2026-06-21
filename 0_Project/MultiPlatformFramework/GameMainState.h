#pragma once

// =======================================
// 【クラス概要】
// ゲームのメイン処理を行うクラス
// =======================================


// =======================================
// ヘッダー
// =======================================
// 基底クラス
#include "BaseSceneState.h"

// =======================================
// クラス
// =======================================
class GameMainState final : public BaseSceneState
{
private:
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	// コンストラクタ・デストラクタ
	GameMainState() = default;
	~GameMainState() override final = default;

	// シーンの描画
	void Draw() override final;
	// シーンの終了処理
	void Uninit() override final;
};

