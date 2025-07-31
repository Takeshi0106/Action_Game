#pragma once
#include <string>
#include <vector>

// ============================================================
// リファレンスを行う
// リファレンスしたデータを書き出す・読み込む関数群
// ============================================================

// =================================================
// 構造体
// =================================================

// 定数バッファの情報構造体
struct ConstantBufferInfo {
	std::string name = "";  // 定数バッファの名前
	int registerNumber = 0; // レジスタ番号
	size_t size = 0;        // 大きさ１６の倍数にする
};

// シェーダー　の情報構造体
struct ShaderInfo {
	std::string shaderName = "";          // シェーダーの名前
	std::vector<ConstantBufferInfo> info; // 定数バッファ情報
};


// ==================================================
// 関数
// ==================================================
// リファレンスしたシェーダー外部情報を読込み
bool ShaderInfoInput(const char* kShader_ConstantInfoPath, std::vector<ShaderInfo>& outShaderInfo);
// 外部ファイルを引き数のstring に代入する関数
bool LoadFile(const char* path, std::string& outContent);
// Stringを渡して、配列に代入する処理
bool ParseShaderInfo(const std::string_view& dataView, std::vector<ShaderInfo>& outShaderInfo);

#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用関数　デバッグ時のみれふぁレクションを行う

// リフレクションした情報をShaderInfo配列に入れる
bool Reflect(void* blob, size_t blobsize, std::vector<ConstantBufferInfo>& outbuffer);
// リファレンスしたシェーダー情報配列を外部ファイルに出力する
bool ShaderInfoOutput(const char* kShaderInfoPath, std::vector<ShaderInfo>& shaderInfo);
#endif