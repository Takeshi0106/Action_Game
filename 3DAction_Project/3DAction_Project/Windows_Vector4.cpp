
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "Vector4.h"
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
inline Vector4 FromXMVECTOR4(const DirectX::XMVECTOR& vec) noexcept;


// ============================
// 計算関数
// ============================
// 内積
float Vector4::Dot(const Vector4& vec1) const noexcept
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector4Dot(
            DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
            DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w)));
}

// 長さ
float Vector4::Length() const noexcept
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector4Length(
            DirectXMathUtiles::ToXMVECTOR(x, y, z, w)));
}

// 正規化
Vector4 Vector4::Normalize() const noexcept
{
    // 計算
    DirectX::XMVECTOR normalize = DirectX::XMVector4Normalize(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w));

    // 戻す
    return FromXMVECTOR4(normalize);
}

// 距離
float Vector4::Distance(const Vector4& vec1) const noexcept
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector4Length(
            DirectX::XMVectorSubtract(
                DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
                DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w))));
}

// 距離の２乗
float Vector4::DistanceSquared(const Vector4& vec1) const noexcept
{
    return DirectX::XMVectorGetX(
        DirectX::XMVector4LengthSq(
            DirectX::XMVectorSubtract(
                DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
                DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w))));
}

// 線形補間
Vector4 Vector4::Lerp(const Vector4& vec1, float t) const noexcept
{
    // 計算
    DirectX::XMVECTOR lerp = DirectX::XMVectorLerp(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
        DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w),
        t);

    // 戻す
    return FromXMVECTOR4(lerp);
}

// GPUに送るデータに変換
Vector4 Vector4::toGPU() const noexcept
{
    return *this;
}

// ============================
// 演算子
// ============================
// 加算演算子
Vector4 operator+(const Vector4& vec1, const Vector4& vec2) noexcept
{
    // 計算
    DirectX::XMVECTOR add = DirectX::XMVectorAdd(
        DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w),
        DirectXMathUtiles::ToXMVECTOR(vec2.x, vec2.y, vec2.z, vec2.w));

    return FromXMVECTOR4(add);
}

// 減算演算子
Vector4 operator-(const Vector4& vec1, const Vector4& vec2) noexcept
{
    // 計算
    DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(
        DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w),
        DirectXMathUtiles::ToXMVECTOR(vec2.x, vec2.y, vec2.z, vec2.w));

    return FromXMVECTOR4(subtract);
}

// 乗算演算子
Vector4 operator*(const Vector4& vec1, float scalar) noexcept
{
    // 計算
    DirectX::XMVECTOR multiplication = DirectX::XMVectorScale(
        DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w),
        scalar);

    return FromXMVECTOR4(multiplication);
}

// 除算演算子
Vector4 operator/(const Vector4& vec1, float scalar) noexcept
{
#if defined(DEBUG) || defined(_DEBUG)
    // 防止
    if (scalar == 0.0f) {
        ErrorLog::OutputToConsole("Vector4 : 0で除算しようとしました");
        return Vector4(0.0f, 0.0f, 0.0f, 0.0f);
    }
#endif

    // 計算
    DirectX::XMVECTOR division = DirectX::XMVectorScale(
        DirectXMathUtiles::ToXMVECTOR(vec1.x, vec1.y, vec1.z, vec1.w),
        1.0f / scalar);

    return FromXMVECTOR4(division);
}


// =====================================
// 関数
// =====================================
// XMVECTORをVector3に変換
inline Vector4 FromXMVECTOR4(const DirectX::XMVECTOR& vec) noexcept
{
    // 変換
    DirectX::XMFLOAT4 out;
    DirectX::XMStoreFloat4(&out, vec);
    return Vector4(out.x, out.y, out.z, out.w);
}