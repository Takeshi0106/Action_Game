
// ===================================
// ヘッダー
// ===================================
// 必須ヘッダー
#include "AABBTree.h"
// スタック
#include <stack>
// アルゴリズム
#include <algorithm>

#if defined(DEBUG) || defined(_DEBUG)
#include <string>
#include "ReportMessage.h"
#endif


// ===================================
// 前方宣言
// ===================================
// ２つのAABBを内包するAABBを作成
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

	// 登録したAABBと近い場所の葉を探す
	uint32_t sibling = ChooseBestSibling(m_RootNodeIndex, aabb);

	// -------------------------------
	// 新しい親ノードを作る
	// -------------------------------
	// 近いAABBの親を保存
	uint32_t oldParent = m_Nodes[sibling].parentIndex;
	// 登録したAABBと近いAABBを内包するAABBを作成
	AABBCollider mergedAABB = CreateMargeAABB(m_Nodes[sibling].aabb, aabb);
	// AABBノード作成
	AABBNode parentNode(mergedAABB);
	// 左右と親を設定
	parentNode.leftIndex = sibling;
	parentNode.rightIndex = currentIndex;
	// 前の親を親に設定
	parentNode.parentIndex = oldParent;

	// 配列に追加
	m_Nodes.push_back(parentNode);
	// 新しく追加した親ノードのインデックスを取得
	uint32_t newParentIndex = static_cast<uint32_t>(m_Nodes.size() - 1);

	// 親を新しいノードに更新
	m_Nodes[sibling].parentIndex = newParentIndex;
	m_Nodes[currentIndex].parentIndex = newParentIndex;

	// 前の親ノードの左右を更新
	if (oldParent != UINT32_MAX)
	{
		// 前のノードが左か右かで判定
		if (m_Nodes[oldParent].leftIndex == sibling) 
		{
			m_Nodes[oldParent].leftIndex = newParentIndex;
		}
		else 
		{
			m_Nodes[oldParent].rightIndex = newParentIndex;
		}
	}
	else
	{
		// ルートノードを更新
		m_RootNodeIndex = newParentIndex;
	}

	// 祖先ノードをすべて更新
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
}


// ==================================
// AABB検索（候補取得）
// ==================================
void AABBTree::Query(const AABBCollider& box, std::vector<ObjectInfo>& results)
{
	if (m_RootNodeIndex == -1) return;

	std::stack<uint32_t> stack;
	stack.push(m_RootNodeIndex);
	uint32_t count = 0;
	uint32_t downCount = 0;

	while (!stack.empty())
	{
		uint32_t index = stack.top();
		stack.pop();
		count++;

		const AABBNode& node = m_Nodes[index];

		// 衝突判定チェック
		if (!CheckAABBCollision(box, node.aabb))
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

			downCount++;
		}
	}

	DebugLog::OutputToConsole(("カウント: " + std::to_string(count) + " 深さ: " + std::to_string(downCount)).c_str());
}


// ===================================
// ツリーを再構築する関数
// ===================================
void AABBTree::RebuildTree()
{
	// ノードを一時保存
	std::vector<LeafData> temporary;
	// AABBの数取得
	size_t aabbCount = m_Nodes.size();
	// 一応、すべてのAABB数分確保
	temporary.reserve(aabbCount);

	for (const auto& n : m_Nodes)
	{
		// 葉だけ取得する
		if (n.isLeaf())
		{
			LeafData data;
			data.aabb = n.aabb;
			data.info = n.objectInfo;
			data.center = (n.aabb.min + n.aabb.max) * 0.5f;

			// 配列に保存
			temporary.push_back(data);
		}
	}

	// 葉の数を取得
	size_t leafCount = temporary.size();

	// 初期化する
	m_Nodes.clear();
	m_Nodes.reserve(aabbCount);
	m_RootNodeIndex = UINT32_MAX;

	// 葉がなければ戻る
	if (leafCount == 0) {
		return;
	}

	// 最構築
	uint32_t root = BuildSubTree(temporary, 0, static_cast<uint32_t>(leafCount));
	m_RootNodeIndex = root;
}


// ===================================
// 最適な兄弟ノードを選択する関数
// ===================================
uint32_t AABBTree::ChooseBestSibling(uint32_t current, const AABBCollider& aabb)
{
	// 葉が見つかるまで探索
	while (!m_Nodes[current].isLeaf())
	{
		// 左右のノードを内包したAABBを作成
		AABBCollider leftcol = CreateMargeAABB(m_Nodes[m_Nodes[current].leftIndex].aabb, aabb);
		AABBCollider rightcol = CreateMargeAABB(m_Nodes[m_Nodes[current].rightIndex].aabb, aabb);

		// 表面積を計算
		float leftCost = SurfaceArea(leftcol);
		float rightCost = SurfaceArea(rightcol);

		// コストが小さいほうの枝に移動する
		current = (leftCost < rightCost) ? m_Nodes[current].leftIndex : m_Nodes[current].rightIndex;
	}

	return current;
}


// ===================================
// ノードの祖先ノードをすべて更新する関数
// ===================================
void AABBTree::UpdateAncestors(uint32_t index)
{
	// 親ルードをたどりながら更新
	while (index != UINT32_MAX)
	{
		// 現在のノードを取得
		AABBNode& node = m_Nodes[index];

		// 子ノードがある場合
		if (!node.isLeaf())
		{
			// AABBを再計算
			node.aabb = CreateMargeAABB(m_Nodes[node.leftIndex].aabb, m_Nodes[node.rightIndex].aabb);
		}

		// 親ノードへ移動
		index = node.parentIndex;
	}
}


// ====================================
// 再構築用サブツリー構築関数
// ====================================
uint32_t AABBTree::BuildSubTree(std::vector<LeafData>& leaves, uint32_t start, uint32_t end)
{
	// 葉の数を計算
	uint32_t count = end - start;

	// 葉ノードが１つの場合
	if (count == 1) 
	{
		// 葉ノードを作成
		AABBNode leaf(leaves[start].aabb);
		leaf.objectInfo = leaves[start].info;

		// 親ノード・子ノードは未設定
		leaf.parentIndex = UINT32_MAX;
		leaf.leftIndex = UINT32_MAX;
		leaf.rightIndex = UINT32_MAX;

		// 配列に保存
		m_Nodes.push_back(leaf);
		// 添え字を返す
		return static_cast<uint32_t>(m_Nodes.size() - 1);
	}

	// 分割軸を計算
	Vector3 minC = leaves[start].center;
	Vector3 maxC = leaves[start].center;

	// 最小・最大位置を計算
	for (uint32_t i = start + 1; i < end; ++i)
	{
		minC.x = std::min(minC.x, leaves[i].center.x);
		minC.y = std::min(minC.y, leaves[i].center.y);
		minC.z = std::min(minC.z, leaves[i].center.z);

		maxC.x = std::max(maxC.x, leaves[i].center.x);
		maxC.y = std::max(maxC.y, leaves[i].center.y);
		maxC.z = std::max(maxC.z, leaves[i].center.z);
	}

	// 幅を検索
	Vector3 extent = maxC - minC;
	int axis = 0;

	// 最も広い軸を選択
	if (extent.y > extent.x && extent.y >= extent.z) { 
		axis = 1; 
	}
	else if (extent.z > extent.x && extent.z > extent.y) { 
		axis = 2; 
	}

	// 中央で分割
	uint32_t mid = start + count / 2;

	// 軸に基づいてソートするラムダ式
	auto cmp = [axis](const LeafData& a, const LeafData& b) 
		{
		if (axis == 0) return a.center.x < b.center.x;
		if (axis == 1) return a.center.y < b.center.y;
		return a.center.z < b.center.z;};

	// ソート
	std::nth_element(leaves.begin() + start, leaves.begin() + mid, leaves.begin() + end, cmp);

	// 左右を再帰構築
	uint32_t leftIdx = BuildSubTree(leaves, start, mid);
	uint32_t rightIdx = BuildSubTree(leaves, mid, end);

	// 親ノードを作成
	AABBCollider merged;
	merged = CreateMargeAABB(m_Nodes[leftIdx].aabb, m_Nodes[rightIdx].aabb);

	AABBNode parent(merged);
	parent.leftIndex = leftIdx;
	parent.rightIndex = rightIdx;
	parent.parentIndex = UINT32_MAX;

	// 配列に追加
	m_Nodes.push_back(parent);
	uint32_t parentIdx = static_cast<uint32_t>(m_Nodes.size() - 1);

	// 子に parent をセット
	m_Nodes[leftIdx].parentIndex = parentIdx;
	m_Nodes[rightIdx].parentIndex = parentIdx;

	return parentIdx;
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
