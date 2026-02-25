#pragma once

// ========================================
// 【クラス概要】
//  シェーダーのリファレクション情報を保持するクラス
// ========================================


// =========================================
// ヘッダー
// =========================================
// 文字列ヘッダー
#include "UTF8_String.h"
// 固定整数ヘッダー
#include <cstdint>


// =========================================
// 構造体定義
// =========================================
// シェーダー定数バッファ情報構造体
enum class VertexFormat : uint8_t
{
    Float2 = 0,
    Float3,
    Float4,
    Uint4,
    ColorRGBA8,
};

#if defined(DEBUG) || defined(_DEBUG)
// 書き出し用フォーマット文字列
inline const String kVertexFormatString[] = {
    u8"Float2",
    u8"Float3",
    u8"Float4",
    u8"Uint4",
    u8"ColorRGBA8",
};

#endif


// 入力レイアウト構造体
struct InputLayoutInfo
{
    // 入力レイアウトの名前
	String name;
    // 入力レイアウトのインデックス
	uint32_t index;
    // 入力レイアウトのフォーマット
	VertexFormat format;
};


// 定数バッファ構造体
struct ConstantBufferInfo
{
    // 定数バッファの名前
    String name;
    // 定数バッファのサイズ
    uint32_t size;
};
