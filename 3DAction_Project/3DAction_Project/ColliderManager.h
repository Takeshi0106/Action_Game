#pragma once

// ===================================
// 【クラス概要】
// コライダー管理マネージャー
// ローカルのコライダー情報を管理する
// ===================================


// ===================================
// ヘッダー
// ===================================
// マネージャーヘッダー
#include "TemplateManager.h"
// 固定長整数型ヘッダー
#include <cstdint>
// AABBコライダーヘッダー
#include "AABBCollider.h"
// 計算ヘッダー
#include "Vector3.h"
#include "Quaternionh.h"

// コライダー計算モジュール
#include "CollisionSystemModule.h"


// ===================================
// 列挙型
// コライダータイプ
// ===================================
enum ColliderType
{
	AABB, // 軸平行バウンディングボックス
	OBB,  // 向き付きバウンディングボックス
	SPHERE, // 球体コライダー

	MAX_COLLIDEAR_TYPE
};


// ===================================
// コライダー情報構造体
// ===================================
struct ColliderInfo
{
	// コライダータイプ
	ColliderType type;
	// コライダーID
	uint32_t ID;
};


// ===================================
// クラス
// ===================================
class ColliderManager
{
private:
	// AABBコライダーマネージャー
	TemplateManager<AABBCollider> m_AABBManager;

	// コライダー判定モジュール
	CollisionSystemModule m_CollisionModule;

public:
	// コンストラクタ・デストラクタ
	ColliderManager() = default;
	~ColliderManager() = default;

	// コライダー追加
	ColliderInfo ColliderAdd(const std::string& _name, const AABBCollider _collider);

	// コライダーの当たり判定
	bool CheckCollision(
		const ColliderInfo& _aColInfo, const Vector3& _aPos, const Vector3& _aSize,
		const ColliderInfo& _bColInfo, const Vector3& _bPos, const Vector3& _bSize);

	// コライダー返す
	AABBCollider* GetAABBCollider(const uint32_t& ID) {
		return m_AABBManager.GetData(ID);
	}
};

