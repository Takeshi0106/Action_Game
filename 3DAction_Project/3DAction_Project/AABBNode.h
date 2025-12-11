#pragma once

// ===================================
// 【構造体】
// AABBノードクラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// AABBヘッダー
#include "AABBCollider.h"
// 固定型長整数ヘッダー
#include <cstdint>


// ===================================
// 構造体
// ===================================
struct AABBNode
{
    AABBCollider aabb;    // このノードのAABB
    uint32_t objectIndex = 0;    // オブジェクトのインデックス
    uint32_t parentIndex = UINT32_MAX;    // 親ノード
    uint32_t leftIndex = UINT32_MAX;      // 左子ノード
    uint32_t rightIndex = UINT32_MAX;     // 右子ノード


	// リーフノードかどうかを判定
    bool isLeaf() const { return leftIndex == UINT32_MAX && rightIndex == UINT32_MAX; }

    // コンストラクタ
    AABBNode(const AABBCollider _aabb, const uint32_t objIndex)
        : aabb(_aabb), objectIndex(objIndex),
        parentIndex(UINT32_MAX),
        leftIndex(UINT32_MAX),
        rightIndex(UINT32_MAX)
    {}
};

