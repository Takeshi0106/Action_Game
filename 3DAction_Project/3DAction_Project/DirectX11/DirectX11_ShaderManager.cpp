
// ===========================================
// ヘッダー
// ===========================================
// 必須ヘッダー
#include "DirectX11_ShaderManager.h"
// ファイルシステムヘッダー
#include <filesystem>
// ファイル入出力ヘッダー
# include <fstream>
// コンパイル用ヘッダー
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
// シェイダーコンパイル用ヘッダー
#include <d3dcommon.h>
// ログ出力用ヘッダー
#include "../ReportMessage.h"


// ===========================================
// プロトタイプ宣言
// ===========================================
// シェーダーバイナリーデータ読み込み関数
void LoadShaderBinaryData(
	const char* _filePath,
	const char* _shaderName,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob);


// ===========================================
// 頂点シェーダー関数
// ===========================================
// 頂点シェーダー作成
const Handle DirectX11_ShaderManager::VertexShaderCreate(
	ID3D11Device* _device,  const char* _name)
{
	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(kCompileFilePath, _name, blob);
	
	// 頂点シェーダー作成
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreateVertexShader(blob.Get()->GetBufferPointer(), 
		blob.Get()->GetBufferSize(), 
		nullptr, 
		shader.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Vertexs.AddData(_name, shader);
}

// 頂点シェーダー返す
ID3D11VertexShader* DirectX11_ShaderManager::GetVertexShader(const Handle& _handle)
{
	return m_Vertexs.GetData(_handle)->Get();
}


// ===========================================
// ピクセルシェーダー関数
// ===========================================
// ピクセルシェーダー作成
const Handle DirectX11_ShaderManager::PixelShaderCreate(
	ID3D11Device* _device, const char* _name)
{
	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(kCompileFilePath, _name, blob);

	// ピクセルシェーダー作成
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreatePixelShader(blob.Get()->GetBufferPointer(), 
		blob.Get()->GetBufferSize(), 
		nullptr, 
		shader.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Pixels.AddData(_name, shader);
}

// ピクセルシェーダー返す
ID3D11PixelShader* DirectX11_ShaderManager::GetPixelShader(const Handle& _handle)
{
	return m_Pixels.GetData(_handle)->Get();
}


// ===========================================
// コンピュートシェーダー関数
// ===========================================
// コンピュートシェーダー作成
const Handle DirectX11_ShaderManager::ComputeShaderCreate(
	ID3D11Device* _device,  const char* _name)
{
	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(kCompileFilePath, _name, blob);

	// コンピュートシェーダー作成
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreateComputeShader(blob.Get()->GetBufferPointer(), 
		blob.Get()->GetBufferSize(), 
		nullptr, 
		shader.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Computes.AddData(_name, shader);
}


// コンピュートシェーダー返す
ID3D11ComputeShader* DirectX11_ShaderManager::GetComputeShader(const Handle& _handle)
{
	return m_Computes.GetData(_handle)->Get();
}


// ===========================================
// シェーダーバイナリーデータ読み込み関数
// ===========================================
void LoadShaderBinaryData(
	const char* _filePath,
	const char* _shaderName,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob)
{
	// フルパス作成
	std::filesystem::path fullPath = _filePath;
	fullPath /= _shaderName;
	fullPath += ".cso";

	// 区切り文字を統一する
	fullPath.make_preferred();

	// バイナリで開く
	std::ifstream file(fullPath, std::ios::binary);
	if (!file.is_open()) {
		ErrorLog::OutputToConsole(
			(std::string("シェーダーバイナリーの読み込みに失敗: ") + 
				fullPath.string()).c_str());
		return;
	}

	// ファイルサイズ取得
	file.seekg(0, std::ios::end);
	size_t fileSize = static_cast<size_t>(file.tellg());
	file.seekg(0, std::ios::beg);

	// Blob作成
	_outBlob.Reset();

	HRESULT hr = D3DCreateBlob(fileSize, _outBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(
			(std::string("Blob作成失敗: ")
				+ fullPath.string()).c_str());
		return;
	}

	// 読み込み
	file.read(
		static_cast<char*>(_outBlob->GetBufferPointer()),
		fileSize);
}
