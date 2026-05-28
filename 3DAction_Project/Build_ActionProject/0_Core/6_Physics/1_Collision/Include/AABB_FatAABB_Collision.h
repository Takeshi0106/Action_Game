#pragma once

// =============================
// ヘッダー
// =============================
#include "AABBCollider.h"
#include "FatAABBCollider.h"
#include "AABB_AABB_Collision.h"

// =============================
// 当たり判定
// =============================
inline bool AABB_FatAABB_Collision(const AABBCollider& _aabb, const FatAABBCollider& _fataabb)
{
	return AABB_AABB_Collision(_aabb, _fataabb.aabb);
}
