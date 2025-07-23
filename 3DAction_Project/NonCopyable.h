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
	~NonCopyable() = default;


	NonCopyable(const NonCopyable&) = delete; // コピーコンストラクタ禁止


	NonCopyable& operator=(const NonCopyable&) = delete; // コピー演算子を禁止
};

