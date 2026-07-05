#pragma once

// =================================================
// シェーダーのコンパイル関数をまとめたもの
// バイナリーデータの作成、書出し、読込み
// バイナリーデータの読込みはFileUtlisを使用していません
// 
// * ヘッダーに重たいものが含まれているため
// ヘッダーにインクルードしないようにしてください
// =================================================


// ==========================================
// ヘッダー
// ==========================================
#include <d3dcompiler.h>  // シェーダーをコンパイルするためのヘッダー
#include <filesystem>     // ファイルパスなどを楽に扱える　C++17以降
#include "UTF8_String.h"


// =========================================
// グローバル変数 シェーダーの拡張子
// =========================================
const String kShaderExtension = u8".hlsl"; // コンパイル前の拡張子
const String kCompileExtension = u8".cso"; // コンパイル後の拡張子


// =========================================
// 関数
// =========================================
namespace ShaderCompilerUtils
{
	// シェーダーをコンパイルして外部ファイルに書き出し引き数のblobにバイナリデータを入れる
	bool OutputCompileShader(const std::filesystem::path kFilePath, const std::filesystem::path name,
		const String& entryPoint, const String& shaderTypeModel, ID3DBlob** blob);

	// パスから.csoを読み込んでblobにバイナリーデータを入れる関数
	bool LoadCompiledShader(const std::filesystem::path& csoPath, ID3DBlob** blob);
}
