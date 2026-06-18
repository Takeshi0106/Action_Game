#pragma once
// =====================================
// ヘッダー
// =====================================
#include "AABBCollider.h"

// =====================================
// AABB同士の当たり判定関数
// =====================================
inline bool AABB_AABB_Collision(const AABBCollider& _a, const AABBCollider& _b)
{
	// 各軸で判定
	if (_a.max.x < _b.min.x || _a.min.x > _b.max.x) { return false; }
	if (_a.max.y < _b.min.y || _a.min.y > _b.max.y) { return false; }
	if (_a.max.z < _b.min.z || _a.min.z > _b.max.z) { return false; }

	// 全ての軸で重なっている
	return true;
}
