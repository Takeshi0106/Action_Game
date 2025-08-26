#pragma once

// ==========================================
// Math構造体 Matrix3x3
// 
// 中身をプラットフォームごとに変更する
// ==========================================


// ==========================================
// Matrix3x3 型
// ==========================================

struct Matrix3x3
{
	// メンバー変数
	float Matrix[3][3];

    // コンストラクタ：単位行列で初期化
    Matrix3x3() : Matrix{
        {1, 0, 0},
        {0, 1, 0},
        {0, 0, 1}} {
    }
    Matrix3x3(const Matrix3x3&) = default; // 代入
    // デストラクタ
    ~Matrix3x3() = default;

    // 代入・チェック演算子
    Matrix3x3& operator=(const Matrix3x3&) = default; // 代入

    // 演算子
    Matrix3x3 operator+(const Matrix3x3& vec2) const;
    Matrix3x3 operator-(const Matrix3x3& vec2) const;
    Matrix3x3 operator*(float scalar) const;
    Matrix3x3 operator/(float scalar) const;

};