#pragma once

// ===================================
// 【構造体概要】
//  軸平行バウンディングボックスの衝突判定クラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// 計算ヘッダー
#include "Vector3.h"
#include "SRT.h"
#include "Matrix3x3.h"


// ===================================
// 構造体
// ===================================
// AABBコライダー構造体
struct AABBCollider
{
	// 最小位置 最大位置
	Vector3 min = {};
	Vector3 max = {};
};

// Fat AABBコライダー構造体
struct FatAABBCollider
{
	AABBCollider aabb = {};
	bool isUpdated = false;
};

// ====================================
// AABBコライダー作成関数
// ====================================
inline AABBCollider CreateAABB(
	const SRT& _srt,
	const Vector3& offset = Vector3(0.0f, 0.0f, 0.0f),
	const Vector3& sizeScale = Vector3(1.0f, 1.0f, 1.0f))
{
	AABBCollider col;

	// 当たり判定の半径
	Vector3 localExtent = (_srt.scale * sizeScale) * 0.5;

	// ローカル中心位置
	Vector3 localCenter = offset * _srt.scale;
	Vector3 rotatedCenter = _srt.rotation.RotateVector(localCenter);

	// ワールド中心位置を計算
	Vector3 worldCenter = _srt.position + rotatedCenter;

	// 回転行列の絶対値計算

	Matrix3x3 R = Matrix3x3::CreateRotationQuaternion_LH(_srt.rotation);
	Matrix3x3 absR = R.Abs();

	// ワールドの拡張ベクトル計算
	Vector3 worldExtent = absR * localExtent;

	// AABB
	col.min = worldCenter - worldExtent;
	col.max = worldCenter + worldExtent;

	return col;
}


// ====================================
// ファットAABB作成関数
// ====================================
inline FatAABBCollider CreateFatAABB(const AABBCollider& aabb, 
	const Vector3& velocity,
	const float lookAheadTime = 1.0f,
	const float padding = 0.1f)
{
	AABBCollider fataabb = aabb;

	Vector3 expand = velocity * lookAheadTime;
	fataabb.min -= expand;
	fataabb.max += expand;

	// さらに少し余裕を持たせる
	fataabb.min -= Vector3(padding,padding,padding);
	fataabb.max += Vector3(padding,padding,padding);

	FatAABBCollider fat;
	fat.aabb = fataabb;
	fat.isUpdated = true;

	return fat;
}


// =====================================
// AABB同士の当たり判定関数
// =====================================
inline bool CheckAABBCollision(const AABBCollider& a, const AABBCollider& b)
{
	// 各軸で判定
	if (a.max.x < b.min.x || a.min.x > b.max.x) { return false; }
	if (a.max.y < b.min.y || a.min.y > b.max.y) { return false; }
	if (a.max.z < b.min.z || a.min.z > b.max.z) { return false; }

	// 全ての軸で重なっている
	return true;
}


// =====================================
// AABB内包判定関数
// =====================================
inline bool IsAABBInside(const AABBCollider& inner, const FatAABBCollider& outer)
{
	// 各軸で判定
	if (inner.min.x < outer.aabb.min.x || inner.max.x > outer.aabb.max.x) { return false; }
	if (inner.min.y < outer.aabb.min.y || inner.max.y > outer.aabb.max.y) { return false; }
	if (inner.min.z < outer.aabb.min.z || inner.max.z > outer.aabb.max.z) { return false; }

	// 全ての軸で内包している
	return true;
}
