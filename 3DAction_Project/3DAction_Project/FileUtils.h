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


// ==================================
// 前方宣言
// ==================================
// 実態を持つバイナリーデータクラス
class BinaryData;
// バイナリーデータ参照クラス
class BinaryView;


// ==================================
// 関数
// ==================================
namespace FileUtis {
	// ------------------------------------
	// テキストファイル IO
	// ------------------------------------
	namespace Text {
		// 書き出し
		bool WriteTextFile(const String& filePath, const String& data);
		// 読み込むファイル (文字列として読み込む)
		bool ReadTextFile(const String& path, String& data);
	}

	
	// ------------------------------------
	// バイナリーデータ IO
	// ------------------------------------
	namespace Binary {
		// 書き出し
		bool WriteBinaryFile(const String& filePath, const BinaryView& data);
		// 読込
		bool ReadBinaryFile(const String& filePath, BinaryData& data);
	}
}

