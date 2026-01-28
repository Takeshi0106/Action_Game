#pragma once

// =========================================
// 【関数群概要】
// セーブ・ロード時に共通で使用する関数
// 情報などをまとめたりする際に使用する
// 
// ＊注意
// 補助関数なのでヘッダーにインクルードしないように注意
// =========================================


// ==========================================
// ヘッダー
// ==========================================
#include <unordered_map> // 配列
#include <vector>        // 配列
#include "UTF8_String.h"
#include "UTF8_StringView.h"
#include "Hashed_String.h"


// ==========================================
// セーブ時の関数
// ==========================================
namespace SaveUtils {
	// データの種類とデータ文字列を受け取り、キーを付けて文字列にして返す
	String MakeTypeInfo(const String& dataType, const String& data, int spaceNumber);
	// データをブロックごとにまとめる データの名前がないブロック
	String FormatAnonymousBlock(const String& data, int spaceNumber);
	// データをブロックごとにまとめる　ブロック名を付ける
	String FormatBlock(const String& blockName, const int blockNumber, const String& data, int spaceNumber);
}


// ==========================================
// ロード時の関数
// ==========================================
namespace LoadUtils {
	// データの種類を渡して中身を返す関数
	bool ExtractTypeInfo(const StringView& data, const String& type, StringView& info);
	// 文字列を解析して、データの種類をキーにしてデータを配列に代入する String_Viewを返すため、元のdataがある場合使える
	std::unordered_map<Hashed_String, StringView> AllExtractTypeInfo(const StringView& data);
	// データ内から、ブロック名を取り出して、blockに代入する
	bool ExtractBlocks(const StringView& data, const String& blockName, StringView& block);
	// ブロック名が入っているデータを渡し、ブロック情報をブロックごとに配列に入れて返す関数
	bool ExtractSubBlocks(const StringView& data, const String& blockName, std::vector<StringView>& blocks);
}

