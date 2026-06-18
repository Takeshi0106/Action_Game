#pragma once

// ==================================
// 【関数概要】
// Mathで使用する共通関数
// ==================================


// =================================
// ヘッダー
// =================================
// 計算ヘッダ―
#include <DirectXMath.h>

namespace DirectXMathUtiles {

	// floatをXMVECTORに変換
	DirectX::XMVECTOR ToXMVECTOR(const float& a, const float& b, const float& c, const float& d);
	DirectX::XMVECTOR ToXMVECTOR(const float& a, const float& b, const float& c);
	DirectX::XMVECTOR ToXMVECTOR(const float& a, const float& b);
}
