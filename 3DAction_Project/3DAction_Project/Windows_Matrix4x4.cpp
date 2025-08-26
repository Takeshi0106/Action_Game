
// =====================================
// DirectXMath
// DirectXMathは
// 左手座標系・行優先
// =====================================


// =====================================
// ヘッダー
// =====================================
// 必須ヘッダー
#include "Matrix4x4.h"
// ベクトルヘッダー
#include "Vector3.h"
// 計算のためのヘッダー
#include <DirectXMath.h>


// ======================================
// 行列作成
// ======================================
// 平行行列作成
Matrix4x4 Matrix4x4::CreateTranslationMatrix(const Vector3& pos) noexcept
{
	Matrix4x4 mat = Matrix4x4::CreateIdentityMatrix(); // 単位行列を作成
    
    mat.Matrix[0][3] = pos.x;
    mat.Matrix[1][3] = pos.y;
    mat.Matrix[2][3] = pos.z;

    return mat;
}

// 回転行列
Matrix4x4 Matrix4x4::CreateRotationXMatrix(float angle) noexcept
{
    // 回転行列を作成
    DirectX::XMMATRIX xMat = DirectX::XMMatrixRotationX(angle);

    // 行優先に変換
    DirectX::XMFLOAT4X4 rowMat;
    DirectX::XMStoreFloat4x4(&rowMat, xMat);

    // Matrix4x4 に代入
    Matrix4x4 mat;

    for (int i = 0; i < 4; ++i)
    {
        mat.Matrix[i][0] = rowMat.m[i][0];
        mat.Matrix[i][1] = rowMat.m[i][1];
        mat.Matrix[i][2] = rowMat.m[i][2];
        mat.Matrix[i][3] = rowMat.m[i][3];
    }

    return mat;
}

Matrix4x4 Matrix4x4::CreateRotationYMatrix(float angle) noexcept
{
    // 回転行列を作成
    DirectX::XMMATRIX yMat = DirectX::XMMatrixRotationY(angle);

    // 行優先に変換
    DirectX::XMFLOAT4X4 rowMat;
    DirectX::XMStoreFloat4x4(&rowMat, yMat);

    // Matrix4x4 に代入
    Matrix4x4 mat;

    for (int i = 0; i < 4; ++i)
    {
        mat.Matrix[i][0] = rowMat.m[i][0];
        mat.Matrix[i][1] = rowMat.m[i][1];
        mat.Matrix[i][2] = rowMat.m[i][2];
        mat.Matrix[i][3] = rowMat.m[i][3];
    }

    return mat;
}

Matrix4x4 Matrix4x4::CreateRotationZMatrix(float angle) noexcept
{
    // 回転行列を作成
    DirectX::XMMATRIX zMat = DirectX::XMMatrixRotationZ(angle);

    // 行優先に変換
    DirectX::XMFLOAT4X4 rowMat;
    DirectX::XMStoreFloat4x4(&rowMat, zMat);

    // Matrix4x4 に代入
    Matrix4x4 mat;
    for (int i = 0; i < 4; ++i)
    {
        mat.Matrix[i][0] = rowMat.m[i][0];
        mat.Matrix[i][1] = rowMat.m[i][1];
        mat.Matrix[i][2] = rowMat.m[i][2];
        mat.Matrix[i][3] = rowMat.m[i][3];
    }

    return mat;
}

Matrix4x4 Matrix4x4::CreateRotationYawPitchRollMatrix(float yaw, float pitch, float roll) noexcept
{
    // クォータニオンで回転を作成
    DirectX::XMVECTOR qua = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

    // クォータニオンから回転行列に変換
    DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationQuaternion(qua);

    // 行優先に変換
    DirectX::XMFLOAT4X4 rowMat;
    DirectX::XMStoreFloat4x4(&rowMat, rotationMat);

    // Matrix4x4 に代入
    Matrix4x4 mat;
    for (int i = 0; i < 4; ++i)
    {
        mat.Matrix[i][0] = rowMat.m[i][0];
        mat.Matrix[i][1] = rowMat.m[i][1];
        mat.Matrix[i][2] = rowMat.m[i][2];
        mat.Matrix[i][3] = rowMat.m[i][3];
    }

    return mat;
}