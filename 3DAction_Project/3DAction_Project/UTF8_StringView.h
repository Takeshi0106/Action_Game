#pragma once

// ===============================================
// 【クラス概要】
// UTF-8文字列参照クラス
// std::u8string_viewのラッパークラス
// 書き換え不可能
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
	// デフォルトコンストラクタは禁止
    constexpr StringView() noexcept = delete;

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
    // ゲッター
    // --------------------------------
    constexpr const char8_t* data() const noexcept { return m_View.data(); }
    constexpr size_t size() const noexcept { return m_View.size(); }
    constexpr bool empty() const noexcept { return m_View.empty(); }

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
};
