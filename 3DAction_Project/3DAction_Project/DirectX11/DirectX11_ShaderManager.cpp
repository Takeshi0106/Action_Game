
// ===========================================
// ヘッダー
// ===========================================
// 必須ヘッダー
#include "DirectX11_ShaderManager.h"
// ログ出力用ヘッダー
#include "../ReportMessage.h"


// ===========================================
// 頂点シェーダー関数
// ===========================================
// 頂点シェーダー作成
const Handle DirectX11_ShaderManager::VertexShaderCreate(
	ID3D11Device* _device, const void* _bytecode, size_t _size, const char* _name)
{
	if (!_device || !_bytecode || _size == 0) {
		ErrorLog::OutputToConsole("無効な頂点シェーダーが作成されそうになりました");
		return Handle();
	}

	// 頂点シェーダー作成
	Microsoft::WRL::ComPtr<ID3D11VertexShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreateVertexShader(_bytecode, _size, nullptr, shader.GetAddressOf());
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
	ID3D11Device* _device, const void* _bytecode, size_t _size, const char* _name)
{
	if (!_device || !_bytecode || _size == 0) {
		ErrorLog::OutputToConsole("無効なピクセルシェーダーが作成されそうになりました");
		return Handle();
	}

	// ピクセルシェーダー作成
	Microsoft::WRL::ComPtr<ID3D11PixelShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreatePixelShader(_bytecode, _size, nullptr, shader.GetAddressOf());
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
	ID3D11Device* _device, const void* _bytecode, size_t _size, const char* _name)
{
	if (!_device || !_bytecode || _size == 0) {
		ErrorLog::OutputToConsole("無効なコンピュートシェーダーが作成されそうになりました");
		return Handle();
	}

	// コンピュートシェーダー作成
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> shader;

	// シェーダー作成
	HRESULT hr = _device->CreateComputeShader(_bytecode, _size, nullptr, shader.GetAddressOf());
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
