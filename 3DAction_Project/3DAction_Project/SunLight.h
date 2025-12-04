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


// ===============================
// クラス
// ===============================
class SunLight : public BaseLight
{
private:
	// 光の方向
	Vector3 m_Direction;
	// 定数文字列
	const char* m_CBName = "SunLight";

	// 派生初期化
	bool DerivativeInit();

public:
	// コンストラクタ / デストラクタ
	SunLight()
		: m_Direction(0.0f, -1.0f, 0.0f)
	{}
	~SunLight() = default;

	// 基本関数
	void Update();
	void UpdateToGPU();
	void Uninit();

	// セッター
	void SetDirection(const Vector3& direction) { m_Direction = direction; }
	// ゲッター
	const Vector3& GetDirection() const { return m_Direction; }
};

