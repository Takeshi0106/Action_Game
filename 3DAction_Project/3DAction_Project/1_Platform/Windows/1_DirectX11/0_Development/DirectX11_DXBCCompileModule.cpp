
// =====================================
// ヘッダー
// =====================================
// 必須ヘッダー
#include "DirectX11_DXBCCompileModule.h"
// シェーダー補助関数
#include "../../../../ShaderUtility.h"
// シェイダーコンパイル用ヘッダー
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
// DirectX用スマートポインター
#include <wrl/client.h> 
// ファイル出力、読込み用ヘッダー
#include <fstream> 
// ファイルシステムヘッダー
#include <filesystem>
// 文字列
#include "../../../../UTF8_String.h"
// バイナリーView
#include "../../../../BinaryView.h"
// 外部ファイルに書き出し読込関数
#include "../../../../FileUtils.h"
// デバッグ情報ややエラー出力用
#include "../../../../ReportMessage.h"


// =====================================
// グローバル関数
// =====================================
// コンパイルするかチェッククラス
bool IsCompileCheck(
	const std::filesystem::path& _hlslPath, 
	const std::filesystem::path& _compilePath);

// シェーダーをコンパイルして出力する関数
bool OutputCompileShader(
	const std::filesystem::path _hlslPath,
	const std::filesystem::path _compilePath,
	const char* entryPoint,
	const char* shaderTypeModel,
	const DX11_CompileMode& mode);


// =====================================
// コンパイルチェック
// =====================================
bool DirectX11_DXBCCompileModule::ShaderCompil(
	const String& _hlslName,
	const DX11_CompileMode _mode)
{
	// ファイルシステムのパスを取得
	std::filesystem::path hlslPath =
		std::filesystem::path(khlslPath.GetU8String()) /
		(_hlslName.GetU8String() + kHlslExtension.GetU8String());

	// コンパイルパスを作成
	std::filesystem::path compilePath =
		std::filesystem::path(kCompilPath.GetU8String()) /
		(hlslPath.filename().stem().u8string() +
			kCompilExtension.GetU8String());

	// 区切り文字を統一
	hlslPath = hlslPath.generic_string();
	compilePath = compilePath.generic_string();

	// シェーダーのコンパイルする必要があるかのチェック
	if (!IsCompileCheck(hlslPath, compilePath)) {
		WarningLog::OutputToConsole(u8"シェーダー " + hlslPath.u8string() + u8" はコンパイルの必要がありませんでした");
		return true;
	}

	// シェーダーの種類を判定
	SETSHADERTYPE type = ShaderUtility::GetShaderTypeFromFileName(hlslPath.filename().u8string());

	switch (type)
	{
	case SETSHADERTYPE::VERTEXSHADER:
		// 頂点シェーダーとしてコンパイル
		if (!OutputCompileShader(hlslPath, compilePath, "main", "vs_5_0", _mode)) {
			ErrorLog::OutputToConsole(u8"頂点シェーダー " + hlslPath.u8string() + u8" のコンパイル失敗");
			return false;
		}
		break;

	case SETSHADERTYPE::PIXSELSHADER:
		// ピクセルシェーダーとしてコンパイル
		if (!OutputCompileShader(hlslPath, compilePath, "main", "ps_5_0", _mode)) {
			ErrorLog::OutputToConsole(u8"ピクセルシェーダー " + hlslPath.u8string() + u8" のコンパイル失敗");
			return false;
		}
		break;

	case SETSHADERTYPE::CONPUTESHADER:
		// コンピュートシェーダーとしてコンパイル
		if (!OutputCompileShader(hlslPath, compilePath, "main", "cs_5_0", _mode)) {
			ErrorLog::OutputToConsole(u8"コンピュートシェーダー " + hlslPath.u8string() + u8" のコンパイル失敗");
			return false;
		}
		break;

	default:
		ErrorLog::OutputToConsole(hlslPath.u8string() + u8" : シェーダーの種類が判定できませんでした");
		return false;
		break;
	}

	return true;
}


// =====================================
// 外部ファイルシェーダー削除
// =====================================
bool DirectX11_DXBCCompileModule::DeleteCompileShader(const String& _hlslName)
{
	// コンパイルパスを取得
	std::filesystem::path compilePath =
		std::filesystem::path(kCompilPath.GetU8String()) /
		(_hlslName.GetU8String() + kCompilExtension.GetU8String());

	// 区切り文字を統一
	compilePath = compilePath.generic_string();

	// ファイルが存在する場合は削除
	if (std::filesystem::exists(compilePath)) 
	{
		// エラーコードを取得
		std::error_code ec;

		if (!std::filesystem::remove(compilePath, ec))
		{
			ErrorLog::OutputToConsole(u8"SPIR-Vファイルの削除に失敗しました: " + compilePath.u8string() + u8" エラー: " +
				String::to_u8string(ec.value()));
			return false;
		}
	}

	return true;
}


// =====================================
// コンパイルするかのチェック関数
// =====================================
bool IsCompileCheck(
	const std::filesystem::path& _hlslPath,
	const std::filesystem::path& _compilePath)
{
	// シェーダーファイルの最終更新日時を取得
	auto shaderLastWriteTime = std::filesystem::last_write_time(_hlslPath);

	// バイナリファイルが存在しない場合は更新が必要
	if (!std::filesystem::exists(_compilePath)) {
		return true;
	}

	// バイナリファイルの最終更新日時を取得
	auto binaryLastWriteTime = std::filesystem::last_write_time(_hlslPath);

	// シェーダーファイルの方が新しい場合は更新が必要
	return shaderLastWriteTime > binaryLastWriteTime;
}


// =====================================
// シェーダーをコンパイルして出力する関数
// =====================================
bool OutputCompileShader(
	const std::filesystem::path _hlslPath,
	const std::filesystem::path _compilepath,
	const char* _entryPoint,
	const char* _shaderTypeModel,
	const DX11_CompileMode& _mode)
{
	// コンパイルしたシェーダーを取得する
	Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
	// エラーを取得する
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	// コンパイルフラグ
	// コンパイル時に厳しくチェックするフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;

	// コンパイルモードによってフラグを切り替える
	if (_mode == DX11_CompileMode::Debug)
	{
		// デバッグ情報を付けるフラグ
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	}
	else
	{
		// 最適化フラグ
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
	}

	// コンパイル
	HRESULT hr = D3DCompileFromFile(
		_hlslPath.wstring().c_str(),            // シェーダーのパス
		nullptr,                                // GPUで使用するマクロ定義（ない場合nullptr）
		D3D_COMPILE_STANDARD_FILE_INCLUDE,      // HLSLで他のHLSLを読み込むフラグ
		_entryPoint,                     // シェーダーないで最初に実行される関数の名前
		_shaderTypeModel,                // シェーダーの種類とバージョン
		dwShaderFlags,                          // コンパイルのフラグ
		0,                                      // 今は何もないフラグ
		blob.GetAddressOf(),                    // コンパイルしたシェーダーを取得する
		errorBlob.GetAddressOf()                // エラーメッセージを取得する
	);
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(_hlslPath.u8string() + u8"のコンパイルに失敗" + (static_cast<const char8_t*>(errorBlob->GetBufferPointer())));
		return false;
	}

	// 書き出す内容を作成
	BinaryView blobData(
		blob->GetBufferPointer(),
		blob->GetBufferSize()
	);

	// ファイルに書き出す
	FileUtis::Binary::WriteBinaryFile(_compilepath.u8string(), blobData);

	// 解放
	errorBlob.Reset();

	return true;
}
