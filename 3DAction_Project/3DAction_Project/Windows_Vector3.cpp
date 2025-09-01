
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "Vector3.h"
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
inline Vector3 FromXMVECTOR3(const DirectX::XMVECTOR& vec) noexcept;


// ============================
// 計算関数
// ============================
// 内積
float Vector3::Dot(const Vector3& vec1) const noexcept
{
	// 計算
	return DirectX::XMVectorGetX(
		DirectX::XMVector3Dot(
			DirectXMathUtiles::ToXMVECTOR(x, y, z),
			DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z)));
}

// 外積
Vector3 Vector3::Cross(const Vector3& vec1) const noexcept
{
	// 計算
	DirectX::XMVECTOR cross = DirectX::XMVector3Cross(
		DirectXMathUtiles::ToXMVECTOR(x, y, z),
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z));

	return FromXMVECTOR3(cross);
}

// 長さ
float Vector3::Length() const noexcept
{
	// 計算
	return DirectX::XMVectorGetX(
		DirectX::XMVector3Length(
			DirectXMathUtiles::ToXMVECTOR(x, y, z)));
}

// 正規化
Vector3 Vector3::Normalize() const noexcept
{
	// 計算
	DirectX::XMVECTOR normalize = DirectX::XMVector3Normalize(
		DirectXMathUtiles::ToXMVECTOR(x, y, z));

	// 戻す
	return FromXMVECTOR3(normalize);
}

// 距離
float Vector3::Distance(const Vector3& vec1) const noexcept
{
	// 計算
	return DirectX::XMVectorGetX(
		DirectX::XMVector3Length(
			DirectX::XMVectorSubtract(
				DirectXMathUtiles::ToXMVECTOR(x, y, z),
				DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z))));
}

// 距離の２乗
float Vector3::DistanceSquared(const Vector3& vec1) const noexcept
{
	return DirectX::XMVectorGetX(
		DirectX::XMVector3LengthSq(
			DirectX::XMVectorSubtract(
				DirectXMathUtiles::ToXMVECTOR(x, y, z),
				DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z))));
}

// 線形補間
Vector3 Vector3::Lerp(const Vector3& vec1, float t)  const noexcept
{
	// 計算
	DirectX::XMVECTOR lerp = DirectX::XMVectorLerp(
		DirectXMathUtiles::ToXMVECTOR(x, y, z),
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z),
		t);

	// 戻す
	return FromXMVECTOR3(lerp);
}

// GPUに送るデータに変換
Vector3 Vector3::toGPU() const noexcept
{
	return *this;
}


// ============================
// 演算子
// ============================
// 加算演算子
Vector3 operator+(const Vector3& vec1, const Vector3& vec2) noexcept
{
	// 計算
	DirectX::XMVECTOR add = DirectX::XMVectorAdd(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z),
		DirectXMathUtiles::ToXMVECTOR(vec2.x, vec2.y, vec2.z));

	return FromXMVECTOR3(add);
}

// 減算演算子
Vector3 operator-(const Vector3& vec1, const Vector3& vec2) noexcept
{
	// 計算
	DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z),
		DirectXMathUtiles::ToXMVECTOR(vec2.x, vec2.y, vec2.z));

	return FromXMVECTOR3(subtract);
}

// 乗算演算子
Vector3 operator*(const Vector3& vec1, float scalar) noexcept
{
	// 計算
	DirectX::XMVECTOR multiplication = DirectX::XMVectorScale(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z),
		scalar);

	return FromXMVECTOR3(multiplication);
}

// 除算演算子
Vector3 operator/(const Vector3& vec1, float scalar) noexcept
{
#if defined(DEBUG) || defined(_DEBUG)
	// 防止
	if (scalar == 0.0f) {
		ErrorLog::OutputToConsole("Vector3 : 0で除算しようとしました");
		return Vector3(0.0f, 0.0f, 0.0f);
	}
#endif

	// 計算
	DirectX::XMVECTOR division = DirectX::XMVectorScale(
		DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z),
		1.0f / scalar);

	return FromXMVECTOR3(division);
}


// =====================================
// 関数
// =====================================
// XMVECTORをVector3に変換
inline Vector3 FromXMVECTOR3(const DirectX::XMVECTOR& vec) noexcept
{
	// 変換
	DirectX::XMFLOAT3 out;
	DirectX::XMStoreFloat3(&out, vec);
	return Vector3(out.x, out.y, out.z);
}