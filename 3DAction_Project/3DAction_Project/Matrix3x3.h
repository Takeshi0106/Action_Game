#pragma once

// ==========================================
// Math構造体 Matrix3x3
// 
// 関数の実態をプラットフォームごとに変更する
// 
// * =---------------------------------------
// * 左手座標系、列優先です
// * 角度は全てラジアンで渡す想定です
// * ----------------------------------------
// ==========================================


// ==========================================
// 前方宣言
// ==========================================
struct Vector3;
struct Quaternion;


// ==========================================
// Matrix3x3 型
// ==========================================

struct Matrix3x3
{
    // メンバー変数
    float Matrix[3][3];

    // コンストラクタ：単位行列で初期化
    constexpr Matrix3x3() noexcept : Matrix{
        {1, 0, 0 },
        {0, 1, 0 },
        {0, 0, 1 } } {
    }

    constexpr Matrix3x3(
        float m00, float m01, float m02,
        float m10, float m11, float m12,
        float m20, float m21, float m22 ) noexcept: Matrix{
            { m00, m01, m02, },
            { m10, m11, m12, },
            { m20, m21, m22, } } {
    }

    Matrix3x3(const Matrix3x3&) noexcept = default;
    // デストラクタ
    ~Matrix3x3() noexcept = default;

    // 代入演算子
    Matrix3x3& operator=(const Matrix3x3&) noexcept = default;

    // 行列作成
    static Matrix3x3 CreateIdentityMatrix() noexcept { return Matrix3x3(); } // 単位行列
	// 拡大縮小行列
	static Matrix3x3 CreateScalingMatrix_LH(const Vector3& scale) noexcept;
    // 回転行列
    static Matrix3x3 CreateRotationXMatrix_LH(float radAngle) noexcept;
    static Matrix3x3 CreateRotationYMatrix_LH(float radAngle) noexcept;
    static Matrix3x3 CreateRotationZMatrix_LH(float radAngle) noexcept;
    static Matrix3x3 CreateRotationYawPitchRollMatrix_LH(float yaw, float pitch, float roll) noexcept;
    static Matrix3x3 CreateRotationQuaternion_LH(const Quaternion& quaternion) noexcept;

    // 計算関数
    Matrix3x3 Transpose() const noexcept;   // 転置
    Matrix3x3 Inverse() const noexcept;     // 逆行列
    float Determinant() const noexcept;     // 行列式
	Matrix3x3 Abs() const noexcept;         // 各要素の絶対値を取る
    Matrix3x3 toGPU() const noexcept;       // GPUに送るように作成

    // 抽出
    Vector3 ExtractScaling() const noexcept;       // スケール抽出
    Quaternion ExtractQuaternion() const noexcept; // 行列からクォータニオンを取得する
};


// 演算子
Matrix3x3 operator+(const Matrix3x3& mat1, const Matrix3x3& mat2) noexcept;
Matrix3x3 operator-(const Matrix3x3& mat1, const Matrix3x3& mat2) noexcept;
Matrix3x3 operator*(const Matrix3x3& mat1, const Matrix3x3& mat2) noexcept;
Matrix3x3 operator*(const Matrix3x3& mat, const Quaternion& q) noexcept;
Vector3   operator*(const Matrix3x3& mat1, Vector3 vec1) noexcept;
Matrix3x3 operator*(const Matrix3x3& mat1, float scalar) noexcept;
Matrix3x3 operator/(const Matrix3x3& mat1, float scalar) noexcept;
bool      operator==(const Matrix3x3& mat1, const Matrix3x3& mat) noexcept;
