
// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "../../Matrix3x3.h"
// 計算のためのヘッダー
#include <DirectXMath.h>
#include <cmath>
// 計算構造体ヘッダー
#include "../../Vector3.h"
#include "../../Quaternionh.h"


// ==========================================
// プロトタイプ宣言
// ==========================================
inline Matrix3x3 CreateMatrix3x3FromXMMATRIX(const DirectX::XMMATRIX& xmMat) noexcept;
inline DirectX::XMMATRIX CreateXMMATRIXFromMatrix3x3(const Matrix3x3& mat3x3) noexcept;


// ===============================================
// 行列作成
// ===============================================
// 拡大縮小行列
Matrix3x3 Matrix3x3::CreateScalingMatrix_LH(const Vector3& scale) noexcept
{
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z);
    return CreateMatrix3x3FromXMMATRIX(xmMat);
}

// 回転行列
Matrix3x3 Matrix3x3::CreateRotationXMatrix_LH(float radAngle) noexcept
{
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixRotationX(radAngle);
    return CreateMatrix3x3FromXMMATRIX(xmMat);
}

Matrix3x3 Matrix3x3::CreateRotationYMatrix_LH(float radAngle) noexcept
{
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixRotationY(radAngle);
    return CreateMatrix3x3FromXMMATRIX(xmMat);
}

Matrix3x3 Matrix3x3::CreateRotationZMatrix_LH(float radAngle) noexcept
{
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixRotationZ(radAngle);
    return CreateMatrix3x3FromXMMATRIX(xmMat);
}

Matrix3x3 Matrix3x3::CreateRotationYawPitchRollMatrix_LH(float yaw, float pitch, float roll) noexcept
{
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
    return CreateMatrix3x3FromXMMATRIX(xmMat);
}

Matrix3x3 Matrix3x3::CreateRotationQuaternion_LH(const Quaternion& quaternion) noexcept
{
    // クォータニオン作成
    DirectX::XMVECTOR qXMVec = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);

    // クォータニオンからXMMATRIX作成
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixRotationQuaternion(qXMVec);

    // Matrix4x4作成
    return CreateMatrix3x3FromXMMATRIX(xmMat);
}


// ================================================
// 計算関数
// ================================================
Matrix3x3 Matrix3x3::Transpose() const noexcept
{
    DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix3x3(*this);
    DirectX::XMMATRIX xmTransposed = DirectX::XMMatrixTranspose(xmMat);
    return CreateMatrix3x3FromXMMATRIX(xmTransposed);
}

Matrix3x3 Matrix3x3::Inverse() const noexcept
{
    DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix3x3(*this);
    DirectX::XMMATRIX xmInversed = DirectX::XMMatrixInverse(nullptr, xmMat);
    return CreateMatrix3x3FromXMMATRIX(xmInversed);
}

float Matrix3x3::Determinant() const noexcept
{
    DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix3x3(*this);
    return DirectX::XMMatrixDeterminant(xmMat).m128_f32[0];
}

Matrix3x3 Matrix3x3::Abs() const noexcept
{
    Matrix3x3 result;
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; j++)
        {
            result.Matrix[i][j] = std::fabs(Matrix[i][j]);
        }
    }
    return result;
}

Matrix3x3 Matrix3x3::toGPU() const noexcept
{
    return *this;
}

// ===============================================
// 抽出
// ================================================
Vector3 Matrix3x3::ExtractScaling() const noexcept
{
    // 各列ベクトルの長さをスケールとして抽出
    float scaleX = std::sqrt(Matrix[0][0] * Matrix[0][0] + Matrix[1][0] * Matrix[1][0] + Matrix[2][0] * Matrix[2][0]);
    float scaleY = std::sqrt(Matrix[0][1] * Matrix[0][1] + Matrix[1][1] * Matrix[1][1] + Matrix[2][1] * Matrix[2][1]);
    float scaleZ = std::sqrt(Matrix[0][2] * Matrix[0][2] + Matrix[1][2] * Matrix[1][2] + Matrix[2][2] * Matrix[2][2]);
    return Vector3(scaleX, scaleY, scaleZ);
}

Quaternion Matrix3x3::ExtractQuaternion() const noexcept
{
    // XMMATRIX作成
    DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix3x3(*this);

    // 拡大縮小を取得
    Vector3 scale = ExtractScaling();

    // スケール除去
    xmMat.r[0] = DirectX::XMVectorDivide(xmMat.r[0], DirectX::XMVectorReplicate(scale.x));
    xmMat.r[1] = DirectX::XMVectorDivide(xmMat.r[1], DirectX::XMVectorReplicate(scale.y));
    xmMat.r[2] = DirectX::XMVectorDivide(xmMat.r[2], DirectX::XMVectorReplicate(scale.z));

    // 回転行列からクォータニオンに変換
    DirectX::XMVECTOR q = DirectX::XMQuaternionRotationMatrix(xmMat);

    return Quaternion(
        DirectX::XMVectorGetX(q),
        DirectX::XMVectorGetY(q),
        DirectX::XMVectorGetZ(q),
        DirectX::XMVectorGetW(q)
    );
}

// ==========================================
// 演算子
// ==========================================
Matrix3x3 operator+(const Matrix3x3& mat1, const Matrix3x3& mat2) noexcept
{
    DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix3x3(mat1);
    DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix3x3(mat2);
    DirectX::XMMATRIX result = {};
    for (int i = 0; i < 3; ++i)
    {
        result.r[i] = DirectX::XMVectorAdd(m1.r[i], m2.r[i]);
    }
    return CreateMatrix3x3FromXMMATRIX(result);
}

Matrix3x3 operator-(const Matrix3x3& mat1, const Matrix3x3& mat2) noexcept
{
    DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix3x3(mat1);
    DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix3x3(mat2);
    DirectX::XMMATRIX result = {};
    for (int i = 0; i < 3; ++i)
    {
        result.r[i] = DirectX::XMVectorSubtract(m1.r[i], m2.r[i]);
    }
    return CreateMatrix3x3FromXMMATRIX(result);
}

Matrix3x3 operator*(const Matrix3x3& mat1, const Matrix3x3& mat2) noexcept
{
    DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix3x3(mat1);
    DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix3x3(mat2);
    DirectX::XMMATRIX result = DirectX::XMMatrixMultiply(m1, m2);
    return CreateMatrix3x3FromXMMATRIX(result);
}

Matrix3x3 operator*(const Matrix3x3& mat, const Quaternion& q) noexcept
{
    // クォータニオン行列を作成
    Matrix3x3 qMat = Matrix3x3::CreateRotationQuaternion_LH(q);

    // 行列の掛け算
    return mat * qMat;
}

Vector3   operator*(const Matrix3x3& mat1, Vector3 vec1) noexcept
{
    return Vector3(
        mat1.Matrix[0][0] * vec1.x + mat1.Matrix[1][0] * vec1.y + mat1.Matrix[2][0] * vec1.z,
        mat1.Matrix[0][1] * vec1.x + mat1.Matrix[1][1] * vec1.y + mat1.Matrix[2][1] * vec1.z,
        mat1.Matrix[0][2] * vec1.x + mat1.Matrix[1][2] * vec1.y + mat1.Matrix[2][2] * vec1.z);
}

Matrix3x3 operator*(const Matrix3x3& mat1, float scalar) noexcept
{
    // XMMATRIX に変換
    DirectX::XMMATRIX m = CreateXMMATRIXFromMatrix3x3(mat1);
    DirectX::XMMATRIX result = {};

    // 行ごとに掛け算
    for (int i = 0; i < 3; ++i)
    {
        result.r[i] = DirectX::XMVectorScale(m.r[i], scalar);
    }

    // 自作行列に変換
    return CreateMatrix3x3FromXMMATRIX(result);
}

Matrix3x3 operator/(const Matrix3x3& mat1, float scalar) noexcept
{
    float inv = 1.0f / scalar;

    // XMMATRIX に変換
    DirectX::XMMATRIX m = CreateXMMATRIXFromMatrix3x3(mat1);
    DirectX::XMMATRIX result = {};

    // 行ごとに割り算
    for (int i = 0; i < 3; ++i)
    {
        result.r[i] = DirectX::XMVectorScale(m.r[i], inv);
    }

    // 変換
    return CreateMatrix3x3FromXMMATRIX(result);
}

bool operator==(const Matrix3x3& mat1, const Matrix3x3& mat) noexcept
{
    // XMMATRIXに変換
    DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix3x3(mat1);
    DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix3x3(mat);

    // チェック
    for (int i = 0; i < 3; ++i)
    {
        DirectX::XMVECTOR cmp = DirectX::XMVectorEqual(m1.r[i], m2.r[i]);
        if (DirectX::XMVectorGetX(cmp) == 0.0f ||
            DirectX::XMVectorGetY(cmp) == 0.0f ||
            DirectX::XMVectorGetZ(cmp) == 0.0f ||
            DirectX::XMVectorGetW(cmp) == 0.0f)
        {
            return false;
        }
    }
    return true;
}


// ===============================================
// 関数
// ================================================
// XMMATRIXからMatrix3x3を作成する
inline Matrix3x3 CreateMatrix3x3FromXMMATRIX(const DirectX::XMMATRIX& xmMat) noexcept
{
    // 行列を作成
    Matrix3x3 mat;

    // 行を取り出して代入
    for (int i = 0; i < 3; i++)
    {
        const DirectX::XMVECTOR& row = xmMat.r[i];
        mat.Matrix[0][i] = DirectX::XMVectorGetX(row);
        mat.Matrix[1][i] = DirectX::XMVectorGetY(row);
        mat.Matrix[2][i] = DirectX::XMVectorGetZ(row);
    }

    return mat;
}

// Matrix4x4からXMMATRIXを作成する
inline DirectX::XMMATRIX CreateXMMATRIXFromMatrix3x3(const Matrix3x3& mat3x3) noexcept
{
    // XMMATRI作成
    DirectX::XMMATRIX xmMat = DirectX::XMMatrixSet(
        mat3x3.Matrix[0][0], mat3x3.Matrix[1][0], mat3x3.Matrix[2][0], 0.0f,
        mat3x3.Matrix[0][1], mat3x3.Matrix[1][1], mat3x3.Matrix[2][1], 0.0f,
        mat3x3.Matrix[0][2], mat3x3.Matrix[1][2], mat3x3.Matrix[2][2], 0.0f,
        0.0f,                0.0f,                0.0f,                1.0f);

    return xmMat;
}
