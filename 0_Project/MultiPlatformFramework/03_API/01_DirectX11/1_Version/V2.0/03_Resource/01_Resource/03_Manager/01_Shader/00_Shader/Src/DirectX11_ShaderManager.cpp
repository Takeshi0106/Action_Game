
// ヘッダー
#include "DirectX11_ShaderManager.h"
#include <filesystem>
# include <fstream>
#include <d3dcompiler.h>
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcommon.h>
#include "ReportMessage.h"

// プロトタイプ宣言
bool LoadShaderBinaryData(
	const String& _filePath,
	const String& _shaderName,
	const String& _fileExt,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob);

// 頂点シェーダー関数
Handle DirectX11_ShaderManager::VertexShaderCreateOnGet(
	ID3D11Device* _device,  
	const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Vertexs.Exists(_name))
	{
		DebugLog::OutputToConsole(u8"頂点シェーダーが既に作成されていました: " + _name.GetString());
		return m_Vertexs.GetHandle(_name);
	}

	// 変数作成
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
	std::vector<ShaderConstantInfo> constantInfos;

	// シェーダーバイナリーデータ読み込み
	if (!LoadShaderBinaryData(
		kCompileFilePath,
		_name.GetString(),
		kCompileFileExt,
		blob))
	{
		ErrorLog::OutputToConsole(u8"頂点シェーダーのバイナリーデータ読み込みに失敗: " + _name.GetString());
		return Handle();
	}

	// 自作バイナリーデータ作成
	BinaryView bv(
		blob.Get()->GetBufferPointer(),
		blob.Get()->GetBufferSize());
	
	// 頂点シェーダー
	DirectX11_VertexShaderData shader;
	if (shader.Create_DX11VertexShader(
		_device,
		bv,
		layoutDesc,
		constantInfos))
	{
		ErrorLog::OutputToConsole(u8"頂点シェーダーの作成に失敗しました: " + _name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Vertexs.AddData(_name, shader);
}

// ピクセルシェーダー作成関数
Handle DirectX11_ShaderManager::PixelShaderCreateOnGet(
	ID3D11Device* _device, 
	const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Pixels.Exists(_name))
	{
		DebugLog::OutputToConsole(u8"ピクセルシェーダーが既に作成されていました: " + _name.GetString());
		return m_Pixels.GetHandle(_name);
	}

	// 変数作成
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	std::vector<ShaderConstantInfo> constantInfos;

	// シェーダーバイナリーデータ読み込み
	if (!LoadShaderBinaryData(
		kCompileFilePath, 
		_name.GetString(),
		kCompileFileExt,
		blob))
	{
		ErrorLog::OutputToConsole(u8"ピクセルシェーダーのバイナリーデータ読み込みに失敗: " + _name.GetString());
		return Handle();
	}

	// 自作バイナリーデータ作成
	BinaryView bv(
		blob.Get()->GetBufferPointer(),
		blob.Get()->GetBufferSize());

	// ピクセルシェーダー
	DirectX11_PixelShaderData shader;
	if(shader.Create_DX11PixelShader(
		_device,
		bv,
		constantInfos))
	{
		ErrorLog::OutputToConsole(u8"ピクセルシェーダーの作成に失敗しました: " +_name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Pixels.AddData(_name, shader);
}

// コンピュートシェーダー作成関数
Handle DirectX11_ShaderManager::ComputeShaderCreateOnGet(
	ID3D11Device* _device,  const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Computes.Exists(_name))
	{
		DebugLog::OutputToConsole(u8"コンピュートシェーダーが既に作成されていました: " + _name.GetString());
		return m_Computes.GetHandle(_name);
	}

	// 変数作成
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	std::vector<ShaderConstantInfo> constantInfos;

	// シェーダーバイナリーデータ読み込み
	if (!LoadShaderBinaryData(
		kCompileFilePath, 
		_name.GetString(), 
		kCompileFileExt,
		blob))
	{
		ErrorLog::OutputToConsole(u8"コンピュートシェーダーのバイナリーデータ読み込みに失敗: " + _name.GetString());
		return Handle();
	}

	// 自作バイナリーデータ作成
	BinaryView bv(
		blob.Get()->GetBufferPointer(),
		blob.Get()->GetBufferSize());

	// コンピュートシェーダー
	DirectX11_ComputeShaderData shader;
	if (shader.Create_DX11ComputeShader(
		_device,
		bv,
		constantInfos))
	{
		ErrorLog::OutputToConsole(u8"コンピュートシェーダーの作成に失敗しました: " + _name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Computes.AddData(_name, shader);
}


// ===========================================
// シェーダーバイナリーデータ読み込み関数
// ===========================================
bool LoadShaderBinaryData(
	const String& _filePath,
	const String& _shaderName,
	const String& _fileExt,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob)
{
	// フルパス作成
	std::filesystem::path fullPath = _filePath.GetU8String();
	fullPath /= _shaderName.GetU8String();
	fullPath += _fileExt.GetU8String();
	fullPath.make_preferred();

	// バイナリで開く
	std::ifstream file(fullPath, std::ios::binary);
	if (!file.is_open()) {
		ErrorLog::OutputToConsole(u8"シェーダーバイナリーの読み込みに失敗: " + fullPath.u8string());
		return false;
	}

	// ファイルサイズ取得
	file.seekg(0, std::ios::end);
	size_t fileSize = static_cast<size_t>(file.tellg());
	file.seekg(0, std::ios::beg);

	// バッファ作成
	_outBlob.Reset();
	HRESULT hr = D3DCreateBlob(fileSize, _outBlob.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"Blob作成失敗: " + fullPath.u8string());
		return false;
	}

	// 読み込み
	file.read(
		static_cast<char*>(_outBlob->GetBufferPointer()),
		fileSize);
	return true;
}
