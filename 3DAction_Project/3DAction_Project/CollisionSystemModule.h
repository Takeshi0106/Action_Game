#pragma once

// ===================================
// 【クラス概要】
// コリジョンシステムモジュール
// コライダーの判定を行うモジュール
// ===================================


// ===================================
// ヘッダー
// ===================================
// 計算ヘッダー
#include "Vector3.h"
// 当たり判定ヘッダー
#include "AABBCollider.h"


// ===================================
// クラス
// ===================================
class CollisionSystemModule
{
public:
	// AABB同士の当たり判定
	bool CheckCollision(
		const AABBCollider& _aCol, const Vector3& _aPos, const Vector3& _aSize,
		const AABBCollider& _bCol, const Vector3& _bPos, const Vector3& _bSize);

};

