#pragma once
// ===============================================
// クラス概要
// ハッシュ化された文字列定義
// コンストラクタ時にハッシュ値を計算し、以降はハッシュ値を返す
// コピーやムーブは可能だが、代入は禁止
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
#include "UTF8_String.h"


// ===============================================
// クラス
// ===============================================
class Hashed_String final
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// 文字列
	String m_String = u8"";
	// ハッシュ値
	size_t m_Hash = 0;

public:
	// --------------------------------
	// コンストラクタ
	// --------------------------------
	// デフォルトコンストラクタ
	Hashed_String() = default;

	explicit Hashed_String(const char8_t* _str)
		: m_String(_str), 
		m_Hash(std::hash<std::u8string>{}(m_String.GetU8String())) {}
	explicit Hashed_String(const String& _str)
		: m_String(_str), 
		m_Hash(std::hash<std::u8string>{}(m_String.GetU8String())) {}
	explicit Hashed_String(String&& _str) noexcept
		: m_String(std::move(_str)), 
		m_Hash(std::hash<std::u8string>{}(m_String.GetU8String())) {}
	Hashed_String(const Hashed_String&) = default;
	Hashed_String(Hashed_String&&) noexcept = default;

	// --------------------------------
	// コピー・ムーブ
	// --------------------------------
	// コピー・ムーブ代入禁止
	Hashed_String& operator=(const Hashed_String& str) = default;
	Hashed_String& operator=(Hashed_String&&) = default;

	// --------------------------------
	// 比較演算子
	// --------------------------------
	bool operator==(const Hashed_String& rhs) const noexcept
	{
		return m_Hash == rhs.GetHash() && m_String == rhs.GetString();
	}

	// --------------------------------
	// 初期化
	// --------------------------------
	void Clear() noexcept {
		m_String = String();
		m_Hash = 0;
	}

	// --------------------------------
	// ゲッター
	// --------------------------------
	// 文字列
	const String& GetString() const noexcept { return m_String; }
	// ハッシュ値
	const size_t& GetHash() const noexcept { return m_Hash; }
};


// ===============================================
// ハッシュ関数
// ===============================================
namespace std
{
	template<>
	struct hash<Hashed_String>
	{
		size_t operator()(const Hashed_String& s) const noexcept
		{
			return s.GetHash();
		}
	};
}
