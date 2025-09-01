
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
// 計算のためのヘッダー
#include <DirectXMath.h>
// 計算構造体ヘッダー
#include "Vector3.h"
#include "Quaternionh.h"


// ======================================
// プロトタイプ宣言
// ======================================
inline Matrix4x4 CreateMatrix4x4FromXMMATRIX(const DirectX::XMMATRIX& xmMat) noexcept; // XMMATRIXからMatrix4x4を作成する
inline DirectX::XMMATRIX CreateXMMATRIXFromMatrix4x4(const Matrix4x4& mat4x4) noexcept; // Matix4x4からXMMATRIXを作成する


// ======================================
// 行列作成
// 静的メンバー関数
// ======================================
// 平行行列作成
Matrix4x4 Matrix4x4::CreateTranslationMatrix_LH(const Vector3& pos) noexcept
{
	// 単位行列を作成
	Matrix4x4 mat = Matrix4x4::CreateIdentityMatrix();

	// 行優先で代入
	mat.Matrix[0][3] = pos.x;
	mat.Matrix[1][3] = pos.y;
	mat.Matrix[2][3] = pos.z;

	return mat;
}

// 回転行列
Matrix4x4 Matrix4x4::CreateRotationXMatrix_LH(float angle) noexcept
{
	// 回転行列を作成
	DirectX::XMMATRIX xMat = DirectX::XMMatrixRotationX(angle);

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(xMat);
}
Matrix4x4 Matrix4x4::CreateRotationYMatrix_LH(float angle) noexcept
{
	// 回転行列を作成
	DirectX::XMMATRIX yMat = DirectX::XMMatrixRotationY(angle);

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(yMat);
}
Matrix4x4 Matrix4x4::CreateRotationZMatrix_LH(float angle) noexcept
{
	// 回転行列を作成
	DirectX::XMMATRIX zMat = DirectX::XMMatrixRotationZ(angle);

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(zMat);
}

Matrix4x4 Matrix4x4::CreateRotationYawPitchRollMatrix_LH(float yaw, float pitch, float roll) noexcept
{
	// 回転行列を作成
	DirectX::XMMATRIX rotationMat = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(rotationMat);
}

// クォータニオンから行列を作成
Matrix4x4 Matrix4x4::CreateRotationQuaternion_LH(const Quaternion& quaternion) noexcept
{
	// クォータニオン作成
	DirectX::XMVECTOR qXMVec = DirectX::XMVectorSet(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
	
	// クォータニオンからXMMATRIX作成
	DirectX::XMMATRIX xmMat = DirectX::XMMatrixRotationQuaternion(qXMVec);
	
	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(xmMat);
}

// 拡大縮小行列
Matrix4x4 Matrix4x4::CreateScalingMatrix_LH(const Vector3& scale) noexcept
{
	// 単位行列を作成
	Matrix4x4 mat = CreateIdentityMatrix();

	// 行優先で代入
	mat.Matrix[0][0] = scale.x;
	mat.Matrix[1][1] = scale.y;
	mat.Matrix[2][2] = scale.z;

	return mat;
}

// 投影行列作成
Matrix4x4 Matrix4x4::CreateProjectionMatrix_LH(float fovY, float aspect, float zn, float zf) noexcept
{
	// 透視行列を作成
	DirectX::XMMATRIX projMat = DirectX::XMMatrixPerspectiveFovLH(fovY, aspect, zn, zf);

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(projMat);
}

Matrix4x4 Matrix4x4::CreateViewMatrix_LH(const Vector3& cameraPos, const Vector3& lookPoint, const Vector3& upDir) noexcept
{
	// カメラに位置
	DirectX::XMVECTOR cameraPosVec = DirectX::XMVectorSet(cameraPos.x, cameraPos.y, cameraPos.z, 0.0f);
	// カメラの方向
	DirectX::XMVECTOR lookPointVec = DirectX::XMVectorSet(lookPoint.x, lookPoint.y, lookPoint.z, 0.0f);
	// カメラの上方向
	DirectX::XMVECTOR upDirVec = DirectX::XMVectorSet(upDir.x, upDir.y, upDir.z, 0.0f);

	// カメラの行列作成
	DirectX::XMMATRIX viewMat = DirectX::XMMatrixLookAtLH(cameraPosVec, lookPointVec, upDirVec);
	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(viewMat);
}


// ================================================
// 計算関数
// ================================================
// 転置
Matrix4x4 Matrix4x4::Transpose() const noexcept
{
	// XMMATRIを作成
	DirectX::XMMATRIX mat = CreateXMMATRIXFromMatrix4x4(*this);

	// 転置してMatrix4x4を作成
	return CreateMatrix4x4FromXMMATRIX(DirectX::XMMatrixTranspose(mat));
}

// 逆行列
Matrix4x4 Matrix4x4::Inverse() const noexcept
{
	// XMMATRIXを作成
	DirectX::XMMATRIX mat = CreateXMMATRIXFromMatrix4x4(*this);

	//  逆行列を計算
	DirectX::XMVECTOR reverse;
	mat = DirectX::XMMatrixInverse(&reverse, mat);

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(mat);
}

// 行列式
float Matrix4x4::Determinant() const noexcept
{
	// XMMATRIXを作成
	DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix4x4(*this);

	// 行列式を計算
	DirectX::XMVECTOR detVec = XMMatrixDeterminant(xmMat);
	float det = DirectX::XMVectorGetX(detVec);

	return det;
}

// GPUに送るための変換をする行列
Matrix4x4 Matrix4x4::toGPU() const noexcept
{
	// DirectXは左手、列優先でOK
	return this->Transpose();
}

// 平行移動ベクトルを抽出
Vector3 Matrix4x4::ExtractTranslation() const noexcept
{
	return Vector3(
		Matrix[0][3],
		Matrix[1][3],
		Matrix[2][3]
	);
}

// 拡大縮小を抽出
Vector3 Matrix4x4::ExtractScaling() const noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix4x4(*this);

	// 軸ベクトルを取得して、長さを求めることで大きさが求まる
	float scaleX = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmMat.r[0]));
	float scaleY = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmMat.r[1]));
	float scaleZ = DirectX::XMVectorGetX(DirectX::XMVector3Length(xmMat.r[2]));

	return Vector3(
		scaleX,
		scaleY,
		scaleZ
	);
}

Quaternion Matrix4x4::ExtractQuaternion() const noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix4x4(*this);

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
// 加算演算子
Matrix4x4 operator+(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix4x4(mat2);
	DirectX::XMMATRIX result = {};

	// １行ずつ足し算
	for (int i = 0; i < 4; i++)
	{
		result.r[i] = DirectX::XMVectorAdd(m1.r[i], m2.r[i]);
	}

	return CreateMatrix4x4FromXMMATRIX(result);
}

// 減算演算子
Matrix4x4 operator-(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix4x4(mat2);
	DirectX::XMMATRIX result = {};

	// １行ずつ減算
	for (int i = 0; i < 4; i++)
	{
		result.r[i] = DirectX::XMVectorSubtract(m1.r[i], m2.r[i]);
	}

	return CreateMatrix4x4FromXMMATRIX(result);
}

// 乗算演算子
Matrix4x4 operator*(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix4x4(mat2);

	// 乗算
	DirectX::XMMATRIX result = XMMatrixMultiply(m1, m2);

	return CreateMatrix4x4FromXMMATRIX(result);
}

Matrix4x4 operator*(const Matrix4x4& mat, const Quaternion& q) noexcept
{
	// クォータニオン行列を作成
	Matrix4x4 qMat = Matrix4x4::CreateRotationQuaternion_LH(q);

	// 行列の掛け算
	return mat* qMat;
}

Vector3   operator*(const Matrix4x4& mat1, Vector3 vec1) noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(vec1.x, vec1.y, vec1.z, 1.0f);

	// 乗算
	DirectX::XMVECTOR result = XMVector3Transform(vec, m1);

	return Vector3(
		DirectX::XMVectorGetX(result),
		DirectX::XMVectorGetY(result),
		DirectX::XMVectorGetZ(result)
	);
}

Matrix4x4 operator*(const Matrix4x4& mat1, float scalar) noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX m = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(scalar);

	// 行ベクトルにスカラーを掛ける
	DirectX::XMMATRIX result = {};
	for (int i = 0; i < 4; i++)
	{
		result.r[i] = DirectX::XMVectorScale(m.r[i], scalar);
	}

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(result);
}

Matrix4x4 operator/(const Matrix4x4& mat1, float scalar) noexcept
{
	float inv = 1.0f / scalar;

	// XMMATRIX作成
	DirectX::XMMATRIX m = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(inv);

	// 乗算
	DirectX::XMMATRIX result = {};
	for (int i = 0; i < 4; i++)
	{
		result.r[i] = DirectX::XMVectorScale(m.r[i], inv);
	}

	// Matrix4x4作成
	return CreateMatrix4x4FromXMMATRIX(result);
}

bool operator==(const Matrix4x4& mat1, const Matrix4x4& mat2) noexcept
{
	// XMMATRIX作成
	DirectX::XMMATRIX m1 = CreateXMMATRIXFromMatrix4x4(mat1);
	DirectX::XMMATRIX m2 = CreateXMMATRIXFromMatrix4x4(mat2);

	for (int i = 0; i < 4; i++)
	{
		// 行ベクトルごとの比較
		DirectX::XMVECTOR cmp = DirectX::XMVectorEqual(m1.r[i], m2.r[i]);

		// 比較チェック
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
// XMMATRIXからMatrix4x4を作成する
inline Matrix4x4 CreateMatrix4x4FromXMMATRIX(const DirectX::XMMATRIX& xmMat) noexcept
{
	// 行列を作成
	Matrix4x4 mat;

	// 行を取り出して代入
	for (int i = 0; i < 4; i++)
	{
		const DirectX::XMVECTOR& row = xmMat.r[i];
		mat.Matrix[i][0] = DirectX::XMVectorGetX(row);
		mat.Matrix[i][1] = DirectX::XMVectorGetY(row);
		mat.Matrix[i][2] = DirectX::XMVectorGetZ(row);
		mat.Matrix[i][3] = DirectX::XMVectorGetW(row);
	}

	return mat;
}

// Matrix4x4からXMMATRIXを作成する
inline DirectX::XMMATRIX CreateXMMATRIXFromMatrix4x4(const Matrix4x4& mat4x4) noexcept
{
	// XMMATRI作成
	DirectX::XMMATRIX xmMat = DirectX::XMMatrixSet(
		mat4x4.Matrix[0][0], mat4x4.Matrix[0][1], mat4x4.Matrix[0][2], mat4x4.Matrix[0][3],
		mat4x4.Matrix[1][0], mat4x4.Matrix[1][1], mat4x4.Matrix[1][2], mat4x4.Matrix[1][3],
		mat4x4.Matrix[2][0], mat4x4.Matrix[2][1], mat4x4.Matrix[2][2], mat4x4.Matrix[2][3],
		mat4x4.Matrix[3][0], mat4x4.Matrix[3][1], mat4x4.Matrix[3][2], mat4x4.Matrix[3][3]
	);

	return xmMat;
}