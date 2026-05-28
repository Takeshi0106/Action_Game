

// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "CollisionSystemModule.h"
// 当たり判定ヘッダー
#include "AABBCollider.h"
// AABBとABB当たり判定
#include "AABB_AABB_Collision.h"

#if defined(_DEBUG) || defined(DEBUG)
// 文字列
#include <string>
// ログ出力
#include "ReportMessage.h"
#endif


// ======================================
// 静的メンバー変数定義
// ======================================
// 衝突判定関数テーブル初期化
CollisionSystemModule::CollisionFunc
CollisionSystemModule::collisionFuncTable[MAX_SHAPE_TYPE][MAX_SHAPE_TYPE] = {};


// ======================================
// コンストラクタ
// ======================================
CollisionSystemModule::CollisionSystemModule()
{
	// 衝突判定関数テーブル設定
	collisionFuncTable[ColliderShapeType::AABB][ColliderShapeType::AABB] = AABB_AABB_Check;
}


// ======================================
// 当たり判定チェック関数
// ======================================
bool CollisionSystemModule::CheckCollision(
	ColliderShapeType shapeA,
	void* colliderA,
	ColliderShapeType shapeB,
	void* colliderB)
{
	// 衝突判定関数取得
	CollisionFunc func = collisionFuncTable[shapeA][shapeB];

#if defined(_DEBUG) || defined(DEBUG)
	// 関数が設定されているかチェック
	if (func == nullptr) {
		ErrorLog::OutputToConsole(u8"衝突判定関数が未設定 A: " + 
			String::to_u8string(shapeA) +
			u8"B: " + 
			String::to_u8string(shapeB));
		return false;
	}
#endif

	// 衝突判定関数呼び出し
	return func(colliderA, colliderB);
}

// ======================================
// AABB同士の当たり判定
// ======================================
bool CollisionSystemModule::AABB_AABB_Check(void* _a, void* _b)
{
	// 型変換
	AABBCollider* _aCol = static_cast<AABBCollider*>(_a);
	AABBCollider* _bCol = static_cast<AABBCollider*>(_b);

	// 当たり判定計算
	return AABB_AABB_Collision(*_aCol, *_bCol);
}
