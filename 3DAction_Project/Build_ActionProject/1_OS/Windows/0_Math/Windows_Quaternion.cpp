
// =================================
// DirectXMathを使用したクォータニオン
// =================================


// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "../../../Quaternionh.h" // 自分のヘッダー
// 計算ヘッダー
#include <DirectXMath.h> // DirectXの計算
// 計算構造体ヘッダー
#include "../../../Matrix4x4.h"
#include "../../../Vector3.h"


// ==================================
// プロトタイプ宣言
// ==================================
// XMVECTORからクォータニオンを作成
inline Quaternion CreateQuaternionFromXMVECTOR(const DirectX::XMVECTOR& quaternion) noexcept;
// Matrix4x4からXMMATRIXを作成
inline DirectX::XMMATRIX CreateXMMATRIXFromMatrix4x4(const Matrix4x4& mat4x4) noexcept;


// ==================================
// クォータニオン作成関数
// 静的メンバー関数
// ==================================
// 行列からクォータニオン作成
Quaternion Quaternion::CreateQuaternionFromMatrix(const Matrix4x4& mat) noexcept
{
	// クォータニオン作成
    DirectX::XMMATRIX xmMat = CreateXMMATRIXFromMatrix4x4(mat);
    DirectX::XMVECTOR quaternion = XMQuaternionRotationMatrix(xmMat);

	// Quaternion作成
	return CreateQuaternionFromXMVECTOR(quaternion);
}

// オイラー角から作成
Quaternion Quaternion::CreateQuaternionFromEuler(float pitch, float yaw, float roll) noexcept
{
	// クォータニオン作成
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);

	// Quaternion作成
	return CreateQuaternionFromXMVECTOR(quaternion);
}

// 軸と角度から作成
Quaternion Quaternion::CreateQuaternionFromAxisAngle(const Vector3& axis, float angle) noexcept
{
	// クォータニオン作成
	DirectX::XMVECTOR axisVec = DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0.0f);
	DirectX::XMVECTOR quaternion = DirectX::XMQuaternionRotationAxis(axisVec, angle);

	// Quaternion作成
	return CreateQuaternionFromXMVECTOR(quaternion);
}


// ===================================
// 計算関数
// ===================================
// 正規化
Quaternion Quaternion::Normalize() const noexcept
{
	// クォータニオン作成
	DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(x, y, z, w);

	// 正規化
	quaternion = DirectX::XMQuaternionNormalize(quaternion);

	// Quaternion作成
	return CreateQuaternionFromXMVECTOR(quaternion);
}

// 共役
Quaternion Quaternion::Conjugate() const noexcept
{
	// クォータニオン作成
	DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(x, y, z, w);
	
	// 共役クォータニオン計算
	quaternion = DirectX::XMQuaternionConjugate(quaternion);

	// Quaternion作成
	return CreateQuaternionFromXMVECTOR(quaternion);
}

// 逆クォータニオン
Quaternion Quaternion::Inverse() const noexcept
{
	// クォータニオン作成
	DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(x, y, z, w);
	
	// 逆クォータニオン計算
	quaternion = DirectX::XMQuaternionInverse(quaternion);

	// Quaaternion作成
	return CreateQuaternionFromXMVECTOR(quaternion);
}

// ベクトルを回転
Vector3 Quaternion::RotateVector(const Vector3& v) const noexcept
{
	// クォータニオンを正規化
	DirectX::XMVECTOR quaternion = DirectX::XMVectorSet(x, y, z, w);
	quaternion = DirectX::XMQuaternionNormalize(quaternion);

	// 逆クォータニオン（共役）を取得
	DirectX::XMVECTOR qConj = DirectX::XMQuaternionConjugate(quaternion);

	// 回転させるベクトル
	DirectX::XMVECTOR vec = DirectX::XMVectorSet(v.x, v.y, v.z, 0.0f);

	// 回転計算
	DirectX::XMVECTOR result = DirectX::XMQuaternionMultiply(
		DirectX::XMQuaternionMultiply(quaternion, vec),
		qConj);

	// 回転後のベクトル
	return Vector3(
		DirectX::XMVectorGetX(result),
		DirectX::XMVectorGetY(result),
		DirectX::XMVectorGetZ(result)
	);
}

// 指定した方向を向くクォータニオンを作成
Quaternion Quaternion::LookRotation(const Vector3& forward, const Vector3& up) noexcept
{
	// ゼロかチェック
	if (forward.Length() < 1e-6f)
	{
		return Quaternion();
	}

	// 念のため正規化
	Vector3 f = forward.Normalize();

	// 上ベクトル
	Vector3 u = up.Normalize();

	// 外積
	Vector3 r = f.Cross(u);

	// チェック
	if (r.Length() < 1e-6f)
	{
		// 前ベクトルと上ベクトルが平行の場合
		r = Vector3(0.0f, 0.0f, 1.0f).Cross(f);

		// 再チェック
		if (r.Length() < 1e-6f)
		{
			r = Vector3(1.0f, 0.0f, 0.0f);
		}
	}

	// 正規化
	r = r.Normalize();

	// 正しい Up を再計算
	u = r.Cross(f);

	// 行優先（Row-major）回転行列
	DirectX::XMMATRIX m(
		DirectX::XMVectorSet(r.x, r.y, r.z, 0.0f), // X軸
		DirectX::XMVectorSet(u.x, u.y, u.z, 0.0f), // Y軸
		DirectX::XMVectorSet(f.x, f.y, f.z, 0.0f), // Z軸
		DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f)
	);

	DirectX::XMVECTOR q =
		DirectX::XMQuaternionRotationMatrix(m);

	return CreateQuaternionFromXMVECTOR(q);
}


// Slerp補間
Quaternion Quaternion::Slerp(const Quaternion& target, float t) const noexcept
{
	// XMVECTORに変換
	DirectX::XMVECTOR q1 = DirectX::XMVectorSet(x, y, z, w);
	DirectX::XMVECTOR q2 = DirectX::XMVectorSet(target.x, target.y, target.z, target.w);
	// Slerp補間
	DirectX::XMVECTOR result = DirectX::XMQuaternionSlerp(q1, q2, t);
	// Quaternion作成
	return CreateQuaternionFromXMVECTOR(result);
}

// GPUに送るデータに変換
Quaternion Quaternion::toGPU() const noexcept
{
	return *this;
}

// ===================================
// 演算子
// ===================================
Quaternion operator*(const Quaternion& q1, const Quaternion& q2) noexcept
{
	// XMVECTOR に変換
	DirectX::XMVECTOR x1 = DirectX::XMVectorSet(q1.x, q1.y, q1.z, q1.w);
	DirectX::XMVECTOR x2 = DirectX::XMVectorSet(q2.x, q2.y, q2.z, q2.w);

	// 掛け算（回転の合成）
	DirectX::XMVECTOR result = DirectX::XMQuaternionMultiply(x1, x2);

	// Quaternion に戻す
	Quaternion qResult;
	qResult.x = DirectX::XMVectorGetX(result);
	qResult.y = DirectX::XMVectorGetY(result);
	qResult.z = DirectX::XMVectorGetZ(result);
	qResult.w = DirectX::XMVectorGetW(result);

	return qResult;
}



// ===================================
// 関数
// ===================================
// XMVECTORからクォータニオンを作成する
inline Quaternion CreateQuaternionFromXMVECTOR(const DirectX::XMVECTOR& quaternion) noexcept
{
	return Quaternion(
		DirectX::XMVectorGetX(quaternion),
		DirectX::XMVectorGetY(quaternion),
		DirectX::XMVectorGetZ(quaternion),
		DirectX::XMVectorGetW(quaternion)
	);
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
