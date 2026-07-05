#pragma once

// ===================================
// 【構造体概要】
//  コライダーの設定
// ===================================


// ===================================
// ヘッダー
// ===================================
// 計算ヘッダー
#include "Vector3.h"
// 固定長整数ヘッダー
#include <cstdint>


// ===================================
// 列挙型
// ===================================
// コライダー形状タイプ
enum ColliderShapeType : uint8_t
{
	AABB = 0,

	MAX_SHAPE_TYPE
};

// タグ
enum ObjectTag : uint32_t
{
	NOTAG = 0,
	PLAYER = 1 << 0,
	ENEMY = 1 << 1,
	ITEM = 1 << 2,
};
 

// ===================================
// 構造体
// ===================================
struct ObjectInfo
{
	// オブジェクトID
	uint32_t objectID = UINT32_MAX;
	// タグ
	ObjectTag tag = ObjectTag::NOTAG;
};


// コライダー設定構造体
struct ColliderPresetConfig
{
	// オフセット
	Vector3 offset = {};
	// サイズスケール
	Vector3 sizeScale = {};

	// コライダーの形状タイプ
	ColliderShapeType shapeType;

	// オブジェクト情報
	ObjectInfo objectInfo = {};
};
