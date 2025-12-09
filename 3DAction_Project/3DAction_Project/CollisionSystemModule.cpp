

// ======================================
// ヘッダー
// ======================================
#include "CollisionSystemModule.h"


// ======================================
// AABB同士の当たり判定
// ======================================
bool CollisionSystemModule::CheckCollision(
	const AABBCollider& _aCol, const AABBCollider& _bCol)
{
	// 当たり判定
	if (_aCol.max.x < _bCol.min.x ||
		_aCol.min.x > _bCol.max.x) {
		return false;
	}
	if (_aCol.max.y < _bCol.min.y ||
		_aCol.min.y > _bCol.max.y) {
		return false;
	}
	if (_aCol.max.z < _bCol.min.z ||
		_aCol.min.z > _bCol.max.z) {
		return false;
	}

	return true;
}
