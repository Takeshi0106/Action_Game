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
// コライダー設定ヘッダー
#include "ColliderConfig.h"
// 固定型長整数ヘッダー
#include <cstdint>


// ===================================
// 構造体
// ===================================
struct AABBNode
{
    // このノードのAABB
    AABBCollider aabb;
    // 親ノード
    uint32_t parentIndex = UINT32_MAX;
    // 左ノード
    uint32_t leftIndex = UINT32_MAX;
    // 右ノード
    uint32_t rightIndex = UINT32_MAX;

    // オブジェクト識別
	ObjectInfo objectInfo = {};


	// ------------------------------
    // 関数
    // ------------------------------
    // コンストラクタ
    AABBNode(const AABBCollider _aabb)
        : aabb(_aabb)
    {}

    // 左右のノードかあるかチェック
    bool isLeaf() const
    {
        return leftIndex == UINT32_MAX && rightIndex == UINT32_MAX;
    }
};

