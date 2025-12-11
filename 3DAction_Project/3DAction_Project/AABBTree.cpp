
// ===================================
// ヘッダー
// ===================================
// 必須ヘッダー
#include "AABBTree.h"
// スタック
#include <stack>


// ===================================
// 前方宣言
// ===================================
// ２つのAABBをマージするAABB作成
inline AABBCollider CreateMargeAABB(const AABBCollider& a, const AABBCollider& b);
// AABBの表面積計算
inline float SurfaceArea(AABBCollider& col);


// ===================================
// ノード追加関数
// ===================================
uint32_t AABBTree::AddNode(const AABBCollider& aabb, const ObjectInfo& info)
{
	// ノード作成
	AABBNode node(aabb);
	node.objectInfo = info;

	// 配列に追加
	m_Nodes.emplace_back(node);
	// 今のインデックス
	uint32_t currentIndex = static_cast<uint32_t>(m_Nodes.size() - 1);

	// ルートノードが未設定の場合は設定
	if (m_RootNodeIndex == UINT32_MAX)
	{
		m_RootNodeIndex = currentIndex;
		return currentIndex;
	}

	// 挿入場所を探す
	uint32_t sibling = ChooseBestSibling(currentIndex, aabb);

	// 新しい親ノードを作る
	uint32_t oldParent = m_Nodes[sibling].parentIndex;
	AABBCollider mergedAABB = CreateMargeAABB(m_Nodes[sibling].aabb, aabb);

	AABBNode parentNode(mergedAABB);
	parentNode.leftIndex = sibling;
	parentNode.rightIndex = currentIndex;
	parentNode.parentIndex = oldParent;

	// 配列に追加
	m_Nodes.push_back(parentNode);
	uint32_t newParentIndex = static_cast<uint32_t>(m_Nodes.size() - 1);

	// 親を更新
	m_Nodes[sibling].parentIndex = newParentIndex;
	m_Nodes[currentIndex].parentIndex = newParentIndex;

	// 兄弟ノードの親を更新
	if (oldParent != UINT32_MAX)
	{
		if (m_Nodes[oldParent].leftIndex == sibling)
			m_Nodes[oldParent].leftIndex = newParentIndex;
		else
			m_Nodes[oldParent].rightIndex = newParentIndex;
	}
	else
	{
		// ルートノードを更新
		m_RootNodeIndex = newParentIndex;
	}

	// 祖先ノードを更新
	UpdateAncestors(newParentIndex);

	// 追加したノードのインデックスを返す
	return currentIndex;
}


// ===================================
// ノード削除関数
// ===================================
void AABBTree::Remove(uint32_t index)
{
	// 親ノードを取得
	uint32_t parentIndex = m_Nodes[index].parentIndex;

	// ルートノードだったら終了
	if (parentIndex == UINT32_MAX)
	{
		m_RootNodeIndex = -1;
		return;
	}

	// 親ノードを取得
	uint32_t grandParent = m_Nodes[parentIndex].parentIndex;
	int sibling = (m_Nodes[parentIndex].leftIndex == index) ? m_Nodes[parentIndex].rightIndex : m_Nodes[parentIndex].leftIndex;

	if (grandParent != UINT32_MAX)
	{
		if (m_Nodes[grandParent].leftIndex == parentIndex)
			m_Nodes[grandParent].leftIndex = sibling;
		else
			m_Nodes[grandParent].rightIndex = sibling;
		m_Nodes[sibling].parentIndex = grandParent;
		UpdateAncestors(grandParent);
	}
	else
	{
		m_RootNodeIndex = sibling;
		m_Nodes[sibling].parentIndex = -1;
	}

	// 注: nodes配列はそのまま、フリーリスト管理すると高速化可能
}

// ==================================
// AABB検索（候補取得）
// ==================================
void AABBTree::Query(const AABBCollider& box, std::vector<ObjectInfo>& results)
{
	if (m_RootNodeIndex == -1) return;

	std::stack<uint32_t> stack;
	stack.push(m_RootNodeIndex);

	while (!stack.empty())
	{
		int index = stack.top();
		stack.pop();

		const AABBNode& node = m_Nodes[index];

		// 衝突判定チェック
		if (CheckAABBCollision(box, node.aabb))
		{
			continue;
		}

		if (node.isLeaf())
		{
			// オブジェクト情報を接触配列に代入
			results.push_back(node.objectInfo);
		}
		else
		{
			stack.push(node.leftIndex);
			stack.push(node.rightIndex);
		}
	}
}

// ===================================
// ノードの祖先ノードを更新する関数
// ===================================
void AABBTree::UpdateAncestors(uint32_t index)
{
	int current = index;

	while (current != UINT32_MAX)
	{
		AABBNode& node = m_Nodes[current];
		if (!node.isLeaf())
		{
			// AABBを再計算
			node.aabb = CreateMargeAABB(m_Nodes[node.leftIndex].aabb, m_Nodes[node.rightIndex].aabb);
		}
		current = node.parentIndex;
	}
}


// ===================================
// 最適な兄弟ノードを選択する関数
// ===================================
uint32_t AABBTree::ChooseBestSibling(uint32_t current, const AABBCollider& aabb)
{
	// 葉ノードを探す
	while (!m_Nodes[current].isLeaf())
	{
		// AABBをマージして計算する
		AABBCollider leftcol = CreateMargeAABB(m_Nodes[m_Nodes[current].leftIndex].aabb, aabb);
		AABBCollider rightcol = CreateMargeAABB(m_Nodes[m_Nodes[current].rightIndex].aabb, aabb);

		float leftCost = SurfaceArea(leftcol);
		float rightCost = SurfaceArea(rightcol);

		// コストが小さいほうの枝に移動する
		current = (leftCost < rightCost) ? m_Nodes[current].leftIndex : m_Nodes[current].rightIndex;
	}
	return current;
}


// ====================================
//  ２つのAABBをマージするAABB作成
// ====================================
inline AABBCollider CreateMargeAABB(const AABBCollider& a, const AABBCollider& b)
{
	// マージしたAABB
	AABBCollider mergedAABB;

	// マージ計算
	mergedAABB.min.x = std::min(a.min.x, b.min.x);
	mergedAABB.min.y = std::min(a.min.y, b.min.y);
	mergedAABB.min.z = std::min(a.min.z, b.min.z);
	mergedAABB.max.x = std::max(a.max.x, b.max.x);
	mergedAABB.max.y = std::max(a.max.y, b.max.y);
	mergedAABB.max.z = std::max(a.max.z, b.max.z);

	return mergedAABB;
}


// ====================================
// AABBの表面積計算
// ====================================
inline float SurfaceArea(AABBCollider& col)
{
	Vector3 diff = col.max - col.min;
	return 2.0f * (diff.x * diff.y + diff.y * diff.z + diff.z * diff.x);
}
