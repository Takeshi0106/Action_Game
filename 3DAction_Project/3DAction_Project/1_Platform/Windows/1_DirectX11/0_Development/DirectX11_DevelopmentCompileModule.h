#pragma once

// ========================================
// クラス概要
// DirectX11用開発コンパイルモジュール
//
// 開発中に、シェーダーのコンパイルやリファレンスの書き出しなどを行うクラス
// ========================================


// ========================================
// ヘッダー
// ========================================
// 文字列ヘッダー
#include "../../../../UTF8_String.h"
// DXBCompile用ヘッダー
#include "DirectX11_DXBCCompileModule.h"
// SPIR-Vコンパイル用ヘッダー
#include "../../../Windows/0_WindowsLibrary/Windows_SPIRVCompileModule.h"
// SPIR-V リファレンスヘッダー
#include "../../../../3_SPIRV/SPIRVReferenctModule.h"	


// ========================================
// クラス
// ========================================
class DX11_DevelopmentCompileModule final
{
private:
	// シェーダーフォルダパス
	const String& kHlslPath;
	// hlsl拡張子
	const String kHlslExtension = u8".hlsl";

	// DXBC コンパイルモジュール
	DirectX11_DXBCCompileModule m_ShaderCompileModule;
	// SPIR-V コンパイルモジュール
	Windows_SPIRV_CompileModule m_SPIRVCompileModule;
	// SPIR-V リファレンスモジュール
	SPIRVReferenctModule m_SPIRVReferenceModule;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DX11_DevelopmentCompileModule(
		const String& _hlslPath,
		const String& _compilPath,
		const String& _spirvPath,
		const String& _refPath) :
		kHlslPath(_hlslPath),
		m_ShaderCompileModule(_hlslPath, _compilPath),
		m_SPIRVCompileModule(_hlslPath, _spirvPath),
		m_SPIRVReferenceModule(_spirvPath, _refPath) {
	}
	~DX11_DevelopmentCompileModule() = default;

	// --------------------------------
	// メンバー関数
	// --------------------------------
	// シェーダーコンパイル
	bool ShaderCompile(const DX11_CompileMode _mode);
};

