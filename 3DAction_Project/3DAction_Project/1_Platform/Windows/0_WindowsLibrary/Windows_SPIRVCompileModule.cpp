
// ==============================================
// ヘッダー
// ==============================================
// 必須ヘッダー
#include "Windows_SPIRVCompileModule.h"
// ウィンドウズヘッダー
#include <Windows.h>
// コンパイルヘッダー
#include <dxcapi.h>
#pragma comment(lib, "dxcompiler.lib")
// スマートポインター
#include <wrl/client.h>
// ファイルシステムヘッダー
#include <filesystem>
// シェーダー便利関数ヘッダー
#include "../../../ShaderUtility.h"
// レポートログ出力
#include "../../../ReportMessage.h"


// ==============================================
// HLSL を SPIR-V に変換する
// ==============================================
bool Windows_SPIRV_CompileModule::SPIRVCompile(const String& _hlslPath)
{
	// DXCユーティリティ
	Microsoft::WRL::ComPtr<IDxcUtils> utils;
	// DXCコンパイラー
	Microsoft::WRL::ComPtr<IDxcCompiler3> compiler;
	// ソースコード
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;
	// コンパイル結果
	Microsoft::WRL::ComPtr<IDxcResult> result;

	// ファイルシステムパス取得
    std::filesystem::path shaderPath = _hlslPath.GetU8String();

    // DXCインスタンス作成
    HRESULT hr = DxcCreateInstance(
        CLSID_DxcUtils,
        IID_PPV_ARGS(&utils));
    // エラーチェック
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"DXCユーティリティの作成に失敗しました。");
        return false;
    }

	// コンパイラー作成
    hr = DxcCreateInstance(
        CLSID_DxcCompiler,
        IID_PPV_ARGS(&compiler));
	// エラーチェック
    if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"DXCコンパイラーの作成に失敗しました。");
        return false;
    }

    // ファイル読み込み
    hr = utils->LoadFile(
		shaderPath.wstring().c_str(),
        nullptr,
        &source);
	// エラーチェック
    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"HLSLファイルの読み込みに失敗しました。");
        return false;
    }

	// コンパイル用バッファ設定
    DxcBuffer buffer{};
    buffer.Ptr = source->GetBufferPointer();
    buffer.Size = source->GetBufferSize();
    buffer.Encoding = DXC_CP_UTF8;

    // シェーダータイプを取得
    SETSHADERTYPE type = ShaderUtility::GetShaderTypeFromFileName(shaderPath.filename().u8string());

	// シェーダータイプに応じてコンパイル引数を設定
	wchar_t profile[16] = {};
	
    switch (type)
    {
		// 頂点シェーダー
    case SETSHADERTYPE::VERTEXSHADER:
        wcscpy_s(profile, L"vs_6_0");
        break;
		// ピクセルシェーダー
    case SETSHADERTYPE::PIXSELSHADER:
        wcscpy_s(profile, L"ps_6_0");
        break;
		// コンピュートシェーダー
    case SETSHADERTYPE::CONPUTESHADER:
        wcscpy_s(profile, L"cs_6_0");
        break;
		// それ以外はエラー
    default:
        ErrorLog::OutputToConsole(u8"シェーダータイプの判定に失敗しました。ファイル名を確認してください。");
        return false;
    }

	// 出力ファイルパス
    std::filesystem::path outputPath =
        std::filesystem::path(kSPIRVFolderPath.GetU8String()) /
		(shaderPath.stem().u8string() + kSPIRVExtension.GetU8String());

    // 念のため変数に代入
    std::wstring outputPathW = outputPath.wstring();

    // コンパイル引数
    LPCWSTR args[] =
    {
        L"-T", 
		profile,    // シェーダーモデル
        L"-E",
        L"main",  // エントリポイント
        L"-spirv",// SPIR-V出力
        L"-Fo",
		outputPathW.c_str() // 出力ファイルパス
    };

	// コンパイル実行
    hr = compiler->Compile(
        &buffer,
        args,
        _countof(args),
        nullptr,
        IID_PPV_ARGS(&result));
	// エラーチェック
    if (FAILED(hr)) {
		ErrorLog::OutputToConsole(outputPath.u8string() +
            u8" HLSLのコンパイルに失敗しました。");
        return false;
    }

	// コンパイルステータス取得
    HRESULT status;
    result->GetStatus(&status);

    // エラーチェック
    if (FAILED(status))
    {
        // エラーメッセージ取得
        Microsoft::WRL::ComPtr<IDxcBlobUtf8> errors;
        result->GetOutput(
            DXC_OUT_ERRORS,
            IID_PPV_ARGS(&errors),
            nullptr);

        if (errors && errors->GetStringLength() > 0)
        {
            OutputDebugStringA(errors->GetStringPointer());
        }

        return false;
    }

    return true;
}
