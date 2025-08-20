#pragma once

#include <string>
#include <vector>

// ============================================================
// リフレクションを行う
// リフレクションしたデータを書き出す・読み込む関数群
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

// 入力レイアウト構造体
struct InputLayoutInfo
{
	std::string semanticName = ""; // セマンティックの名前
	int semanticIndex = 0;         // セマンティックの番号
	int inputSlot = 0;             // スロット番号
	int format = 0;                // データの形式 (DXGI)
};

// シェーダーの情報構造体
struct ShaderInfo {
	std::string shaderName = "";            // シェーダーの名前
	std::vector<ConstantBufferInfo> CBInfo; // 定数バッファ情報
	std::vector<InputLayoutInfo> ILInfo;    // 入力レイアウトの情報
};


// ==================================================
// 関数
// ==================================================
// リファレンスしたシェーダー外部情報を読込み
bool ShaderInfoInput(const char* kShaderInfoPath, std::vector<ShaderInfo>& outShaderInfo);

#if defined(DEBUG) || defined(_DEBUG)
// デバッグ用関数　デバッグ時のみを行う

// リフレクションした情報をShaderInfo配列に入れる
bool Reflect(void* blob, size_t blobsize, std::vector<ConstantBufferInfo>& CBInfo, std::vector<InputLayoutInfo>& ILInfo);
// リファレンスしたシェーダー情報配列を外部ファイルに出力する
bool ShaderInfoOutput(const char* kShaderInfoPath, std::vector<ShaderInfo>& shaderInfo);
#endif