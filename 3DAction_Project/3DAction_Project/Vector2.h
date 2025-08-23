#pragma once

// ==========================================
// Math構造体 Vector2
// 
// 中身をプラットフォームごとに変更する
// ==========================================


// ==========================================
// Vector2 型
// ==========================================
struct Vector2
{
    // メンバー変数
    float x, y;

    // コンストラクタ / デストラクタ
    Vector2() : x(0.0f), y(0.0f) {}
    Vector2(float _x, float _y) : x(_x), y(_y) {}
    Vector2(const Vector2& vec) : x(vec.x), y(vec.y) {}
    ~Vector2() = default;

    // 代入・チェック演算子
    Vector2& operator=(const Vector2& vec2) {
        x = vec2.x;
        y = vec2.y;

        return *this;
    }
    bool operator==(const Vector2& vec2) const
    {
        return x == vec2.x && y == vec2.y;
    }
    bool operator!=(const Vector2& vec2) const
    {
        return !(*this == vec2);
    }

    // 演算子
    Vector2 operator+(const Vector2& vec2) const;
    Vector2 operator-(const Vector2& vec2) const;
    Vector2 operator*(float scalar) const;
    Vector2 operator/(float scalar) const;

    // 計算関数
    float Dot(const Vector2& vec2) const; // 内積
    Vector2 Cross(const Vector2& vec2) const; // 外積
    float Length() const; // 長さ
    Vector2 Normalize() const; // 正規化
    float Distance(const Vector2& vec) const; // 距離
    float DistanceSquared(const Vector2& vec) const; // 距離の２乗
    Vector2 Lerp(const Vector2& target, float t) const; // 線形補間
};