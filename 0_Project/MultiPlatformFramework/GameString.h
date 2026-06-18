#pragma once

// ===============================================
// ヘッダー概要
// ゲーム層で使用する文字列定義
// エンコードをUTF-8に統一するために使用
// std::stringを使用できないようにする
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
#include <string>
#include <string_view>


// ===============================================
// 名前空間
// ===============================================
namespace Game
{
	//using UTF8_Char = char8_t;
	//using UTF8_String = std::u8string;
	//using UTF8_StringView = std::u8string_view;

	class String
	{
	private:
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
	};
}
