
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
inline Vector4 FromXMVECTOR4(const DirectX::XMVECTOR& vec);


// ============================
// 演算子
// ============================
// 加算演算子
Vector4 Vector4::operator+(const Vector4& vec) const
{
    // 計算
    DirectX::XMVECTOR add = DirectX::XMVectorAdd(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
        DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z, vec.w));

    return FromXMVECTOR4(add);
}

// 減算演算子
Vector4 Vector4::operator-(const Vector4& vec) const
{
    // 計算
    DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
        DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z, vec.w));

    return FromXMVECTOR4(subtract);
}

// 乗算演算子
Vector4 Vector4::operator*(float scalar) const
{
    // 計算
    DirectX::XMVECTOR multiplication = DirectX::XMVectorScale(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
        scalar);

    return FromXMVECTOR4(multiplication);
}

// 除算演算子
Vector4 Vector4::operator/(float scalar) const
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
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
        1.0f / scalar);

    return FromXMVECTOR4(division);
}


// ============================
// 計算関数
// ============================
// 内積
float Vector4::Dot(const Vector4    & vec) const
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector4Dot(
            DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
            DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z, vec.w)));
}

// 長さ
float Vector4::Length() const
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector4Length(
            DirectXMathUtiles::ToXMVECTOR(x, y, z, w)));
}

// 正規化
Vector4 Vector4::Normalize() const
{
    // 計算
    DirectX::XMVECTOR normalize = DirectX::XMVector4Normalize(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w));

    // 戻す
    return FromXMVECTOR4(normalize);
}

// 距離
float Vector4::Distance(const Vector4& vec) const
{
    // 計算
    return DirectX::XMVectorGetX(
        DirectX::XMVector4Length(
            DirectX::XMVectorSubtract(
                DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
                DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z, vec.w))));
}


// 距離の２乗
float Vector4::DistanceSquared(const Vector4& vec) const
{
    return DirectX::XMVectorGetX(
        DirectX::XMVector4LengthSq(
            DirectX::XMVectorSubtract(
                DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
                DirectXMathUtiles::ToXMVECTOR(vec.x, vec.y, vec.z, vec.w))));
}


// 線形補間
Vector4 Vector4::Lerp(const Vector4& target, float t) const
{
    // 計算
    DirectX::XMVECTOR lerp = DirectX::XMVectorLerp(
        DirectXMathUtiles::ToXMVECTOR(x, y, z, w),
        DirectXMathUtiles::ToXMVECTOR(target.x, target.y, target.z, target.w),
        t);

    // 戻す
    return FromXMVECTOR4(lerp);
}


// =====================================
// 関数
// =====================================
// XMVECTORをVector3に変換
inline Vector4 FromXMVECTOR4(const DirectX::XMVECTOR& vec)
{
    // 変換
    DirectX::XMFLOAT4 out;
    DirectX::XMStoreFloat4(&out, vec);
    return Vector4(out.x, out.y, out.z, out.w);
}