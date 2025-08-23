#pragma once

// ==========================================
// Mathクラス
// 
// 中身をプラットフォームごとに変更する
// ==========================================


// ==========================================
// Vector3 型
// ==========================================
class Vector3
{
private:
    float x, y, z;

public:
    // コンストラクタ / デストラクタ
    Vector3();
    Vector3(float _x, float _y, float _z);
    Vector3(const Vector3& rhs);

    ~Vector3() = default;

    // 演算子    
    Vector3& operator=(const Vector3& vec3);
    Vector3 operator+(const Vector3& vec3) const;
    Vector3 operator-(const Vector3& vec3) const;
    Vector3 operator*(float scalar) const;

    // 計算関数
    float Dot(const Vector3& vec3) const;
    Vector3 Cross(const Vector3& vec3) const;
    float Length() const;
    Vector3 Normalize() const;
};