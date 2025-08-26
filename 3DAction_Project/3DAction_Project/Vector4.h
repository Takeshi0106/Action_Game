#pragma once

// ==========================================
// Math構造体　Vector4
// 
// 中身をプラットフォームごとに変更する
// C++17基準
// ==========================================


// ==========================================
// Vector4 型
// ==========================================
struct Vector4
{
    // メンバー変数
    float x, y, z, w;

    // コンストラクタ / デストラクタ
    Vector4() : x(0.0f), y(0.0f), z(0.0f),w(0.0f) {}
    Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}
    Vector4(const Vector4& vec) : x(vec.x), y(vec.y), z(vec.z),w(vec.w) {}
    ~Vector4() = default;

    // 代入演算子
    Vector4& operator=(const Vector4& vec4) = default;

    // チェック演算子
    bool operator==(const Vector4& vec) const {
        return x == vec.x && y == vec.y && z == vec.z && w == vec.w;
    }
    bool operator!=(const Vector4& vec) const {
        return !(*this == vec);
    }

    // 演算子
    Vector4 operator+(const Vector4& vec4) const;
    Vector4 operator-(const Vector4& vec4) const;
    Vector4 operator*(float scalar) const;
    Vector4 operator/(float scalar) const;

    // 計算関数
    float Dot(const Vector4& vec4) const; // 内積
    float Length() const; // 長さ
    Vector4 Normalize() const; // 正規化
    float Distance(const Vector4& vec) const; // 距離
    float DistanceSquared(const Vector4& vec) const; // 距離の２乗
    Vector4 Lerp(const Vector4& target, float t) const; // 線形補間
};