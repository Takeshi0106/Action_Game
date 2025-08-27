#pragma once

// ==========================================
// Math構造体 Matrix4x4
// 
// 中身をプラットフォームごとに変更する
// C++17基準で作成しています
// 
// * =---------------------------------------
// * 左手座標系、行優先です
// * 角度は全てラジアンで渡す想定です
// * ----------------------------------------
// ==========================================


// ==========================================
// 前方宣言
// ==========================================
struct Vector3;
struct Quaternion;


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
        {0, 0, 0, 1} } {}

    constexpr Matrix4x4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33) noexcept
        : Matrix{
            { m00, m01, m02, m03 },
            { m10, m11, m12, m13 },
            { m20, m21, m22, m23 },
            { m30, m31, m32, m33 } } {}

    Matrix4x4(const Matrix4x4&) noexcept = default;
    // デストラクタ
    ~Matrix4x4() noexcept = default;

    // 代入演算子
    Matrix4x4& operator=(const Matrix4x4&) noexcept = default;

    // 行列作成
    static Matrix4x4 CreateIdentityMatrix() noexcept { return Matrix4x4(); } // 単位行列
    // 平行移動行列
    static Matrix4x4 CreateTranslationMatrix_LH(const Vector3& pos) noexcept;
    // 回転行列
    static Matrix4x4 CreateRotationXMatrix_LH(float radAngle) noexcept;
    static Matrix4x4 CreateRotationYMatrix_LH(float radAngle) noexcept;
    static Matrix4x4 CreateRotationZMatrix_LH(float radAngle) noexcept;
    static Matrix4x4 CreateRotationYawPitchRollMatrix_LH(float yaw, float pitch, float roll) noexcept;
    static Matrix4x4 CreateRotationQuaternion_LH(const Quaternion& quaternion) noexcept;
    // 拡大縮小行列
    static Matrix4x4 CreateScalingMatrix_LH(const Vector3& scale) noexcept;
    // 投影行列
    static Matrix4x4 CreateProjectionMatrix_LH(float fovY, float aspect, float zn, float zf) noexcept;
    // ビュー変換行列
    static Matrix4x4 CreateViewMatrix_LH(const Vector3& cameraPos, const Vector3& lookPoint, const Vector3& upDir) noexcept;

    // 計算関数
    Matrix4x4 Transpose() const noexcept;   // 転置
    Matrix4x4 Inverse() const noexcept;     // 逆行列
    float Determinant() const noexcept;     // 行列式

    // 抽出
    Vector3 ExtractTranslation() const noexcept;   // 平行移動を抽出
    Vector3 ExtractScaling() const noexcept;       // スケール抽出
    Quaternion ExtractQuaternion() const noexcept; // 行列からクォータニオンを取得する
};


// 演算子
Matrix4x4 operator+(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept;
Matrix4x4 operator-(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept;
Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept;
Matrix4x4 operator*(const Matrix4x4& mat, const Quaternion& q) noexcept;
Vector3   operator*(const Matrix4x4& mat1, Vector3 vec1) noexcept;
Matrix4x4 operator*(const Matrix4x4& mat1, float scalar) noexcept;
Matrix4x4 operator/(const Matrix4x4& mat1, float scalar) noexcept;
bool      operator==(const Matrix4x4& mat1, const Matrix4x4& mat) noexcept;