#pragma once

// ==================================
// 【関数群概要】
// 外部ファイルに読込書出しをする共通関数
// 
// *文字列を読込み、書き込み
// ===================================


// ==================================
// ヘッダー
// ==================================
// 文字列
#include <string> // 文字列
#include <string_view> // 文字列参照


// ==================================
// 関数
// ==================================
namespace FileUtis {
	// 書き出すファイル
	bool WriteFile(const std::string& filePath, const std::string_view& data);
	// 読み込むファイル
	bool ReadFile(const std::string& path, std::string& data);
}

