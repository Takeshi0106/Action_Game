
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
#include "../../../ReportMessage.h"


// ===========================================
// プロトタイプ宣言
// ===========================================
// シェーダーバイナリーデータ読み込み関数
void LoadShaderBinaryData(
	const String& _filePath,
	const String& _shaderName,
	const String& _fileExt,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob);


// ===========================================
// 頂点シェーダー関数
// ===========================================
// 頂点シェーダー作成
Handle DirectX11_ShaderManager::VertexShaderCreateOnGet(
	ID3D11Device* _device,  
	const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Vertexs.Exists(_name))
	{
		// ログ出力
		WarningLog::OutputToConsole(
			u8"頂点シェーダーが既に作成されていました: " +
			_name.GetString());

		// ハンドル返す
		return m_Vertexs.GetHandle(_name);
	}

	// 変数作成
	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	// 入力レイアウト作成用データ入れる
	std::vector<D3D11_INPUT_ELEMENT_DESC> layoutDesc;
	// 定数バッファ情報入れる
	std::vector<ShaderConstantInfo> constantInfos;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(
		kCompileFilePath, 
		_name.GetString(),
		kCompileFileExt,
		blob);

	// 自作バイナリーデータ作成
	BinaryView bv(
		blob.Get()->GetBufferPointer(),
		blob.Get()->GetBufferSize());
	
	// 頂点シェーダー作成
	DirectX11_VertexShaderData shader;

	// シェーダー作成
	if(shader.Create_DX11VertexShader(
		_device,
		bv,
		layoutDesc,
		constantInfos))
	{
		// ログ出力
		ErrorLog::OutputToConsole(
			u8"頂点シェーダーの作成に失敗しました: " +
			_name.GetString());

		// 失敗したら空ハンドルを返す
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Vertexs.AddData(_name, shader);
}


// ===========================================
// ピクセルシェーダー作成関数
// ===========================================
Handle DirectX11_ShaderManager::PixelShaderCreateOnGet(
	ID3D11Device* _device, 
	const Hashed_String& _name)
{
	// 既に存在する場合はハンドルを返す
	if (m_Pixels.Exists(_name))
	{
		// ログ出力
		WarningLog::OutputToConsole(
			u8"ピクセルシェーダーが既に作成されていました: " +
			_name.GetString());

		// ハンドル返す
		return m_Pixels.GetHandle(_name);
	}

	// バイナリーデータ入れる
	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	// 定数バッファ情報
	std::vector<ShaderConstantInfo> constantInfos;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(
		kCompileFilePath, 
		_name.GetString(),
		kCompileFileExt,
		blob);

	// 自作バイナリーデータ作成
	BinaryView bv(
		blob.Get()->GetBufferPointer(),
		blob.Get()->GetBufferSize());

	// ピクセルシェーダー作成
	DirectX11_PixelShaderData shader;

	// シェーダー作成
	if(shader.Create_DX11PixelShader(
		_device,
		bv,
		constantInfos))
	{
		// ログ出力
		ErrorLog::OutputToConsole(
			u8"ピクセルシェーダーの作成に失敗しました: " +
			_name.GetString());

		// 失敗したら空ハンドルを返す
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Pixels.AddData(_name, shader);
}


// ===========================================
// コンピュートシェーダー作成関数
// ===========================================
Handle DirectX11_ShaderManager::ComputeShaderCreateOnGet(
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
	// 定数バッファ情報
	std::vector<ShaderConstantInfo> constantInfos;

	// シェーダーバイナリーデータ読み込み
	LoadShaderBinaryData(
		kCompileFilePath, 
		_name.GetString(), 
		kCompileFileExt,
		blob);

	// 自作バイナリーデータ作成
	BinaryView bv(
		blob.Get()->GetBufferPointer(),
		blob.Get()->GetBufferSize());

	// コンピュートシェーダー作成
	DirectX11_ComputeShaderData shader;

	// シェーダー作成
	if(shader.Create_DX11ComputeShader(
		_device,
		bv,
		constantInfos))
	{
		// ログ出力
		ErrorLog::OutputToConsole(
			u8"コンピュートシェーダーの作成に失敗しました: " +
			_name.GetString());

		// 失敗したら空ハンドルを返す
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_Computes.AddData(_name, shader);
}


// ===========================================
// シェーダーバイナリーデータ読み込み関数
// ===========================================
void LoadShaderBinaryData(
	const String& _filePath,
	const String& _shaderName,
	const String& _fileExt,
	Microsoft::WRL::ComPtr<ID3DBlob>& _outBlob)
{
	// フルパス作成
	std::filesystem::path fullPath = _filePath.GetU8String();
	fullPath /= _shaderName.GetU8String();
	fullPath += _fileExt.GetU8String();

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
