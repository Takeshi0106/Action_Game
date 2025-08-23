
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
Color FromXMColor(const DirectX::XMVECTOR& vec);

// ============================
// カラー
// ============================

// コンストラクタ
Color::Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
Color::Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
Color::Color(const Color& color) :r(color.r), g(color.g), b(color.b), a(color.a) {}


// =========================================
// 演算子
// =========================================
// 代入演算子
Color& Color::operator=(const Color& color)
{
    r = color.r;
    g = color.g;
    b = color.b;
    a = color.a;

    return *this;
}

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



// ==================================
// プロトタイプ宣言
// ==================================
Color FromXMColor(const DirectX::XMVECTOR& vec)
{
    // 変換
    DirectX::XMFLOAT4 out;
    DirectX::XMStoreFloat4(&out, vec);
    return Color(out.x, out.y, out.z,out.w);
}