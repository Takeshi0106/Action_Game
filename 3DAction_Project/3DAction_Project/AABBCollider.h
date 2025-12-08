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


// ===================================
// クラス
// ===================================
struct AABBCollider
{
	// 最小位置 最大位置
	Vector3 min = {};
	Vector3 max = {};

	// オフセット
	Vector3 offset = {};
	Vector3 offsetSize = {};


	// 更新したAABBコライダーを返す
	const AABBCollider UpdateAABB(const Vector3& _position, const Vector3& _size) const
	{
		// 更新後のコライダー
		AABBCollider updateCollider;

		// サイズの半分を計
		Vector3 finalSize = Vector3(_size.x * offsetSize.x,
			_size.y * offsetSize.y, _size.z * offsetSize.z);
		Vector3 halfSize = finalSize * 0.5f;

		// 中心位置計算
		Vector3 centerPos = _position + offset;

		// 最小位置 最大位置を計算して設定
		updateCollider.min = centerPos - halfSize;
		updateCollider.max = centerPos + halfSize;

		return updateCollider;
	}
};


// ====================================
// コライダー作成関数
// ====================================
inline AABBCollider CreateAABB(const float width, const float height, const float depth)
{
	AABBCollider aabb;

	// 半分の大きさを計算
	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;
	const float halfDepth = depth * 0.5f;

	// 最小位置 最大位置を計算して設定
	aabb.min = Vector3(-halfWidth, -halfHeight, -halfDepth);
	aabb.max = Vector3(halfWidth, halfHeight, halfDepth);

	return aabb;
}
