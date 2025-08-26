#pragma once

// ==========================================
// Math構造体 Matrix3x3
// 
// 中身をプラットフォームごとに変更する
// ==========================================


// ==========================================
// Matrix3x3 型
// ==========================================

struct Matrix4x4
{
	// メンバー変数
	float Matrix[4][4];

    // コンストラクタ：単位行列で初期化
    constexpr Matrix4x4() noexcept : Matrix{
        {1, 0, 0, 0},
        {0, 1, 0, 0},
        {0, 0, 1, 0},
        {0, 0, 0, 1} } {
    }
    Matrix4x4(const Matrix4x4&) noexcept = default;
    // デストラクタ
    ~Matrix4x4() noexcept = default;

    // 代入演算子
    Matrix4x4& operator=(const Matrix4x4&) noexcept = default;

    // チェック演算子 SIMDを使用できる関数があるため、CPPに記載する
    bool operator==(const Matrix4x4& mat);



    // 演算子
    Matrix4x4 operator+(const Matrix4x4& vec2) const;
    Matrix4x4 operator-(const Matrix4x4& vec2) const;
    Matrix4x4 operator*(float scalar) const;
    Matrix4x4 operator/(float scalar) const;

    // 関数
};