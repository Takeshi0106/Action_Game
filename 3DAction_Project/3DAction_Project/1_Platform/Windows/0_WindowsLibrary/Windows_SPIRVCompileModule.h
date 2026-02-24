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
	// HLSLファイルパス
	const String& kHlslFolderPath;
	// SPIR-Vコンパイルファイルパス
	const String& kSPIRVFolderPath;

	// DXC.exeのパス
	const String& kDxcPath;

	// HLSL拡張子
	const String kHlslExtension = u8".hlsl";
	// SPIR-V 拡張子
	const String kSPIRVExtension = u8".spv";


public:
	// ---------------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------------
	Windows_SPIRV_CompileModule(
		const String& _hlslPath, 
		const String& _spirvPath,
		const String& _dxcPath) :
		kHlslFolderPath(_hlslPath),
		kSPIRVFolderPath(_spirvPath),
	    kDxcPath(_dxcPath){
	}
	~Windows_SPIRV_CompileModule() = default;


	// -----------------------------------------
	// HLSL を SPIR-V に変換する
	// 相対パスで指定してください (拡張子も含む)
	// -----------------------------------------
	bool SPIRVCompile(const String& _hlslName);

	// -----------------------------------------
	// SPIR-Vコンパイル外部ファイルを削除する
	// -----------------------------------------
	bool DeleteCompileShader(const String& _hlslName);

};

