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


// ===================================
// 構造体
// ===================================
// 木を再構築する際の葉ノードデータ
struct LeafData
{
	AABBCollider aabb;
	ObjectInfo info;
	Vector3 center;
};


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


	// --------------------------------
	// メンバー関数
	// --------------------------------
	// 最適な兄弟ノードを選択する関数
	uint32_t ChooseBestSibling(uint32_t current, const AABBCollider& aabb);
	// ノードの祖先ノードを更新する関数
	void UpdateAncestors(uint32_t index);
	// 再構築用サブツリー構築関数
	uint32_t BuildSubTree(std::vector<LeafData>& leaves, uint32_t start, uint32_t end);

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
		m_Nodes.reserve(size * 2);
	}

	// ノード追加関数
	uint32_t AddNode(const AABBCollider& aabb, const ObjectInfo& info);
	// ノード削除
	void Remove(uint32_t index);
    // AABB検索（候補取得）
    void Query(const AABBCollider& box, std::vector<ObjectInfo>& results);

	// ツリーを再構築する関数
	void RebuildTree();

};

