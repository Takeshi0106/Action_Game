#pragma once

// ============================
// 【クラス概要】
// 制作・デバッグ用カメラ
// シーン上を自由に動き回れるカメラ
// 投資投影
// 初期化前にInputをセットしてください。
// エラーが出ます。
// ============================


// =============================
// ヘッダー
// =============================
// 基底クラスヘッダー
#include "BaseCamera.h"
// プレイヤーコントローラー
#include "DCCCamera3D_Controller.h"


// =============================
// 前方宣言
// =============================
class Input;


// =============================
// クラス
// =============================
class DCCCamera3D final : public BaseCamera
{
private:
	// コントローラー
	DCCCamera3D_Controller m_Controller;

	// 派生初期化
	bool DerivativInit() override final;

	// カメラを動かす
	void MoveCamera();

public:
	// コンストラクタ・デストラクタ
	DCCCamera3D() = default;
	~DCCCamera3D() = default;
	
	// 初期化
	void SetInput(Input* input);

	// 更新
	void Update() override final;

	// X軸反転設定
	void SetInvertX(bool isInvert) {
		m_Controller.SetInvertX(isInvert);
	}
};

