#pragma once

// ==============================================
// コピーを禁止するクラス
// 生成・破棄は可能
// ==============================================

class NonCopyable
{
protected:
	// 自動生成しないことを避けるために明示する
	NonCopyable() = default;
	virtual ~NonCopyable() = default;

	// コンストラクタで代入禁止
	NonCopyable(const NonCopyable&) = delete;
	// 代入演算子禁止
	NonCopyable& operator=(const NonCopyable&) = delete;
};
