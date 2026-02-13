
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
// レポートログ出力
#include "../../ReportMessage.h"


// ==============================================
// HLSL を SPIR-V に変換する
// ==============================================
bool Windows_SPIRV_CompileModule::SPIRVCompile()
{
	Microsoft::WRL::ComPtr<IDxcUtils> utils;
	Microsoft::WRL::ComPtr<IDxcCompiler3> compiler;
	Microsoft::WRL::ComPtr<IDxcResult> result;

    // DXCインスタンス作成
    HRESULT hr = DxcCreateInstance(
        CLSID_DxcUtils,
        IID_PPV_ARGS(&utils));

    if (FAILED(hr)) return false;

    hr = DxcCreateInstance(
        CLSID_DxcCompiler,
        IID_PPV_ARGS(&compiler));

    if (FAILED(hr)) return false;

    // ファイル読み込み
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;
    hr = utils->LoadFile(
        L"shader.hlsl",
        nullptr,
        &source);

    if (FAILED(hr)) return false;

    DxcBuffer buffer{};
    buffer.Ptr = source->GetBufferPointer();
    buffer.Size = source->GetBufferSize();
    buffer.Encoding = DXC_CP_UTF8;

    // コンパイル引数
    LPCWSTR args[] =
    {
        L"-T", L"vs_6_0",   // Shader Model 6
        L"-E", L"VSMain",  // エントリポイント
        L"-spirv",         // SPIR-V出力
        L"-Fo", L"shader.spv"
    };

    hr = compiler->Compile(
        &buffer,
        args,
        _countof(args),
        nullptr,
        IID_PPV_ARGS(&result));

    if (FAILED(hr)) return false;

    HRESULT status;
    result->GetStatus(&status);

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
