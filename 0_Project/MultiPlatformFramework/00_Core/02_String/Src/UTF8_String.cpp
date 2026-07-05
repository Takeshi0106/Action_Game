
// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "UTF8_String.h"
// 文字列参照
#include "UTF8_StringView.h"
// バイナリビュー
#include "BinaryView.h"

#if defined(_DEBUG) || defined(DEBUG)
// アラサート
#include <cassert>
#endif


// =================================
// コンストラクタ
// =================================
String::String(const char8_t* _str) : m_String(_str) {}
String::String(const std::u8string& _str) : m_String(_str) {}	
String::String(std::u8string&& _str) noexcept : m_String(std::move(_str)) {}
String::String(StringView& strView) noexcept : m_String(strView.GetData(), strView.GetSize()) {}
String::String(const char8_t* data, size_t length) noexcept : m_String(data, length) {}


// ==================================
// View をコピー
// ==================================
String& String::operator=(const	StringView& strView) noexcept 
{
	m_String.assign(strView.GetData(), strView.GetSize());
	return *this;
}


// ==================================
// 比較演算子
// ==================================
bool String::operator==(const String& rhs) const noexcept
{
	return m_String == rhs.GetU8String();
}


// --------------------------------
// 代入演算子
// --------------------------------
String& String::operator+=(const String& rhs) noexcept
{
	m_String = m_String + rhs.GetU8String();
	return *this;
}
String& String::operator+=(const std::u8string& rhs) noexcept
{
	m_String = m_String + rhs;
	return *this;
}
String& String::operator+=(const char8_t* rhs) noexcept
{
	m_String = m_String + std::u8string(rhs);
	return *this;
}


// ==================================
// 文字列が空かどうか
// ==================================
bool String::IsEmpty() const noexcept
{
	return m_String.empty();
}

// ==================================
// 文字列をクリアする
// ==================================
void String::Clear() noexcept
{
	m_String.clear();
}


// ==================================
// ASCIIをutf8に変換する
// ==================================
String String::FromASCII(const char* ascii)
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


// --------------------------------
// ゲッター
// --------------------------------
const std::u8string& String::GetU8String() const noexcept
{
	return m_String;
}
// char8_t* を返す
const char8_t* String::GetU8Char() const noexcept 
{
	return m_String.c_str();
}
// char* を返す
const char* String::GetChar() const noexcept
{
	return reinterpret_cast<const char*>(m_String.c_str());
}
// バイナリデータとして取得
BinaryView String::GetBinaryView() const noexcept
{
	return BinaryView(
		m_String.data(),
		m_String.size() * sizeof(char8_t));
}
// サイズを返す
size_t String::GetSize() const noexcept
{
	return m_String.size();
}


// ====================================
// 演算子
// ====================================
// String + String
String operator+(const String& lhs, const String& rhs) noexcept
{
	return String(lhs.GetU8String() + rhs.GetU8String());
}
// String + std::u8
String operator+(const String& lhs, const std::u8string& rhs) noexcept
{
	return String(lhs.GetU8String() + rhs);
}
// std::u8 + String
String operator+(const std::u8string& lhs, const String& rhs) noexcept
{
	return String(lhs + rhs.GetU8String());
}
// String + char8
String operator+(const String& lhs, const char8_t* rhs) noexcept
{
	return String(lhs.GetU8String() + std::u8string(rhs));
}
// char8 + String
String operator+(const char8_t* lhs, const String& rhs) noexcept
{
	return String(std::u8string(lhs) + rhs.GetU8String());
}
