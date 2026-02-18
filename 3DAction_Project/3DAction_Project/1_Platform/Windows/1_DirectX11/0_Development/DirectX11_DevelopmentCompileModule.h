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
// DXBCompile用ヘッダー
#include "DirectX11_ShaderCompileModule.h"



// ========================================
// クラス
// ========================================
class DX11_DevelopmentCompileModule final
{
private:
	// シェーダーコンパイルモジュール
	DirectX11_ShaderCompileModule m_ShaderCompileModule;

public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DX11_DevelopmentCompileModule() = default;
	~DX11_DevelopmentCompileModule() = default;
};

