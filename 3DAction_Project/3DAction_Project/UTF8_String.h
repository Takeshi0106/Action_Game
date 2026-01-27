#pragma once

// ===============================================
// ヘッダー概要
// ゲーム層で使用する文字列定義
// エンコードをUTF-8に統一するために使用
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
#include <string>


// ===============================================
// 名前空間
// ===============================================
class String final
{
private:
	// 実態
	std::u8string m_String;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	String() = default;
	String(const char8_t* _str) : m_String(_str) {}
	String(const std::u8string& _str) : m_String(_str) {}
	String(std::u8string&& _str) noexcept : m_String(std::move(_str)) {}

	// --------------------------------
	// コピー・ムーブ
	// --------------------------------
	String(const String&) = default;
	String(String&&) noexcept = default;
	String& operator=(const String&) = default;
	String& operator=(String&&) noexcept = default;

	// --------------------------------
	// 比較演算子
	// --------------------------------
	bool operator==(const String& rhs) const noexcept
	{
		return m_String == rhs.GetU8String();
	}

	// --------------------------------
	// ゲッター
	// --------------------------------
	const std::u8string& GetU8String() const noexcept
	{
		return m_String;
	}
};
