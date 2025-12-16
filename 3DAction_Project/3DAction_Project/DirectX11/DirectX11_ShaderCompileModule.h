#pragma once

// =============================
// クラス概要
// DirectX11用シェーダーコンパイルモジュール
// Debug : 同じ階層のシェーダーをコンパイルチェック
// Release : hlslフォルダからコンパイルチェック
// =============================


// =============================
// ヘッダー
// =============================


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
	const char* khlslPath;
	// シェーダーコンパイル設定
	const char* kCompilPath;
	// 拡張子
	const char* kHlslExtension = ".hlsl";
	const char* kCompilExtension = ".cso";


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DirectX11_ShaderCompileModule(
		const char* _hlslPath,
		const char* _compilPath) :
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
