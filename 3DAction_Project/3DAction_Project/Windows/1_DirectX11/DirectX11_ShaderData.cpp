
// ==============================================
// ヘッダー
// ==============================================
// 必須ヘッダー
#include "DirectX11_ShaderData.h"
// レポートメッセージヘッダー
#include "../../ReportMessage.h"


// ==============================================
// 頂点シェーダーデータ作成
// ==============================================
bool DirectX11_VertexShaderData::Create_DX11VertexShader(
	ID3D11Device* _device,
	const BinaryView& _shaderData,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& _inputLayoutDescs,
	const std::vector<ShaderConstantInfo>& _constantInfo)
{
	// 頂点シェーダー作成
	HRESULT hr = _device->CreateVertexShader(
		_shaderData.GetData(),
		_shaderData.GetSize(),
		nullptr,
		m_VertexShader.GetAddressOf());
	
	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"頂点シェーダーの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// 入力レイアウト作成
	hr = _device->CreateInputLayout(
		_inputLayoutDescs.data(),
		(UINT)_inputLayoutDescs.size(),
		_shaderData.GetData(),
		_shaderData.GetSize(),
		m_InputLayout.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"入力レイアウトの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// 定数バッファ情報保存
	m_ShaderConstantInfo = _constantInfo;

	return true;
}


// ==============================================
// ピクセルシェーダーデータ作成
// ==============================================
bool DirectX11_PixelShaderData::Create_DX11PixelShader(
	ID3D11Device* _device,
	const BinaryView& _shaderData,
	const std::vector<ShaderConstantInfo>& _constantInfo)
{
	// ピクセルシェーダー作成
	HRESULT hr = _device->CreatePixelShader(
		_shaderData.GetData(),
		_shaderData.GetSize(),
		nullptr,
		m_PixelShader.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"ピクセルシェーダーの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// 定数バッファ情報保存
	m_ShaderConstantInfo = _constantInfo;

	return true;
}


// ==============================================
// コンピュートシェーダーデータ作成
// ==============================================
bool DirectX11_ComputeShaderData::Create_DX11ComputeShader(
	ID3D11Device* _device,
	const BinaryView& _shaderData,
	const std::vector<ShaderConstantInfo>& _constantInfo)
{
	// コンピュートシェーダー作成
	HRESULT hr = _device->CreateComputeShader(
		_shaderData.GetData(),
		_shaderData.GetSize(),
		nullptr,
		m_ComputeShader.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"コンピュートシェーダーの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// 定数バッファ情報保存
	m_ShaderConstantInfo = _constantInfo;

	return true;
}
