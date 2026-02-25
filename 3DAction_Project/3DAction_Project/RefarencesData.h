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
// 配列ヘッダー
#include <vector>


// =========================================
// 構造体定義
// =========================================
// シェーダー定数バッファ情報構造体
enum class VertexFormat : uint8_t
{
	None = 0,

    Float2,
    Float3,
    Float4,
    Uint4,
};

#if defined(DEBUG) || defined(_DEBUG)
// 書き出し用フォーマット文字列
inline const String kVertexFormatString[] = {
    u8"None",

    u8"Float2",
    u8"Float3",
    u8"Float4",
    u8"Uint4"
};

#endif


// 入力レイアウト構造体
struct Self_ILInfo
{
    // 入力レイアウトの名前
	String name;
    // 入力レイアウトのインデックス
	uint32_t index;
    // 入力レイアウトのフォーマット
	VertexFormat format;
};


// 定数バッファ構造体
struct Self_CBInfo
{
    // 定数バッファの名前
    String name;
	// 定数バッファのインデックス
	uint16_t index;
    // 定数バッファのサイズ
    uint32_t size;
};


// シェーダーリファレクション情報構造体
struct SelfReflectionInfo
{
    // シェーダー名
    String shaderName;

    // 入力レイアウト情報の配列
    std::vector<Self_ILInfo> ilInfos;
    // 定数バッファ情報の配列
    std::vector<Self_CBInfo> cbInfos;
};
