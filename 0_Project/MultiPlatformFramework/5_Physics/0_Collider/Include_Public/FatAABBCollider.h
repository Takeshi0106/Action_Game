#pragma once
#include "AABBCollider.h"

// ====================================
// Fat AABBコライダー構造体
// ====================================
struct FatAABBCollider
{
	AABBCollider aabb = {};
	bool isUpdated = false;
};

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
	fataabb.min -= Vector3(padding, padding, padding);
	fataabb.max += Vector3(padding, padding, padding);

	FatAABBCollider fat;
	fat.aabb = fataabb;
	fat.isUpdated = true;

	return fat;
}
