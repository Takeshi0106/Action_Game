
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "Vector2.h"
// ユーリアリティー関数
#include "WindowsMath_Utiles.h"
// 計算ヘッダー
#include <DirectXMath.h>

#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用にログ出力
#include "ReportMessage.h"
#endif


// ======================================
// DirectXTKと同じ、毎計算floatをXMVECTORに変換して、
// 計算、floatに変換を行っています
// 
// ＊注意
// SIMDに対応していますが、毎計算時に変換しているため、
// そこまで速くなっていないと思います。
// ======================================


// ========================================
// プロトタイプ宣言
// ========================================
// XMVECTORをVector3に変換
inline Vector2 FromXMVECTOR2(const DirectX::XMVECTOR& vec) noexcept;


// ============================
// 計算関数
// ============================
// 内積
float Vector2::Dot(const Vector2& vec1) const noexcept
{
	// 計算
	return DirectX::XMVectorGetX(
		DirectX::XMVector2Dot(
			DirectXMathUtiles::ToXMVECTOR(x, y),
			DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y)));
}

// 外積
float Vector2::Cross(const Vector2& vec1) const noexcept
{
	// 計算
	return DirectX::XMVectorGetZ(
		DirectX::XMVector2Cross(
			DirectXMathUtiles::ToXMVECTOR(x, y),
			DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y)));
}

// 長さ
float Vector2::Length() const noexcept
{
	// 計算
	return DirectX::XMVectorGetX(
		DirectX::XMVector2Length(
			DirectXMathUtiles::ToXMVECTOR(x, y)));
}

// 正規化
Vector2 Vector2::Normalize() const noexcept
{
	// 計算
	DirectX::XMVECTOR normalize = DirectX::XMVector2Normalize(
		DirectXMathUtiles::ToXMVECTOR(x, y));

	// 戻す
	return FromXMVECTOR2(normalize);
}

// 距離
float Vector2::Distance(const Vector2& vec1) const noexcept
{
	// 計算
	return DirectX::XMVectorGetX(
		DirectX::XMVector2Length(
			DirectX::XMVectorSubtract(
				DirectXMathUtiles::ToXMVECTOR(x, y),
				DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y))));
}

// 距離の２乗
float Vector2::DistanceSquared(const Vector2& vec1) const noexcept
{
	return DirectX::XMVectorGetX(
		DirectX::XMVector2LengthSq(
			DirectX::XMVectorSubtract(
				DirectXMathUtiles::ToXMVECTOR(x, y),
				DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y))));
}

// 線形補間
Vector2 Vector2::Lerp(const Vector2& vec1, float t) const noexcept
{
	// 計算
	DirectX::XMVECTOR lerp = DirectX::XMVectorLerp(
		DirectXMathUtiles::ToXMVECTOR(x, y),
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y),
		t);

	// 戻す
	return FromXMVECTOR2(lerp);
}


// ============================
// 演算子
// ============================
// 加算演算子
Vector2 operator+(const Vector2& vec1, const Vector2& vec2)noexcept
{
	// 計算
	DirectX::XMVECTOR add = DirectX::XMVectorAdd(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y),
		DirectXMathUtiles::ToXMVECTOR(vec2.x, vec2.y));

	return FromXMVECTOR2(add);
}

// 減算演算子
Vector2 operator-(const Vector2& vec1, const Vector2& vec2) noexcept
{
	// 計算
	DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y),
		DirectXMathUtiles::ToXMVECTOR(vec2.x, vec2.y));

	return FromXMVECTOR2(subtract);
}

// 乗算演算子
Vector2 operator*(const Vector2& vec1,float scalar) noexcept
{
	// 計算
	DirectX::XMVECTOR multiplication = DirectX::XMVectorScale(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y),
		scalar);

	return FromXMVECTOR2(multiplication);
}

// 除算演算子
Vector2 operator/(const Vector2& vec1, float scalar) noexcept
{
#if defined(DEBUG) || defined(_DEBUG)
	// 防止
	if (scalar == 0.0f) {
		ErrorLog::OutputToConsole("Vector4 : 0で除算しようとしました");
		return Vector2(0.0f, 0.0f);
	}
#endif

	// 計算
	DirectX::XMVECTOR division = DirectX::XMVectorScale(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y),
		1.0f / scalar);

	return FromXMVECTOR2(division);
}


// =====================================
// 関数
// =====================================
// XMVECTORをVector3に変換
inline Vector2 FromXMVECTOR2(const DirectX::XMVECTOR& vec) noexcept
{
	// 変換
	DirectX::XMFLOAT2 out = {};
	DirectX::XMStoreFloat2(&out, vec);
	return Vector2(out.x, out.y);
}