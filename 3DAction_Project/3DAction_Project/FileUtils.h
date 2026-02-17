#pragma once

// ==================================
// 【関数群概要】
// 外部ファイルにStringを読込書出しをする共通関数
// 
// 読込は全てUTF-8として読み込む
// ＊ファイルのエンコードがUTF-8でない場合、文字化けします
// 　チェックは行いません。
// ===================================


// ==================================
// ヘッダー
// ==================================
// 文字列
#include "UTF8_String.h"
#include "BinaryView.h"


// ==================================
// 関数
// ==================================
namespace FileUtis {
	// 書き出すファイル
	bool WriteStringFile(const String& filePath, const BinaryView& data);
	// 読み込むファイル (文字列として読み込む)
	bool ReadStringFile(const String& path, String& data);
}

