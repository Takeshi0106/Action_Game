#pragma once

// =======================================
// 【クラス概要】
// ゲームの処理を行うクラス
// =======================================


// =======================================
// ヘッダー
// =======================================
// 基底クラス
#include "BaseGame.h"
// シーンマネージャー
#include "SceneManager.h"


// =======================================
// クラス
// =======================================
class ActionGame :public BaseGame
{
private:
	// シーンマネージャー
	SceneManager m_SceneManager;

	// フレーム計測用
	float m_FPSTime = 0.0f;
	int m_FPSCount = 0;

	// 初期化
	bool DerivativeInit() override final;

	// Imgui使用 ＊リリース時は実行しません
	void DebugImgui();

public:
	ActionGame() = default;
	~ActionGame() = default;

	// 描画
	void Draw() override final;
	// 更新
	bool Update() override final;
	// 後処理
	void Uninit() override final;
};
