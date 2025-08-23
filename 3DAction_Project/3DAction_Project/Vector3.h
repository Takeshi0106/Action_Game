#pragma once

// ==========================================
// Math構造体　Vector3
// 
// 中身をプラットフォームごとに変更する
// ==========================================


// ==========================================
// Vector3 型
// ==========================================
struct Vector3
{
    // メンバー変数
    float x, y, z;

    // コンストラクタ / デストラクタ
    Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
    Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    Vector3(const Vector3& vec) : x(vec.x), y(vec.y), z(vec.z) {}
    ~Vector3() = default;

    // 代入演算子
    Vector3& operator=(const Vector3& vec3) {
        x = vec3.x;
        y = vec3.y;
        z = vec3.z;

        return *this;
    }
    // チェック演算子
    bool operator==(const Vector3& vec) const {
        return x == vec.x && y == vec.y && z == vec.z;
    }
    bool operator!=(const Vector3& vec) const {
        return !(*this == vec);
    }

    // 演算子
    Vector3 operator+(const Vector3& vec3) const;
    Vector3 operator-(const Vector3& vec3) const;
    Vector3 operator*(float scalar) const;
    Vector3 operator/(float scalar) const;

    // 計算関数
    float Dot(const Vector3& vec3) const; // 内積
    Vector3 Cross(const Vector3& vec3) const; // 外積
    float Length() const; // 長さ
    Vector3 Normalize() const; // 正規化
    float Distance(const Vector3& vec) const; // 距離
    float DistanceSquared(const Vector3& vec) const; // 距離の２乗
    Vector3 Lerp(const Vector3& target, float t) const; // 線形補間
};