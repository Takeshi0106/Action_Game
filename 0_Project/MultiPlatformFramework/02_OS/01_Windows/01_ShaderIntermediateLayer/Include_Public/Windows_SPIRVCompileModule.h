#pragma once

// =========================================
// 【クラス概要】
// Windows用 
// hlsl を SPIR-V にコンパイルを行うモジュール
// Windows のみで使用することを想定しています
// =========================================
// ヘッダー
#include "UTF8_String.h"
#include "BinaryData.h"
#include <wrl/client.h>
#include <dxcapi.h>

// =========================================
// クラス
// =========================================
class Windows_SPIRV_CompileModule final
{
private:
	// 変数
	Microsoft::WRL::ComPtr<IDxcUtils> m_Utils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> m_Compiler;

public:
	// コンストラクタ・デストラクタ
	Windows_SPIRV_CompileModule() = default;
	~Windows_SPIRV_CompileModule() = default;

	// 初期化
	bool Init(String& _assetPath);
	// HLSL を SPIR-V に変換する
	BinaryData SPIRVCompile(const String& _hlslName, bool IsDebug);
};

