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
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../Hashed_String.h"


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
	const Handle RenderTargetViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_RENDER_TARGET_VIEW_DESC& _desc,
		const Hashed_String& _name);

	// シェーダーリソースビュー作成
	const Handle ShaderResourceViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc,
		const Hashed_String& _name);

	// 深度ステンシルビュー作成
	const Handle DepthStencilViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc,
		const Hashed_String& _name);


	// ------------------------------------------
	// ビュー取得関数
	// ------------------------------------------
	ID3D11RenderTargetView* GetRenderTargetView(const Handle& _handle);
	ID3D11ShaderResourceView* GetShaderResourceView(const Handle& _handle);
	ID3D11DepthStencilView* GetDepthStencilView(const Handle& _handle);


	// ------------------------------------------
	// ビュー削除関数
	// ------------------------------------------
	void ReleaseRTV(const Handle& _handle);
	void ReleaseSRV(const Handle& _handle);
	void ReleaseDSV(const Handle& _handle);


	// ------------------------------------------
	// 全てのビュー削除関数
	// ------------------------------------------
	void ReleaseAllView();
};

