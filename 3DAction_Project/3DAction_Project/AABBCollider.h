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
// コライダー設定ヘッダー
#include "ColliderConfig.h"


// ===================================
// 構造体
// ===================================
// AABBコライダー構造体
struct AABBCollider
{
	// 最小位置 最大位置
	Vector3 min = {};
	Vector3 max = {};

	// コライダー設定
	ColliderPresetConfig config = {};

	// コライダー設定
	// 更新したAABBコライダーを返す
	const AABBCollider& UpdateAABB(const Vector3& _position, const Vector3& _size)
	{
		// サイズの半分を計
		Vector3 finalSize = Vector3(_size.x * config.sizeScale.x,
			_size.y * config.sizeScale.y, _size.z * config.sizeScale.z);
		Vector3 halfSize = finalSize * 0.5f;

		// 中心位置計算
		Vector3 centerPos = _position + config.offset;

		// 最小位置 最大位置を計算して設定
		min = centerPos - halfSize;
		max = centerPos + halfSize;

		return *this;
	}

	// コライダー更新
	const AABBCollider& UpdateAABB(const SRT& _srt)
	{
		// 当たり判定の半径
		Vector3 localExtent = (_srt.scale * config.sizeScale) * 0.5;

		// ローカル中心位置
		Vector3 rotatedCenter = _srt.rotation.RotateVector(config.offset);
		Vector3 worldCenter = _srt.position + rotatedCenter;

		// 回転行列の絶対値計算
		Matrix3x3 R = Matrix3x3::CreateRotationQuaternion_LH(_srt.rotation);
		Matrix3x3 absR = R.Abs();

		Vector3 worldExtent = absR * localExtent;

		// AABB
		min = worldCenter - worldExtent;
		max = worldCenter + worldExtent;

		return *this;
	}

};


// ====================================
// コライダー作成関数
// ====================================
inline AABBCollider CreateAABB(const Vector3& size)
{

	AABBCollider aabb;

	// 半分の大きさを計算
	const float halfWidth = size.x * 0.5f;
	const float halfHeight = size.y * 0.5f;
	const float halfDepth = size.z * 0.5f;

	// 最小位置 最大位置を計算して設定
	aabb.min = Vector3(-halfWidth, -halfHeight, -halfDepth);
	aabb.max = Vector3(halfWidth, halfHeight, halfDepth);

	return aabb;
}

inline AABBCollider CreateAABB(const Vector3& size, const Vector3& _offset, const Vector3& scale)
{
	// 基本のAABB作成
	AABBCollider aabb = CreateAABB(size);

	// オフセットとスケールを設定
	aabb.config.offset = _offset;
	aabb.config.sizeScale = scale;

	return aabb;
}
