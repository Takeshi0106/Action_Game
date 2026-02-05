
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
#include "../../ReportMessage.h"


// ===========================================
// プロトタイプ宣言
// ===========================================
// シェーダーバイナリーデータ読み込み関数
void LoadShaderBinaryData(
	const String& _filePath,
	const String& _shaderName,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob);


// ===========================================
// 頂点シェーダー関数
// ===========================================
// 頂点シェーダー作成
const Handle DirectX11_ShaderManager::VertexShaderCreateOnGet(
	ID3D11Device* _device,  
	const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Vertexs.Exists(_name))
	{
		return m_Vertexs.GetHandle(_name);
		WarningLog::OutputToConsole(
			u8"頂点シェーダーが既に作成されていました: " + 
			_name.GetString());
	}

	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(kCompileFilePath, _name.GetString(), blob);
	
	// 頂点シェーダー作成
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreateVertexShader(
		blob.Get()->GetBufferPointer(), 
		blob.Get()->GetBufferSize(), 
		nullptr, 
		shader.GetAddressOf());

	// 失敗したら空ハンドルを返す
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Vertexs.AddData(_name, shader);
}

// 頂点シェーダーチェック
const bool DirectX11_ShaderManager::ExistsVertexShader(
	const Hashed_String& _name) const
{
	return m_Vertexs.Exists(_name);
}

// 頂点シェーダー返す
const ID3D11VertexShader* DirectX11_ShaderManager::GetVertexShader(
	const Handle& _handle)
{
	return m_Vertexs.GetData(_handle)->Get();
}

// 頂点シェーダー削除
void DirectX11_ShaderManager::ReleaseVertexShader(
	const Handle& _name)
{
	m_Vertexs.Remove(_name);
}


// ===========================================
// ピクセルシェーダー関数
// ===========================================
// ピクセルシェーダー作成
const Handle DirectX11_ShaderManager::PixelShaderCreateOnGet(
	ID3D11Device* _device, 
	const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Pixels.Exists(_name))
	{
		return m_Pixels.GetHandle(_name);
		WarningLog::OutputToConsole(
			u8"ピクセルシェーダーが既に作成されていました: " + 
			_name.GetString());
	}

	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(kCompileFilePath, _name.GetString(), blob);

	// ピクセルシェーダー作成
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreatePixelShader(
		blob.Get()->GetBufferPointer(), 
		blob.Get()->GetBufferSize(), 
		nullptr, 
		shader.GetAddressOf());

	// 失敗したら空ハンドルを返す
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Pixels.AddData(_name, shader);
}

// ピクセルシェーダーチェック
const bool DirectX11_ShaderManager::ExistsPixelShader(
	const Hashed_String& _name) const
{
	return m_Pixels.Exists(_name);
}

// ピクセルシェーダー返す
const ID3D11PixelShader* DirectX11_ShaderManager::GetPixelShader(
	const Handle& _handle)
{
	return m_Pixels.GetData(_handle)->Get();
}

// ピクセルシェーダー削除
void DirectX11_ShaderManager::ReleasePixelShader(
	const Handle& _name)
{
	m_Pixels.Remove(_name);
}

// ===========================================
// コンピュートシェーダー関数
// ===========================================
// コンピュートシェーダー作成
const Handle DirectX11_ShaderManager::ComputeShaderCreateOnGet(
	ID3D11Device* _device,  const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Computes.Exists(_name))
	{
		WarningLog::OutputToConsole(
			u8"コンピュートシェーダーが既に作成されていました: " +
			_name.GetString());
		return m_Computes.GetHandle(_name);
	}

	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(kCompileFilePath, _name.GetString(), blob);

	// コンピュートシェーダー作成
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreateComputeShader(blob.Get()->GetBufferPointer(), 
		blob.Get()->GetBufferSize(), 
		nullptr, 
		shader.GetAddressOf());

	// 失敗したら空ハンドルを返す
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Computes.AddData(_name, shader);
}

// コンピュートシェーダーチェック
const bool DirectX11_ShaderManager::ExistsComputeShader(
	const Hashed_String& _name) const
{
	return m_Computes.Exists(_name);
}

// コンピュートシェーダー返す
const ID3D11ComputeShader* DirectX11_ShaderManager::GetComputeShader(const Handle& _handle)
{
	return m_Computes.GetData(_handle)->Get();
}

// コンピュートシェーダー削除
void DirectX11_ShaderManager::ReleaseComputeShader(
	const Handle& _name)
{
	m_Computes.Remove(_name);
}


// ===========================================
// シェーダー全て削除
// ===========================================
void DirectX11_ShaderManager::ReleaseAllShader()
{
	m_Vertexs.ALLClear();
	m_Pixels.ALLClear();
	m_Computes.ALLClear();
}

// ===========================================
// シェーダーバイナリーデータ読み込み関数
// ===========================================
void LoadShaderBinaryData(
	const String& _filePath,
	const String& _shaderName,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob)
{
	// フルパス作成
	std::filesystem::path fullPath = _filePath.GetU8String();
	fullPath /= _shaderName.GetU8String();
	fullPath += ".cso";

	// 区切り文字を統一する
	fullPath.make_preferred();

	// バイナリで開く
	std::ifstream file(fullPath, std::ios::binary);
	if (!file.is_open()) {
		ErrorLog::OutputToConsole(
			u8"シェーダーバイナリーの読み込みに失敗: " + 
				fullPath.u8string());
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
			u8"Blob作成失敗: "+ fullPath.u8string());
		return;
	}

	// 読み込み
	file.read(
		static_cast<char*>(_outBlob->GetBufferPointer()),
		fileSize);
}
