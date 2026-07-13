
// ヘッダー
#include "DirectX11_DXBCCompileModule.h"
#include "ShaderUtility.h"
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <wrl/client.h> 
#include <filesystem>
#include "UTF8_String.h"
#include "ReportMessage.h"

// シェーダーをコンパイルして出力する関数
BinaryData CompileShader(
	const std::filesystem::path _hlslPath,
	const char* entryPoint,
	const char* shaderTypeModel,
	const DX11_CompileMode& mode);

// コンパイルチェック
BinaryData DirectX11_DXBCCompileModule::ShaderCompil(
	const String& _hlslFilePath,
	const DX11_CompileMode _mode)
{
	// .hlslのパスを取得
	std::filesystem::path hlslPath = std::filesystem::path(_hlslFilePath.GetU8String());
	hlslPath.make_preferred();

	// 各シェーダーをコンパイル
	BinaryData shaderData;
	SETSHADERTYPE type = ShaderUtility::GetShaderTypeFromFileName(hlslPath.filename().u8string());
	switch (type)
	{
	case SETSHADERTYPE::VERTEXSHADER:
		shaderData = CompileShader(hlslPath, "main", "vs_5_0", _mode);
		if (shaderData.IsEmpty()) {
			ErrorLog::OutputToConsole(u8"頂点シェーダー " + hlslPath.u8string() + u8" のコンパイル失敗");
			return BinaryData();
		}
		break;
	case SETSHADERTYPE::PIXSELSHADER:
		shaderData = CompileShader(hlslPath, "main", "ps_5_0", _mode);
		if (shaderData.IsEmpty()) {
			ErrorLog::OutputToConsole(u8"ピクセルシェーダー " + hlslPath.u8string() + u8" のコンパイル失敗");
			return BinaryData();
		}
		break;
	case SETSHADERTYPE::CONPUTESHADER:
		shaderData = CompileShader(hlslPath, "main", "cs_5_0", _mode);
		if (shaderData.IsEmpty()) {
			ErrorLog::OutputToConsole(u8"コンピュートシェーダー " + hlslPath.u8string() + u8" のコンパイル失敗");
			return BinaryData();
		}
		break;
	default:
		ErrorLog::OutputToConsole(hlslPath.u8string() + u8" : シェーダーの種類が判定できませんでした");
		return BinaryData();
		break;
	}

	return shaderData;
}

// シェーダーをコンパイルして出力する関数
BinaryData CompileShader(
	const std::filesystem::path _hlslPath,
	const char* _entryPoint,
	const char* _shaderTypeModel,
	const DX11_CompileMode& _mode)
{
	Microsoft::WRL::ComPtr<ID3DBlob> blob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

	// コンパイルフラグ
	DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
	if (_mode == DX11_CompileMode::Debug)
	{
		dwShaderFlags |= D3DCOMPILE_DEBUG;
	}
	else
	{
		dwShaderFlags |= D3DCOMPILE_OPTIMIZATION_LEVEL3;
	}

	// コンパイル
	HRESULT hr = D3DCompileFromFile(
		_hlslPath.wstring().c_str(),      
		nullptr,                          
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		_entryPoint,
		_shaderTypeModel,
		dwShaderFlags,
		0,
		blob.GetAddressOf(),
		errorBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(_hlslPath.u8string() + u8"のコンパイルに失敗" + (static_cast<const char8_t*>(errorBlob->GetBufferPointer())));
		return BinaryData();
	}

	// ファイルを書き出す
	BinaryData blobData;
	blobData.Write(
		blob->GetBufferPointer(),
		blob->GetBufferSize());

	// 解放
	errorBlob.Reset();
	return blobData;
}
