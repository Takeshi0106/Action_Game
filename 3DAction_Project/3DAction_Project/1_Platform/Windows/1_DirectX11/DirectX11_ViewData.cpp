
// ==========================================
// ヘッダー
// ==========================================
// 必須ヘッダー
#include "DirectX11_ViewData.h"
// レポートメッセージヘッダー
#include "../../../ReportMessage.h"


// ==========================================
// レンダーターゲットビュー作成
// ==========================================
bool DirectX11_RTVData::Create_DX11RTV(
	ID3D11Device* _device,
	ID3D11Texture2D& _resource,
	const D3D11_RENDER_TARGET_VIEW_DESC& desc)
{
	// レンダーターゲットビュー作成
	HRESULT hr = _device->CreateRenderTargetView(
		&_resource,
		&desc,
		m_RTV.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"レンダーターゲットビューの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用情報取得
	m_Desc = desc;
#endif

	return true;
}


// ==========================================
// シェーダーリソースビュー作成
// ==========================================
bool DirectX11_SRVData::Create_DX11SRV(
	ID3D11Device* _device,
	ID3D11Texture2D& _resource,
	const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc)
{
	// シェーダーリソースビュー作成
	HRESULT hr = _device->CreateShaderResourceView(
		&_resource,
		&_desc,
		m_SRV.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"シェーダーリソースビューの作成に失敗しました " +
			String::to_u8string(hr));
		return false;
	}

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用情報取得
	m_Desc = _desc;
#endif

	return true;
}


// ==========================================
// 深度ステンシルビュー作成
// ==========================================
bool DirectX11_DSVData::Create_DX11DSV(
	ID3D11Device* _device,
	ID3D11Texture2D& _resource,
	const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc)
{	
	// 深度ステンシルビュー作成
	HRESULT hr = _device->CreateDepthStencilView(
		&_resource,
		&_desc,
		m_DSV.GetAddressOf());
	
	// 作成失敗
	if (FAILED(hr)) {
		// エラーログ出力
		ErrorLog::OutputToConsole(u8"深度ステンシルビューの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用情報取得
	m_Desc = _desc;
#endif

	return true;
}


#if defined(DEBUG) || defined(_DEBUG)
// ==========================================
// RTV デバッグ 同一確認
// ==========================================
bool DirectX11_RTVData::IsSame(
	const D3D11_RENDER_TARGET_VIEW_DESC& _desc) const
{
	// フォーマットが異なる
	if (m_Desc.Format != _desc.Format) {
		return false;
	}
	// ビューディメンションが異なる
	if (m_Desc.ViewDimension != _desc.ViewDimension) {
		return false;
	}
	// いまのところTexture2Dにみなので
	if (m_Desc.Texture2D.MipSlice != _desc.Texture2D.MipSlice) {
		return false;
	}

	return true;
}


// ==========================================
// SRV デバッグ 同一確認
// ==========================================
bool DirectX11_SRVData::IsSame(
	const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc) const
{
	// フォーマットが異なる
	if (m_Desc.Format != _desc.Format) {
		return false;
	}
	// ビューディメンションが異なる
	if (m_Desc.ViewDimension != _desc.ViewDimension) {
		return false;
	}
	// いまのところTexture2Dにみなので
	if (m_Desc.Texture2D.MostDetailedMip != _desc.Texture2D.MostDetailedMip ||
		m_Desc.Texture2D.MipLevels != _desc.Texture2D.MipLevels) {
		return false;
	}

	return true;
}


// ==========================================
// DSV デバッグ 同一確認
// ==========================================
bool DirectX11_DSVData::IsSame(
	const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc) const
{
	// フォーマットが異なる
	if (m_Desc.Format != _desc.Format) {
		return false;
	}
	// ビューディメンションが異なる
	if (m_Desc.ViewDimension != _desc.ViewDimension) {
		return false;
	}
	// いまのところTexture2Dにみなので
	if (m_Desc.Texture2D.MipSlice != _desc.Texture2D.MipSlice) {
		return false;
	}
}

#endif
