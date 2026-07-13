#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用ビュー（レンダーターゲットビュー、シェーダーリソースビューなど）マネージャー
// APIObject を管理するマネージャー
// ==============================================
#include "DirectX11_ViewData.h"
#include "TemplateManager.h"
#include "Hashed_String.h"

// ==============================================
// クラス
// ==============================================
class DirectX11_ViewManager final
{
private:
	// メンバー変数
	TemplateManager<DirectX11_RTVData> m_RenderTargetViews;
	TemplateManager<DirectX11_SRVData> m_ShaderResourceViews;
	TemplateManager<DirectX11_DSVData> m_DepthStencilViews;

public:
	// コンストラクタ・デストラクタ
	DirectX11_ViewManager() = default;
	~DirectX11_ViewManager() = default;

	// 作成関数
	Handle RenderTargetViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_RENDER_TARGET_VIEW_DESC& _desc,
		const Hashed_String& _name);
	Handle ShaderResourceViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc,
		const Hashed_String& _name);
	Handle DepthStencilViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc,
		const Hashed_String& _name);

	// 取得関数
	const DirectX11_RTVData* GetRenderTargetView(const Handle& _handle) {
		return m_RenderTargetViews.GetData(_handle);
	}
	const DirectX11_SRVData* GetShaderResourceView(const Handle& _handle) {
		return m_ShaderResourceViews.GetData(_handle);
	}
	const DirectX11_DSVData* GetDepthStencilView(const Handle& _handle) {
		return m_DepthStencilViews.GetData(_handle);
	}

	// 削除関数
	void ReleaseRTV(const Handle& _handle) {
		m_RenderTargetViews.Remove(_handle);
	}
	void ReleaseSRV(const Handle& _handle) {
		m_ShaderResourceViews.Remove(_handle);
	}
	void ReleaseDSV(const Handle& _handle) {
		m_DepthStencilViews.Remove(_handle);
	}

	// 全て削除
	void ReleaseAllView() {
		m_RenderTargetViews.ALLClear();
		m_ShaderResourceViews.ALLClear();
		m_DepthStencilViews.ALLClear();
	}
};

