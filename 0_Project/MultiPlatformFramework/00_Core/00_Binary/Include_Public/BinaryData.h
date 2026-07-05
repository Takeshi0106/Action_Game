#pragma once

// ==================================
// 【クラス概要】
// バイナリーデータクラス
// 実態を保持するバイナリーデータクラス
// ==================================


// ==================================
// ヘッダー
// ==================================
// 配列ヘッダー
#include <vector>
// バイナリーデータ参照クラス
#include <cstddef>
// バイナリーデータ参照クラス
#include "BinaryView.h"


// ==================================
// クラス
// ==================================
class BinaryData
{    
private:
    // --------------------------------
    // 実体データ
    // --------------------------------
    std::vector<std::byte> m_Data;


public:
    // --------------------------------
    // コンストラクタ・デストラクタ
    // --------------------------------
    BinaryData() = default;
	~BinaryData() = default;


    // --------------------------------
    // 書き込み
    // --------------------------------
	// バイナリーデータを書き込む
    void Write(const void* data, size_t size)
    {
        // 既存データを上書き
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


    // --------------------------------
	// 空かどうかのチェック
    // --------------------------------
	bool IsEmpty() const noexcept { return m_Data.empty(); }


    // --------------------------------
    // ゲッター
    // --------------------------------
	// バイナリーデータ参照を取得
    BinaryView GetBinaryView() const noexcept { return BinaryView(m_Data.data(), m_Data.size()); }
	// データサイズを取得
	size_t GetSize() const noexcept { return m_Data.size(); }
	// データポインタを取得
	const std::byte* GetData() const noexcept { return m_Data.data(); }
};
