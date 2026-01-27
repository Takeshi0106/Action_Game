
// ===========================================================================
// ヘッダー
// ===========================================================================
// 必須ヘッダー
#include "DirectX11_ViewManager.h"
// ログ出力用ヘッダー
#include "../ReportMessage.h"


// ===========================================================================
// レンダーターゲット
// ===========================================================================
// 作成
const Handle DirectX11_ViewManager::RenderTargetViewCreate(
	ID3D11Device* _device,
	ID3D11Texture2D* _resource,
	UINT mmipSlice,
	const String& _name)
{
	if (!_device || !_resource) {
		ErrorLog::OutputToConsole("無効なレンダーターゲットビューが作成されそうになりました");
		return Handle();
	}

	// レンダーターゲットビュー作成
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> view;

	// リソースの情報を取得
	D3D11_TEXTURE2D_DESC resourceDesc{};
	_resource->GetDesc(&resourceDesc);

	// レンダーターゲットビュー作成情報設定
	D3D11_RENDER_TARGET_VIEW_DESC desc{};
	desc.Format = resourceDesc.Format;
	desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = mmipSlice;

	// レンダーターゲットビュー作成
	HRESULT hr = _device->CreateRenderTargetView(_resource, &desc, view.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_RenderTargetViews.AddData((Hashed_String)_name, view);
}

// 取得
ID3D11RenderTargetView* DirectX11_ViewManager::GetRenderTargetView(
	const Handle& _handle)
{
	return m_RenderTargetViews.GetData(_handle)->Get();
}


// ===========================================================================
// シェーダーリソースビュー
// ===========================================================================
// 作成
const Handle DirectX11_ViewManager::ShaderResourceViewCreate(
	ID3D11Device* _device,
	ID3D11Texture2D* _resource,
	UINT mostDetailedMip,
	UINT mipLevels,
	const String& _name)
{
	if (!_device || !_resource) {
		ErrorLog::OutputToConsole("無効なシェーダーリソースビューが作成されそうになりました");
		return Handle();
	}

	// シェーダーリソースビュー作成
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> view;

	// リソースの情報を取得
	D3D11_TEXTURE2D_DESC resourceDesc{};
	_resource->GetDesc(&resourceDesc);

	// シェーダーリソースビュー作成情報設定
	D3D11_SHADER_RESOURCE_VIEW_DESC desc{};
	desc.Format = resourceDesc.Format;
	desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MostDetailedMip = mostDetailedMip;
	desc.Texture2D.MipLevels = mipLevels;

	// シェーダーリソースビュー作成
	HRESULT hr = _device->CreateShaderResourceView(_resource, &desc, view.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_ShaderResourceViews.AddData((Hashed_String)_name, view);
}

// 取得
ID3D11ShaderResourceView* DirectX11_ViewManager::GetShaderResourceView(
	const Handle& _handle)
{
	return m_ShaderResourceViews.GetData(_handle)->Get();
}


// ===========================================================================
// 深度ステンシルビュー
// ===========================================================================
// 作成
const Handle DirectX11_ViewManager::DepthStencilViewCreate(
	ID3D11Device* _device,
	ID3D11Texture2D* _resource,
	UINT mipSlice,
	const String& _name)
{
	if (!_device || !_resource) {
		ErrorLog::OutputToConsole("無効な深度ステンシルビューが作成されそうになりました");
		return Handle();
	}

	// 深度ステンシルビュー作成
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> view;

	// リソースの情報を取得
	D3D11_TEXTURE2D_DESC resourceDesc{};
	_resource->GetDesc(&resourceDesc);

	// 深度ステンシルビュー作成情報設定
	D3D11_DEPTH_STENCIL_VIEW_DESC desc{};
	desc.Format = resourceDesc.Format;
	desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	desc.Texture2D.MipSlice = mipSlice;

	// 深度ステンシルビュー作成
	HRESULT hr = _device->CreateDepthStencilView(_resource, &desc, view.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_DepthStencilViews.AddData((Hashed_String)_name, view);
}

// 取得
ID3D11DepthStencilView* DirectX11_ViewManager::GetDepthStencilView(const Handle& _handle)
{
	return m_DepthStencilViews.GetData(_handle)->Get();
}

