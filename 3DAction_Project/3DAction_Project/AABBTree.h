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
	uint32_t index = UINT32_MAX;
	Vector3 center = {};
};

// 葉の深度構造体
struct TreeBalance
{
	// 葉ノードの平均深さ
	float averageDepth = 0.0f;
	// 葉ノードの最大深さ
	uint32_t maxDepth = 0;
};

// ハンドル構造体
struct AABBTreeHandle
{
	// 添え字
	uint32_t index = UINT32_MAX;
	// 世代
	uint32_t generation = 0;
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
	// 世代
	std::vector<uint32_t> m_Generations;
	// 今からのノードID
	std::vector<uint32_t> m_FreeNodeIDs;

	// ルートノードインデックス
	uint32_t m_RootNodeIndex = UINT32_MAX;


	// --------------------------------
	// メンバー関数
	// --------------------------------
	// 最適な兄弟ノードを選択する関数
	uint32_t ChooseBestSibling(uint32_t _current, const AABBCollider& _aabb);
	// ノードの祖先ノードをすべて更新する関数
	void UpdateAncestors(uint32_t _index);
	// 再構築用サブツリー構築関数
	uint32_t BuildSubTree(std::vector<LeafData>& _leaves, uint32_t _start, uint32_t _end);

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
	void Reserve(const uint32_t _size)
	{
		m_Nodes.reserve(_size * 2);
		m_Generations.reserve(_size * 2);
		m_FreeNodeIDs.reserve(_size * 2);
	}

	// ノード追加関数
	const AABBTreeHandle AddNode(const AABBCollider& _aabb, const ObjectInfo& _info);
	// ノード削除
	void Remove(const AABBTreeHandle& _handle);
    // AABB検索（候補取得）
    void Query(const AABBCollider& _box, std::vector<ObjectInfo>& _results);

	// ツリーを再構築する関数
	void RebuildTree();
	// ツリーの深度チェック
	TreeBalance CalculateBalance() const;
};

