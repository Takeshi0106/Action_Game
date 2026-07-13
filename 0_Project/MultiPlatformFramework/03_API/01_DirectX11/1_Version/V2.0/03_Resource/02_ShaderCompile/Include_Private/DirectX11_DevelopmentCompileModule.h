#pragma once

// ========================================
// クラス概要
// DirectX11用開発コンパイルモジュール
//
// 開発中に、シェーダーのコンパイルやリファレンスの書き出しなどを行うクラス
// ========================================
#include "UTF8_String.h"
#include "DirectX11_DXBCCompileModule.h"
#include "Windows_SPIRVCompileModule.h"
#include "SPIRVReferenceModule.h"	

// ========================================
// クラス
// ========================================
class DX11_DevelopmentCompileModule final
{
private:
	// メンバー変数
	const String& kHlslPath;
	const String kHlslExtension = u8".hlsl";

	DirectX11_DXBCCompileModule m_ShaderCompileModule;
	Windows_SPIRV_CompileModule m_SPIRVCompileModule;
	SPIRVReferenceModule m_SPIRVReferenceModule;

	// 関数
	bool ShaderCompile(const String& _hlslName, const DX11_CompileMode _mode);
	bool DeleteCompileShader(const String& _hlslName);

public:
	// コンストラクタ・デストラクタ
	DX11_DevelopmentCompileModule(
		const String& _hlslPath,
		const String& _compilPath,
		const String& _spirvPath,
		const String& _refPath,
		const String& _dxcPath) :
		kHlslPath(_hlslPath),
		m_ShaderCompileModule(_hlslPath, _compilPath),
		m_SPIRVCompileModule(_hlslPath, _spirvPath, _dxcPath),
		m_SPIRVReferenceModule(_spirvPath, _refPath) {
	}
	~DX11_DevelopmentCompileModule() = default;

	// シェーダーコンパイル
	bool ShaderCompile(const DX11_CompileMode _mode);
};

