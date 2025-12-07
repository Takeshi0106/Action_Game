#pragma once

// ============================
// 【クラス概要】
// DCCカメラ3Dコントローラー
// ============================


// ============================
// ヘッダー
// ============================
// 入力情報ヘッダー
#include "Input.h"


// ============================
// クラス
// ============================
class DCCCamera3D_Controller
{
private:
	// 入力情報
	Input* m_Input = nullptr;
	// マウス感度
	float m_MouseSensitivity = 0.002f;
	// カメラの位置感度
	float m_CameraPositionSensitivity = 0.5f;
	// マウスのXを反転
	float m_IsInvertX = -1.0f;

public:
	// コンストラクタ・デストラクタ
	DCCCamera3D_Controller() = default;
	~DCCCamera3D_Controller() = default;
	
	// 初期化
	void Init(Input* input) {
		m_Input = input;
	}

	// 初期化が行われているかのチェック
	bool IsInit() {
		if (m_Input == nullptr) {
			return false;
		}

		return true;
	}

	// 入力情報受け取り
	// Activeにするか取得
	bool GetControlActive();
	// 位置移動ベクトル取得
	Vector2 GetPositionVector();
	// 視点移動ベクトル取得
	Vector2 GetLookVector();

	// セッター
	void SetMouseSensitivity(float sensitivity) { m_MouseSensitivity = sensitivity; }
	void SetInvertX(bool isInvert)
	{
		if (isInvert) {
			m_IsInvertX = -1.0f;
		}
		else {
			m_IsInvertX = 1.0f;
		}
	}
};

