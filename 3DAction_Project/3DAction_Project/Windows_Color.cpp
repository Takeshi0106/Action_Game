
// =========================================
// 【クラス概要】
// WIndows(DirectXMath)用の色情報クラス
// =========================================


// ============================
// ヘッダー
// ============================
#include "Color.h"
#include <DirectXMath.h>
#include "WindowsMath_Utiles.h"


// ==============================
// プロトタイプ宣言
// ==============================
// XMVECTORをColorに変換
inline Color FromXMColor(const DirectX::XMVECTOR& vec);
// 内容が下限と上限か判定する　下限より下の場合、下限に　上限以上の場合上限になる
inline void LimitCheck(float& value, const float& min, const float& max);


// =========================================
// 演算子
// =========================================
// 加算演算子
Color Color::operator+(const Color& color) const
{
    // 計算
    DirectX::XMVECTOR add = DirectX::XMVectorAdd(
        DirectXMathUtiles::ToXMVECTOR(r, g, b, a),
        DirectXMathUtiles::ToXMVECTOR(color.r, color.g, color.b, color.a));

    return FromXMColor(add);
}

// 減算演算子
Color Color::operator-(const Color& color) const
{
    // 計算
    DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(
        DirectXMathUtiles::ToXMVECTOR(r, g, b, a),
        DirectXMathUtiles::ToXMVECTOR(color.r, color.g, color.b, color.a));

    return FromXMColor(subtract);
}

// 乗算演算子
Color Color::operator*(float scalar) const
{
    // 計算
    DirectX::XMVECTOR multiplication = DirectX::XMVectorScale(
        DirectXMathUtiles::ToXMVECTOR(r, g, b, a),
        scalar);

    return FromXMColor(multiplication);
}
// カラー同士の乗算
Color Color::operator*(const Color& color) const
{
    // 計算
    DirectX::XMVECTOR multiplication = DirectX::XMVectorMultiply(
        DirectXMathUtiles::ToXMVECTOR(r, g, b, a),
        DirectXMathUtiles::ToXMVECTOR(color.r, color.g, color.b, color.a));

    return FromXMColor(multiplication);
}


// ==================================
// 計算関数
// ==================================
// 線形補間
Color Color::Lerp(const Color& color, float t) const
{
    // 計算
    DirectX::XMVECTOR result = DirectX::XMVectorLerp(
        DirectXMathUtiles::ToXMVECTOR(r, g, b, a),
        DirectXMathUtiles::ToXMVECTOR(color.r, color.g, color.b, color.a),
        t);

    return FromXMColor(result);
}


void Color::Clamp()
{
    LimitCheck(r, 0.0f, 1.0f);
    LimitCheck(g, 0.0f, 1.0f);
    LimitCheck(b, 0.0f, 1.0f);
    LimitCheck(a, 0.0f, 1.0f);
}


// ==================================
// 関数
// ==================================
inline Color FromXMColor(const DirectX::XMVECTOR& vec)
{
    // 変換
    DirectX::XMFLOAT4 out;
    DirectX::XMStoreFloat4(&out, vec);
    return Color(out.x, out.y, out.z,out.w);
}


inline void LimitCheck(float& value, const float& min, const float& max)
{
    // 判定する
    if (value < min)
    {
        value = min;
    }
    else if (value > max)
    {
        value = max;
    }
}