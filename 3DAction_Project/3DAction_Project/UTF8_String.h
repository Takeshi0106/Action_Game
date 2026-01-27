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
};
