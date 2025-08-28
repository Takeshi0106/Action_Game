#pragma once

// ==============================
// クォータニオン構造体
// 
// C++17で作成しています
// ==============================


// ==============================
// 前方宣言
// ==============================
struct Matrix4x4;
struct Vector3;


// ==============================
// 構造体
// ==============================
struct Quaternion 
{
    // メンバー変数
	float x, y, z, w;

    // コンストラクタ
    constexpr Quaternion() noexcept : x(0), y(0), z(0), w(1) {} // 単位クォータニオン
    constexpr Quaternion(float _x, float _y, float _z, float _w) noexcept
        : x(_x), y(_y), z(_z), w(_w) {
    }


    // クォータニオン作成
    // 行列からクォータニオン作成
    static Quaternion CreateQuaternionFromMatrix(const Matrix4x4& mat) noexcept;
    // オイラー角から作成
    static Quaternion CreateQuaternionFromEuler(float pitch, float yaw, float roll) noexcept;
    // 軸と角度から作成
    static Quaternion CreateQuaternionFromAxisAngle(const Vector3& axis, float angle) noexcept;

    // 正規化
    Quaternion Normalize() const noexcept;
    // 共役　正規化している時は逆クォータニオンと同じ物が得られます
    Quaternion Conjugate() const noexcept;
    // 逆クォータニオン
    Quaternion Inverse() const noexcept;
    // ベクトルを回転
    Vector3 RotateVector(const Vector3& v) const noexcept;

    // GPUに送るデータに変換
    Quaternion toGPU() const noexcept;
};


// クォータニオンの掛け算（回転の合成）
Quaternion operator*(const Quaternion& q1, const Quaternion& q2) noexcept;