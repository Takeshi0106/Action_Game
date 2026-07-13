// ヘッダー
#include "DirectX11_ViewManager.h"
#include "ReportMessage.h"

// レンダーターゲット
Handle DirectX11_ViewManager::RenderTargetViewCreateOnGet(
	ID3D11Device* _device,
	ID3D11Texture2D& _resource,
	const D3D11_RENDER_TARGET_VIEW_DESC& _desc,
	const Hashed_String& _name)
{
	// 既に作成済みかチェック
	if (m_RenderTargetViews.Exists(_name))
	{
		// 取得
		Handle handle = m_RenderTargetViews.GetHandle(_name);
#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		if (!m_RenderTargetViews.GetData(handle)->IsSame(_desc)) {
			ErrorLog::OutputToConsole(u8"同じ名前のレンダーターゲットビューが異なる設定で作成されようとしました: " + _name.GetString());
			return Handle();
		}
#endif
		DebugLog::OutputToConsole(u8"同じ名前のレンダーターゲットビューが作成されようとしました: " + _name.GetString());
		return handle;
	}

	// レンダーターゲットビュー作成
	DirectX11_RTVData rtvData;
	if (!rtvData.Create_DX11RTV(_device, _resource, _desc))
	{
		ErrorLog::OutputToConsole(u8"レンダーターゲットビューの作成に失敗しました: " + _name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_RenderTargetViews.AddData(_name, rtvData);
}

// シェーダーリソースビュー
Handle DirectX11_ViewManager::ShaderResourceViewCreateOnGet(
	ID3D11Device* _device,
	ID3D11Texture2D& _resource,
	const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc,
	const Hashed_String& _name)
{
	// 既に作成済みかチェック
	if (m_ShaderResourceViews.Exists(_name))
	{
		Handle handle = m_ShaderResourceViews.GetHandle(_name);
#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		if (!m_ShaderResourceViews.GetData(handle)->IsSame(_desc)) {
			ErrorLog::OutputToConsole(u8"同じ名前のシェーダーリソースビューが異なる設定で作成されようとしました: " + _name.GetString());
			return Handle();
		}
#endif
		DebugLog::OutputToConsole(u8"同じ名前のシェーダーリソースビューが作成されようとしました: " + _name.GetString());
		return handle;
	}

	// シェーダーリソースビュー作成
	DirectX11_SRVData srvData;
	if (!srvData.Create_DX11SRV(_device, _resource, _desc))
	{
		ErrorLog::OutputToConsole(u8"シェーダーリソースビューの作成に失敗しました: " + _name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_ShaderResourceViews.AddData(_name, srvData);
}

// 深度ステンシルビュー
Handle DirectX11_ViewManager::DepthStencilViewCreateOnGet(
	ID3D11Device* _device,
	ID3D11Texture2D& _resource,
	const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc,
	const Hashed_String& _name)
{
	// 作成済みかチェック
	if (m_DepthStencilViews.Exists(_name))
	{
		Handle handle = m_DepthStencilViews.GetHandle(_name);
#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		if (!m_DepthStencilViews.GetData(handle)->IsSame(_desc)) {
			ErrorLog::OutputToConsole(u8"同じ名前の深度ステンシルビューが異なる設定で作成されようとしました: " + _name.GetString());
			return Handle();
		}
#endif
		DebugLog::OutputToConsole(u8"同じ名前の深度ステンシルビューが作成されようとしました: " + _name.GetString());
		return handle;
	}

	// 深度ステンシルビュー作成
	DirectX11_DSVData dsvData;
	if (!dsvData.Create_DX11DSV(_device, _resource, _desc))
	{
		ErrorLog::OutputToConsole(u8"深度ステンシルビューの作成に失敗しました: " + _name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_DepthStencilViews.AddData(_name, dsvData);
}
