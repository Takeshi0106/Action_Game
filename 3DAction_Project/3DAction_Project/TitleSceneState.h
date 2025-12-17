#pragma once

// ========================================
// タイトルシーン
// ========================================


// ========================================
// ヘッダー
// ========================================
// 基底クラス
#include "BaseSceneState.h"
// カメラ作成
#include "DCCCamera3D.h"
// 太陽光
#include "SunLight.h"
// ナイトオブジェクト
#include "Player.h"


// ========================================
// クラス
// ========================================
class TitleSceneState final : public BaseSceneState
{
private:
	// DCCカメラ
	DCCCamera3D m_Camera;
	// 太陽光
	SunLight m_SunLight;
	// ナイトオブジェクト
	Player m_Player;


	// -----------------------------------
	// メンバー関数
	// -----------------------------------
	// シーンの初期化
	bool DerivativeInit() override final;
	// シーンの更新
	void DerivatIveUpdate(float _delta) override final;

public:
	// -----------------------------------
	// コンストラクタ・デストラクタ
	// -----------------------------------
	TitleSceneState() = default;
	~TitleSceneState() = default;


	// シーンの描画
	void Draw() override final;
	// シーンの終了処理
	void Uninit() override final;
};

