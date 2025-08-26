#pragma once

// ==========================================
// Math構造体 Matrix4x4
// 
// 中身をプラットフォームごとに変更する
// C++17基準で作成しています
// ==========================================


// ==========================================
// 前方宣言
// ==========================================
struct Vector3;


// ==========================================
// Matrix4x4 型
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

    // 行列作成
     // 単位行列
    static Matrix4x4 Identity() noexcept;
     // 平行移動行列
    static Matrix4x4 Translation(const Vector3& pos) noexcept;
    // 回転行列
    static Matrix4x4 RotationX(float angle) noexcept;
    static Matrix4x4 RotationY(float angle) noexcept;
    static Matrix4x4 RotationZ(float angle) noexcept;
    static Matrix4x4 RotationYawPitchRoll(float yaw, float pitch, float roll) noexcept;
    // 拡大縮小行列
    static Matrix4x4 Scaling(const Vector3& scale) noexcept;
    // 投影・ビュー行列
    static Matrix4x4 PerspectiveFovLH(float fovY, float aspect, float zn, float zf) noexcept;
    static Matrix4x4 LookAtLH(const Vector3& eye, const Vector3& at, const Vector3& up) noexcept;

    // 計算関数
    // 基本操作
    Matrix4x4 Transpose() const noexcept;   // 転置
    Matrix4x4 Inverse() const noexcept;     // 逆行列
    float Determinant() const noexcept;     // 行列式

    // 行列からベクトルやスケールを抽出
    Vector3 ExtractTranslation() const noexcept;
    Vector3 ExtractScaling() const noexcept;
    Vector3 ExtractRotationEuler() const noexcept; // オイラー角に変換
};


// 演算子
Matrix4x4 operator+(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept;
Matrix4x4 operator-(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept;
Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept;
Vector3   operator*(const Matrix4x4& mat1, Vector3 vec1) noexcept;
Matrix4x4 operator*(const Matrix4x4& mat1, float scalar) noexcept;
Matrix4x4 operator/(const Matrix4x4& mat1, float scalar) noexcept;
bool      operator==(const Matrix4x4& mat1, const Matrix4x4& mat) noexcept;