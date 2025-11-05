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
	// 描画マネージャー
	BaseDrawManager* m_DrawManager = nullptr;

public:
	ActionGame() = default;
	~ActionGame() = default;

	// 初期化
	void Init(BaseDrawManager* _drawManager) override;
	// 更新
	void Update() override;
	// 描画
	void Draw() override;
	// 後処理
	void Uninit() override;
};

