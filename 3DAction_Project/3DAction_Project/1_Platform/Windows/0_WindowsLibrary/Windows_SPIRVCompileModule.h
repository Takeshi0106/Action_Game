#pragma once

// =========================================
// 【クラス概要】
// Windows用 
// hlsl を SPIR-V にコンパイルを行うモジュール
// Windows のみで使用することを想定しています
// =========================================


// =========================================
// ヘッダー
// =========================================
// UTF8文字列ヘッダー
#include "../../../UTF8_String.h"


// =========================================
// クラス
// =========================================
class Windows_SPIRV_CompileModule final
{
private:
	// SPIR-Vコンパイルファイルパス
	const String& kSPIRVFolderPath;

	// SPIR-V 拡張子
	const String kSPIRVExtension = u8".spv";


public:
	// ---------------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------------
	Windows_SPIRV_CompileModule(String& _kSPIRVPath) : kSPIRVFolderPath(_kSPIRVPath) {}
	~Windows_SPIRV_CompileModule() = default;


	// -----------------------------------------
	// HLSL を SPIR-V に変換する
	// 相対パスで指定してください (拡張子も含む)
	// -----------------------------------------
	bool SPIRVCompile(const String& _hlslPath);
};

