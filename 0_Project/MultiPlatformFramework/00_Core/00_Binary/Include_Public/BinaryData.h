#pragma once

// ==================================
// 【クラス概要】
// バイナリーデータクラス
// 実態を保持するバイナリーデータクラス
// ==================================
// ヘッダー
#include <vector>
#include <cstddef>
#include "BinaryView.h"


// ==================================
// クラス
// ==================================
class BinaryData
{    
private:
    // メンバー変数
    std::vector<std::byte> m_Data;

public:
    // コンストラクタ・デストラクタ
    BinaryData() = default;
	~BinaryData() = default;

	// バイナリーデータを書き込む
    void Write(const void* data, size_t size)
    {
        m_Data.resize(size);
        std::memcpy(m_Data.data(), data, size);
    }
    // バイナリービューから書き込む
    void Write(const BinaryView& view)
    {
        Write(view.GetData(), view.GetSize());
	}
	// テンプレート版書き込み
    template<class T>
    void Write(const T& value)
    {
        Write(&value, sizeof(T));
    }

	// 空かどうかのチェック
	bool IsEmpty() const noexcept { return m_Data.empty(); }

    // ゲッター
    BinaryView GetBinaryView() const noexcept { return BinaryView(m_Data.data(), m_Data.size()); }
	size_t GetSize() const noexcept { return m_Data.size(); }
	const std::byte* GetData() const noexcept { return m_Data.data(); }
};
