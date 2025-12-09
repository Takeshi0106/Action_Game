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
// コライダー設定
#include "ColliderConfig.h"


// ===================================
// クラス
// ===================================
class CollisionSystemModule
{
private:
	// 関数ポインタの型を定義
	using CollisionFunc = bool(*)(void*, void*);
	// 衝突判定関数テーブル
	static CollisionFunc collisionFuncTable[MAX_SHAPE_TYPE][MAX_SHAPE_TYPE];


	// --------------------------------
	// 仮想関数
	// --------------------------------
	// AABB同士の当たり判定
	static bool AABB_AABB_Check(void* a, void* b);

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	CollisionSystemModule();
	~CollisionSystemModule() = default;


	// --------------------------------
	// 当たり判定チェック関数
	// --------------------------------
	bool CheckCollision(
		ColliderShapeType shapeA,
		void* colliderA,
		ColliderShapeType shapeB,
		void* colliderB);

};

