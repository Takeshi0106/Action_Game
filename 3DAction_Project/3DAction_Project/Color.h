#pragma once

// ==========================================
// Math色情報クラス
// 
// プラットフォームごとに中身を変更する
// C++17基準
// ==========================================


// ===========================================
// 色情報
// ===========================================
struct Color
{
    // メンバー変数
    float r, g, b, a;

    // コンストラクタ / デストラクタ
    Color() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    Color(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}
    Color(const Color& color) :r(color.r), g(color.g), b(color.b), a(color.a) {}

    ~Color() = default;

    // 代入演算子
    Color& operator=(const Color& color) = default;

    // 演算子
    Color operator+(const Color& color) const;
    Color operator-(const Color& color) const;
    Color operator*(float scalar) const;
    Color operator*(const Color& color) const;

    // 計算関数
    Color Lerp(const Color& color, float t) const; // 線形補間
    void Clamp(); // 0~1.0に制限させる
};