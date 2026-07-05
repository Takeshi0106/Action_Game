#pragma once

// =============================
// クラス概要
// DirectX11用シェーダーコンパイルモジュール
// HLSLを入れているフォルダを指定して、シェーダーをコンパイルするクラス
// fxc を使用してコンパイルしています。
// =============================


// =============================
// ヘッダー
// =============================
// 文字列ヘッダー
#include "../../../../UTF8_String.h"
// 固定整数ヘッダー
#include <cstdint>


// ============================
// 列挙型定義
// ============================
enum class DX11_CompileMode : uint8_t
{
	Debug,
	Release
};


// ============================
// クラス
// ============================
class DirectX11_DXBCCompileModule final
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// シェーダーフォルダパス
	const String& khlslPath;
	// シェーダーコンパイル設定
	const String& kCompilPath;
	// 拡張子
	const String kHlslExtension = u8".hlsl";
	const String kCompilExtension = u8".cso";


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DirectX11_DXBCCompileModule(
		const String& _hlslPath,
		const String& _compilPath) :
		khlslPath(_hlslPath),
		kCompilPath(_compilPath)
	{
	}
	~DirectX11_DXBCCompileModule() = default;


	// --------------------------------
	// メンバー関数
	// --------------------------------
	// シェーダーコンパイル
	bool ShaderCompil(
		const String& _hlslName,
		const DX11_CompileMode _mode);

	// 外部ファイルシェーダー削除
	bool DeleteCompileShader(const String& _hlslName);
};
