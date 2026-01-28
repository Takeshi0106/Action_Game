#pragma once

// ===============================
// 太陽光クラス
// ===============================


// ===============================
// ヘッダー
// ===============================
// 基底クラス
#include "BaseLight.h"
// 計算ヘッダー
#include "Vector3.h"
// 文字列ヘッダー
#include "UTF8_String.h"


// ===============================
// クラス
// ===============================
class SunLight final : public BaseLight
{
private:
	// 光の方向
	Vector3 m_Direction;
	// 定数文字列
	const String m_CBName = u8"SunLight";

	// 派生初期化
	bool DerivativeInit();

	// デバッグ用imgui関数
	void DebugImgui();

public:
	// コンストラクタ / デストラクタ
	SunLight()
		: m_Direction(0.0f, -1.0f, 0.0f)
	{}
	~SunLight() = default;

	// 基本関数
	void Update() override final;
	void UpdateToGPU() override final;
	void Uninit() override final;

	// セッター
	void SetDirection(const Vector3& direction) { m_Direction = direction; }
	// ゲッター
	const Vector3& GetDirection() const { return m_Direction; }
};

