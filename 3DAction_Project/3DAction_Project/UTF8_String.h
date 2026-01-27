#pragma once

// ===============================================
// ヘッダー概要
// ゲーム層で使用する文字列定義
// エンコードをUTF-8に統一するために使用
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// 文字列
#include <string>
// アラサート
#include <cassert>

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
	// char8_t* を返す
	const char8_t* GetU8Char() const noexcept{
		return m_String.c_str();
	}

	// --------------------------------
	// ASCII を UTF-8 に変換する関数
	// --------------------------------
	static String FromASCII(const char* ascii)
	{
#if defined(_DEBUG) || defined(DEBUG)
		// 念のため、文字が 0x7F を超えていないかチェック
		for (const unsigned char* p =
			reinterpret_cast<const unsigned char*>(ascii);
			*p; p++)
		{
			assert(*p <= 0x7F && "String::FromASCII : non-ASCII character");
		}
#endif

		// 同じなのでそのまま変換
		return String(reinterpret_cast<const char8_t*>(ascii));
	}

	// ---------------------------------- 
	// 数字をStringに変換
	// ----------------------------------
	static String to_u8string(int n)
	{
		// ASCII に変換
		std::string s = std::to_string(n);
		// U8 に変換(エンコードが同じ互換)
		return String(reinterpret_cast<const char8_t*>(s.c_str()));
	}
	static String to_u8string(float n)
	{
		// ASCII に変換
		std::string s = std::to_string(n);
		// U8 に変換(エンコードが同じ互換)
		return String(reinterpret_cast<const char8_t*>(s.c_str()));
	}
};


// --------------------------------
// 文字列結合演算子
// --------------------------------
// String同士
inline String operator+(const String& lhs, const String& rhs) noexcept
{
	return String(lhs.GetU8String() + rhs.GetU8String());
}
// String + std::u8
inline String operator+(const String& lhs, const std::u8string& rhs) noexcept
{
	return String(lhs.GetU8String() + rhs);
}
// std::u8 + String
inline String operator+(const std::u8string& lhs, const String& rhs) noexcept
{
	return String(lhs + rhs.GetU8String());
}
// String + char8
inline String operator+(const String& lhs, const char8_t* rhs) noexcept
{
	return String(lhs.GetU8String() + std::u8string(rhs));
}
// char8 + String
inline String operator+(const char8_t* lhs, const String& rhs) noexcept
{
	return String(std::u8string(lhs) + rhs.GetU8String());
}
