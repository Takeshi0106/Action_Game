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


// ========================================
// クラス
// ========================================
class DX11_DevelopmentCompileModule final
{
private:
	// シェーダーコンパイルモジュール
	DirectX11_DXBCCompileModule m_ShaderCompileModule;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DX11_DevelopmentCompileModule(
		const String& _hlslPath,
		const String& _compilPath) :
		m_ShaderCompileModule(_hlslPath, _compilPath) {
	}
	~DX11_DevelopmentCompileModule() = default;

	// --------------------------------
	// メンバー関数
	// --------------------------------
	// シェーダーコンパイル
	void ShaderCompile(const String& _hlslFolderPath, const DX11_CompileMode _mode);
};

