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
