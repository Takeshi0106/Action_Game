#pragma once

// ============================================================
// リフレクションを行う
// リフレクションしたデータを書き出す・読み込む関数群
// 
// ＊注意
// 補助関数群なのでヘッダーにインクルードしないでください
// ============================================================


// =====================================================
// ヘッダー
// =====================================================
// 文字列ヘッダー
#include <string>
// 配列ヘッダー
#include <vector>
// 情報を入れるクラス
#include "ConstantBufferInfo.h"
#include "InputLayoutInfo.h"
#include "ShaderInfo.h"


// ==================================================
// 関数
// ==================================================
namespace ShaderReflectionUtils
{
	// リファレンスしたシェーダー外部情報を読込み
	bool ShaderInfoInput(const char* kShaderInfoPath, std::vector<ShaderInfo>& outShaderInfo);

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用関数　デバッグ時のみを行う

	// リフレクションした情報をShaderInfo配列に入れる
	bool Reflect(void* blob, size_t blobsize, std::vector<ConstantBufferInfo>& CBInfo, std::vector<InputLayoutInfo>& ILInfo);
	// リファレンスしたシェーダー情報配列を外部ファイルに出力する
	bool ShaderInfoOutput(const char* kShaderInfoPath, std::vector<ShaderInfo>& shaderInfo);

#endif

}