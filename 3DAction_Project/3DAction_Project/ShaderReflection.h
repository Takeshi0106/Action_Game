#pragma once

#include <string>
#include <vector>
#include "ShaderStructs.h"

// ============================================================
// リフレクションを行う
// リフレクションしたデータを書き出す・読み込む関数群
// ============================================================


// =================================================
// 構造体
// =================================================
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