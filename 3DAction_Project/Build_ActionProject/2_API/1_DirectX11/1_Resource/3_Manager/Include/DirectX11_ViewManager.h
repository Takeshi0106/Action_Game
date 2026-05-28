#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用ビュー（レンダーターゲットビュー、シェーダーリソースビューなど）マネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// Viewデータ
#include "DirectX11_ViewData.h"
// データ管理テンプレートヘッダー
#include "../../../TemplateManager.h"
// 文字列ヘッダー
#include "Hashed_String.h"


// ==============================================
// クラス
// ==============================================
class DirectX11_ViewManager final
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// レンダーターゲットビュー管理
	TemplateManager<DirectX11_RTVData> m_RenderTargetViews;
	// シェーダーリソースビュー管理
	TemplateManager<DirectX11_SRVData> m_ShaderResourceViews;
	// 深度ステンシルビュー管理
	TemplateManager<DirectX11_DSVData> m_DepthStencilViews;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_ViewManager() = default;
	~DirectX11_ViewManager() = default;

	// ------------------------------------------
	// ビュー作成関数
	// ------------------------------------------
	// レンダーターゲットビュー作成
	Handle RenderTargetViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_RENDER_TARGET_VIEW_DESC& _desc,
		const Hashed_String& _name);

	// シェーダーリソースビュー作成
	Handle ShaderResourceViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc,
		const Hashed_String& _name);

	// 深度ステンシルビュー作成
	Handle DepthStencilViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc,
		const Hashed_String& _name);

	// ------------------------------------------
	// ビュー取得関数
	// ------------------------------------------
	// RTV取得
	const DirectX11_RTVData* GetRenderTargetView(const Handle& _handle) {
		return m_RenderTargetViews.GetData(_handle);
	}
	// SRV取得
	const DirectX11_SRVData* GetShaderResourceView(const Handle& _handle) {
		return m_ShaderResourceViews.GetData(_handle);
	}
	// DSV取得
	const DirectX11_DSVData* GetDepthStencilView(const Handle& _handle) {
		return m_DepthStencilViews.GetData(_handle);
	}

	// ------------------------------------------
	// ビュー削除関数
	// ------------------------------------------
	// RTV削除
	void ReleaseRTV(const Handle& _handle) {
		m_RenderTargetViews.Remove(_handle);
	}
	// SRV削除
	void ReleaseSRV(const Handle& _handle) {
		m_ShaderResourceViews.Remove(_handle);
	}
	// DSV削除
	void ReleaseDSV(const Handle& _handle) {
		m_DepthStencilViews.Remove(_handle);
	}

	// ------------------------------------------
	// 全てのビュー削除関数
	// ------------------------------------------
	void ReleaseAllView() {
		// RTV全削除
		m_RenderTargetViews.ALLClear();
		// SRV全削除
		m_ShaderResourceViews.ALLClear();
		// DSV全削除
		m_DepthStencilViews.ALLClear();
	}

};

