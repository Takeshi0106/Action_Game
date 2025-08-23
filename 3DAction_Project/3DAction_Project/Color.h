#pragma once

// ==========================================
// Math色情報クラス
// 
// プラットフォームごとに中身を変更する
// ==========================================


// ===========================================
// 色情報
// ===========================================
class Color
{
private:
    float r, g, b, a;

public:
    // コンストラクタ / デストラクタ
    Color();
    Color(float _r, float _g, float _b, float _a);
    Color(const Color& color);

    ~Color() = default;

    // 演算子
    Color& operator=(const Color& color);
    Color operator+(const Color& color) const;
    Color operator-(const Color& color) const;
    Color operator*(float scalar) const;

    // 計算関数

};