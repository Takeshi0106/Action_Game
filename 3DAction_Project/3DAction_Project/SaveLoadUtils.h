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
#include <string>        // 文字列
#include <unordered_map> // 配列
#include <vector>        // 配列


// ==========================================
// セーブ時の関数
// ==========================================
namespace SaveUtils {
	// データの種類とデータ文字列を受け取り、キーを付けて文字列にして返す
	std::string MakeTypeInfo(const std::string& dataType, const std::string& data, int spaceNumber);
	// データをブロックごとにまとめる データの名前がないブロック
	std::string FormatAnonymousBlock(const std::string& data, int spaceNumber);
	// データをブロックごとにまとめる　ブロック名を付ける
	std::string FormatBlock(const std::string& blockName, const int blockNumber, const std::string& data, int spaceNumber);
}


// ==========================================
// ロード時の関数
// ==========================================
namespace LoadUtils {
	// 1行を渡して、データの中身を返す関数
	std::string ExtractTypeInfoLine(const std::string& line);
	// 文字列を解析して、データの種類をキーにしてデータを配列に代入する
	std::unordered_map<std::string, std::string> AllExtractTypeInfo(const std::string& data);
	// ブロック名が入っているデータを渡し、ブロック情報をブロックごとに配列に入れて返す関数
	bool ExtractBlocks(const std::string& data, const std::string& blockName, std::vector<std::string>& blocks);
}

