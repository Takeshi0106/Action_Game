#pragma once

// =============================
// クラス概要
// DirectX11用シェーダーコンパイルモジュール
// Debug : 同じ階層のシェーダーをコンパイルチェック
// Release : hlslフォルダからコンパイルチェック
// fxc を使用してコンパイルしています。
// =============================


// =============================
// ヘッダー
// =============================
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ============================
// クラス
// ============================
class DirectX11_ShaderCompileModule final
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
	DirectX11_ShaderCompileModule(
		const String& _hlslPath,
		const String& _compilPath) :
		khlslPath(_hlslPath),
		kCompilPath(_compilPath)
	{
	}
	~DirectX11_ShaderCompileModule() = default;


	// --------------------------------
	// メンバー関数
	// --------------------------------
	void ShaderCompil();
};
