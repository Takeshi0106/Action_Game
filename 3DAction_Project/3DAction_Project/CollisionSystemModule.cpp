

// ======================================
// ヘッダー
// ======================================
#include "CollisionSystemModule.h"


// ======================================
// AABB同士の当たり判定
// ======================================
bool CollisionSystemModule::CheckCollision(
	const AABBCollider& _aCol, const Vector3& _aPos, const Vector3& _aSize,
	const AABBCollider& _bCol, const Vector3& _bPos, const Vector3& _bSize)
{
	// AABBコライダーを更新
	AABBCollider aabbA = _aCol.UpdateAABB(_aPos, _aSize);
	AABBCollider aabbB = _bCol.UpdateAABB(_bPos, _bSize);

	// 当たり判定
	if (aabbA.max.x < aabbB.min.x || 
		aabbA.min.x > aabbB.max.x) {
		return false;
	}
	if (aabbA.max.y < aabbB.min.y || 
		aabbA.min.y > aabbB.max.y) {
		return false;
	}
	if (aabbA.max.z < aabbB.min.z || 
		aabbA.min.z > aabbB.max.z) {
		return false;
	}

	return true;
}
