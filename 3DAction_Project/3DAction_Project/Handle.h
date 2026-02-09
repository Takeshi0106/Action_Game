#pragma once

// ===================================
// 【構造体概要】
// ハンドル構造体
// ===================================


// ===================================
// ヘッダー
// ===================================
// 固定長整数ヘッダー
#include <cstdint>


// ===================================
// 構造体定義
// ===================================
struct Handle
{
	// 添え字
	uint32_t index = UINT32_MAX;
	// 世代
	uint32_t generation = UINT32_MAX;


	// ===================================
	// 有効チェック
	// ===================================
	bool IsValid() const
	{
		return (index != UINT32_MAX) && (generation != UINT32_MAX);
	}
};


// ===================================
// 比較演算子オーバーロード
// ===================================
inline bool operator==(const Handle& lhs, const Handle& rhs)
{
	return (lhs.index == rhs.index) && (lhs.generation == rhs.generation);
}
