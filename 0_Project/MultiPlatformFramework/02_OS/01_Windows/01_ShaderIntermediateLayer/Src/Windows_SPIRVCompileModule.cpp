// ヘッダー
#include "Windows_SPIRVCompileModule.h"
#include <filesystem>
#include <Windows.h>
#include "ShaderUtility.h"
#include "ReportMessage.h"

// 初期化
bool Windows_SPIRV_CompileModule::Init(String& _assetPath)
{
	// DLLを追加
    std::filesystem::path dllPath = _assetPath.GetU8String();
    SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
    AddDllDirectory(dllPath.c_str());

    // DXC ユーティリティの作成
    if (FAILED(DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&m_Utils))) ||
        FAILED(DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&m_Compiler))))
    {
        ErrorLog::OutputToConsole(u8"DXCの初期化に失敗しました。");
        return false;
    }

    return true;
}

// HLSL を SPIR-V に変換する
BinaryData Windows_SPIRV_CompileModule::SPIRVCompile(const String& _hlslName, bool _isDebug)
{
	// .hlslパス取得
	std::filesystem::path shaderPath = _hlslName.GetU8String();
    shaderPath.make_preferred();

    // シェーダータイプを取得
    SETSHADERTYPE type = ShaderUtility::GetShaderTypeFromFileName(shaderPath.filename().u8string());
    std::wstring profile;
    switch (type)
    {
    case SETSHADERTYPE::VERTEXSHADER:
		profile = L"vs_6_0";
        break;
	case SETSHADERTYPE::PIXSELSHADER:
		profile = L"ps_6_0";
        break;
	case SETSHADERTYPE::CONPUTESHADER:
		profile = L"cs_6_0";
        break;
	default:
        ErrorLog::OutputToConsole(u8"シェーダータイプの判定に失敗しました。ファイル名を確認してください。");
        return BinaryData();
    }

    // HLSL読み込み
    Microsoft::WRL::ComPtr<IDxcBlobEncoding> source;
    if (FAILED(m_Utils->LoadFile(shaderPath.c_str(), nullptr, &source)))
    {
        ErrorLog::OutputToConsole(u8"HLSLの読み込みに失敗しました。");
        return {};
    }

    // コンパイルオプション
    std::vector<LPCWSTR> args =
    {
        L"-E", L"main",
        L"-T", profile.c_str(),
        L"-spirv",
        L"-fvk-use-dx-layout",
        L"-fspv-target-env=vulkan1.3"
    };
    if (_isDebug)
    {
        args.push_back(L"-Zi");
        args.push_back(L"-Qembed_debug");
        args.push_back(L"-Od");
    }
    else
    {
        args.push_back(L"-O3");
        args.push_back(L"-Qstrip_debug");
    }

    // コンパイル
    DxcBuffer buffer{};
    buffer.Ptr = source->GetBufferPointer();
    buffer.Size = source->GetBufferSize();
    buffer.Encoding = DXC_CP_UTF8;

    Microsoft::WRL::ComPtr<IDxcResult> result;
    if (FAILED(m_Compiler->Compile(
        &buffer,
        args.data(),
        static_cast<UINT32>(args.size()),
        nullptr,
        IID_PPV_ARGS(&result))))
    {
        ErrorLog::OutputToConsole(u8"SPIR-Vのコンパイルに失敗しました。");
        return {};
    }

    // SPIR-V取得
    Microsoft::WRL::ComPtr<IDxcBlob> blob;
    if (FAILED(result->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&blob), nullptr)))
    {
        ErrorLog::OutputToConsole(u8"SPIR-Vの取得に失敗しました。");
        return {};
    }

    // BinaryDataへコピー
    BinaryData binary;
	binary.Write(blob->GetBufferPointer(), blob->GetBufferSize());
    return binary;
}
