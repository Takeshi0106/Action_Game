#pragma once

// =========================================
// 【クラス概要】
// Windows用 
// hlsl を SPIR-V にコンパイルを行うモジュール
// =========================================

class Windows_SPIRV_CompileModule final
{
private:


public:
	// ---------------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------------
	Windows_SPIRV_CompileModule() = default;
	~Windows_SPIRV_CompileModule() = default;


	// -----------------------------------------
	// HLSL を SPIR-V に変換する
	// -----------------------------------------
	bool SPIRVCompile();
};

