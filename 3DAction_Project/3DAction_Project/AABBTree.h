#pragma once


// ===================================
// 【クラス概要】
// AABBツリークラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// AABBノードヘッダー
#include "AABBNode.h"
// 配列ヘッダー
#include <vector>
// 固定長整数ヘッダー
#include <cstdint>
// 当たり判定チェック
#include "CollisionSystemModule.h"


// ===================================
// クラス
// ===================================
class AABBTree
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// ノード配列
	std::vector<AABBNode> m_Nodes;
	// ルートノードインデックス
	uint32_t m_RootNodeIndex = UINT32_MAX;
	// コライダーシステム
	CollisionSystemModule m_System;


	// --------------------------------
	// メンバー関数
	// --------------------------------
	uint32_t ChooseBestSibling(uint32_t current, const AABBCollider& aabb);
	void UpdateAncestors(int index);

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	AABBTree() = default;
	~AABBTree() = default;


	// --------------------------------
	// ノード処理関数
	// --------------------------------
	// リザーブ関数
	void Reserve(const uint32_t size)
	{
		m_Nodes.reserve(size);
	}

	// ノード追加関数
	uint32_t AddNode(const AABBCollider& aabb, const uint32_t objectIndex);
	// ノード削除
	void Remove(uint32_t index);
    // AABB検索（候補取得）
    void Query(const AABBCollider& box, std::vector<uint32_t>& results);

};

