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
	}

	// ノード追加関数
	uint32_t AddNode(const AABBCollider& _aabb, const ObjectInfo& _info);
	// ノード削除
	void Remove(uint32_t _index);
    // AABB検索（候補取得）
    void Query(const AABBCollider& _box, std::vector<ObjectInfo>& _results);

	// ツリーを再構築する関数
	void RebuildTree();

};

