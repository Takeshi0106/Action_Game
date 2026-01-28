#pragma once

// ===============================================
// 【クラス概要】
// UTF-8文字列参照クラス
// std::u8string_viewのラッパークラス
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// 文字列ヘッダー
#include <string>
// 文字列参照ヘッダー
#include <string_view>
// 自作文字列ヘッダー
#include "UTF8_String.h"


// ===============================================
// クラス
// ===============================================
class StringView
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
    std::u8string_view m_View;

public:
    // --------------------------------
    // コンストラクタ
    // --------------------------------
    constexpr StringView() noexcept = default;

    // char8_t* とサイズから作成
    constexpr StringView(const char8_t* str, size_t size) noexcept
        : m_View(str, size) {
    }
    // null終端文字列から作成
    StringView(const char8_t* str) noexcept
        : m_View(str, str ? std::char_traits<char8_t>::length(str) : 0) {
    }
    // std::u8string から作成
    StringView(const std::u8string& str) noexcept
        : m_View(str) {
    }
    // String から作成
    StringView(const class String& str) noexcept
        : m_View(str.GetU8String()) {
    }

    // --------------------------------
    // セット
    // --------------------------------
    constexpr void SetRemovePrefix(size_t n) noexcept
    {
        m_View.remove_prefix(n);
	}

    // --------------------------------
    // ゲッター
    // --------------------------------
    constexpr const char8_t* GetData() const noexcept { return m_View.data(); }
    constexpr size_t GetSize() const noexcept { return m_View.size(); }
    constexpr bool IsEmpty() const noexcept { return m_View.empty(); }
    constexpr size_t GetFind(const StringView& substr, size_t pos = 0) const noexcept
    {
        return m_View.find(substr.m_View, pos);
	}
    constexpr size_t GetFindFirstNotof(const StringView& charSet, size_t pos = 0) const noexcept
    {
        return m_View.find_first_not_of(charSet.m_View, pos);
    }
    constexpr StringView SubStr(size_t pos = 0, size_t count = std::u8string_view::npos) const noexcept
    {
        return StringView(m_View.substr(pos, count).data(), m_View.substr(pos, count).size());
	}

    // --------------------------------
    // インデックスアクセス
    // --------------------------------
    constexpr char8_t operator[](size_t i) const noexcept { return m_View[i]; }

    // --------------------------------
    // 比較演算子
    // --------------------------------
    bool operator==(const StringView& rhs) const noexcept
    {
        return m_View == rhs.m_View;
    }
    // =================================
    // 代入演算子
	// =================================
    StringView& operator=(const StringView& rhs) noexcept
    {
        m_View = rhs.m_View;
        return *this;
	}
};
