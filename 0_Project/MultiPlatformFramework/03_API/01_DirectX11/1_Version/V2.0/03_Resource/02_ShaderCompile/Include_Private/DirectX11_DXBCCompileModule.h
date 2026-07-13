#pragma once

// =============================
// クラス概要
// DirectX11用シェーダーコンパイルモジュール
// HLSLを入れているフォルダを指定して、シェーダーをコンパイルするクラス
// fxc を使用してコンパイルしています。
// =============================
// ヘッダー
#include "UTF8_String.h"
#include "BinaryData.h"
#include <cstdint>

// 列挙型定義
enum class DX11_CompileMode : uint8_t
{
	Debug,
	Release
};

// クラス
class DirectX11_DXBCCompileModule final
{
private:

public:
	// コンストラクタ・デストラクタ
	DirectX11_DXBCCompileModule() = default;
	~DirectX11_DXBCCompileModule() = default;

	// シェーダーコンパイル
	BinaryData ShaderCompil(
		const String& _hlslFilePath,
		const DX11_CompileMode _mode);
};
