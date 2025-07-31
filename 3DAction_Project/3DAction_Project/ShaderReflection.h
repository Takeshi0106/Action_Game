#pragma once

#include <string>
#include <vector>

// ============================================================
// リファレンスを行う
// リファレンスしたデータを書き出す・読み込む関数群
// ============================================================


// ==================================================
// 前方宣言
// ==================================================


// =================================================
// 構造体
// =================================================

// 定数バッファの情報構造体
struct ConstantBufferInfo {
	std::string name = "";  // 定数バッファの名前
	int registerNumber = 0; // レジスタ番号
	size_t size = 0;        // 大きさ１６の倍数にする
};

// 入力レイアウト構造体
struct InputLayoutInfo
{
	std::string semanticName = "";
	int semanticIndex = 0;
	int inputSlot = 0;
	int format = 0;
};

// シェーダー　の情報構造体
struct ShaderInfo {
	std::string shaderName = "";          // シェーダーの名前
	std::vector<ConstantBufferInfo> CBInfo; // 定数バッファ情報
	std::vector<InputLayoutInfo> ILInfo; // 入力レイアウトの情報
};


// ==================================================
// 関数
// ==================================================
// リファレンスしたシェーダー外部情報を読込み
bool ShaderInfoInput(const char* kShader_ConstantInfoPath, std::vector<ShaderInfo>& outShaderInfo);

#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用関数　デバッグ時のみれふぁレクションを行う

// リフレクションした情報をShaderInfo配列に入れる
bool Reflect(void* blob, size_t blobsize, std::vector<ConstantBufferInfo>& CBInfo, std::vector<InputLayoutInfo>& ILInfo);
// リファレンスしたシェーダー情報配列を外部ファイルに出力する
bool ShaderInfoOutput(const char* kShaderInfoPath, std::vector<ShaderInfo>& shaderInfo);
#endif