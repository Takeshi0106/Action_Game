#pragma once

// ==============================
// クォータニオン構造体
// 
// C++17で作成しています
// Larp Slarp　補間がないため、後から追加するべき
// ==============================


// ==============================
// ヘッダー
// ==============================
#include "Vector3.h"


// ==============================
// 前方宣言
// ==============================
struct Matrix4x4;


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

    // 指定した方向を向くクォータニオンを作成
    static Quaternion LookRotation(const Vector3& forward, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f)) noexcept;

    // 正規化
    Quaternion Normalize() const noexcept;
    // 共役　正規化している時は逆クォータニオンと同じ物が得られます
    Quaternion Conjugate() const noexcept;
    // 逆クォータニオン
    Quaternion Inverse() const noexcept;
    // ベクトルを回転
    Vector3 RotateVector(const Vector3& v) const noexcept;
    // Slerp補間
    Quaternion Slerp(const Quaternion& target, float t) const noexcept;

    // GPUに送るデータに変換
    Quaternion toGPU() const noexcept;
};

// クォータニオンの掛け算（回転の合成）
Quaternion operator*(const Quaternion& q1, const Quaternion& q2) noexcept;
