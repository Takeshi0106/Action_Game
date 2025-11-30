#pragma once

// =======================================
// 【クラス概要】
// ゲームの処理を行うクラス
// =======================================


// =======================================
// ヘッダー
// =======================================
#include "BaseGame.h"


// =======================================
// クラス
// =======================================
class ActionGame :public BaseGame
{
private:
	// フレーム計測用
	float m_FPSTime = 0.0f;
	int m_FPSCount = 0;

	// 初期化
	bool DerivativeInit() override final;

public:
	ActionGame() = default;
	~ActionGame() = default;

	// 更新
	void Update() override final;
	// 描画
	void Draw() override final;
	// 後処理
	void Uninit() override final;
};
