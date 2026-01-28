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


// ===============================================
// 前方宣言
// ===============================================
class StringView;
class BinaryView;


// ===============================================
// 名前空間
// ===============================================
class String final
{
private:
	// --------------------------------
	// 実態
	// --------------------------------
	std::u8string m_String;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	String() = default;
	String(const char8_t* _str);
	String(const std::u8string& _str);
	String(std::u8string&& _str) noexcept;
	String(StringView& strView) noexcept;
	String(const char8_t* data, size_t length) noexcept;

	// --------------------------------
	// コピー・ムーブ
	// --------------------------------
	String(const String&) = default;
	String(String&&) noexcept = default;
	String& operator=(const String&) = default;
	String& operator=(String&&) noexcept = default;
	String& operator=(const	StringView& strView) noexcept;


	// --------------------------------
	// 比較演算子
	// --------------------------------
	bool operator==(const String& rhs) const noexcept;

	// --------------------------------
	// 代入演算子
	// --------------------------------
	String& operator+=(const String& rhs) noexcept;
	String& operator+=(const std::u8string& rhs) noexcept;
	String& operator+=(const char8_t* rhs) noexcept;

	// --------------------------------
	// 空文字列かどうか
	// --------------------------------
	const bool IsEmpty() const noexcept;

	// --------------------------------
	// ASCII を UTF-8 に変換する関数
	// --------------------------------
	static String FromASCII(const char* ascii);

	// ---------------------------------- 
	// 数字をStringに変換
	// ----------------------------------
	static String to_u8string(int n);
	static String to_u8string(uint64_t n);
	static String to_u8string(float n);

	// --------------------------------
	// ゲッター
	// --------------------------------
	const std::u8string& GetU8String() const noexcept;
	// char8_t* を返す
	const char8_t* GetU8Char() const noexcept;
	// バイナリデータとして取得
	BinaryView GetBinaryView() const noexcept;
	// サイズ
	const size_t GetSize() const noexcept;
};


// --------------------------------
// 文字列結合演算子
// --------------------------------
// String同士
String operator+(const String& lhs, const String& rhs) noexcept;
// String + std::u8
String operator+(const String& lhs, const std::u8string& rhs) noexcept;
// std::u8 + String
String operator+(const std::u8string& lhs, const String& rhs) noexcept;
// String + char8
String operator+(const String& lhs, const char8_t* rhs) noexcept;
// char8 + String
String operator+(const char8_t* lhs, const String& rhs) noexcept;
