#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用ビュー（レンダーターゲットビュー、シェーダーリソースビューなど）マネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ==============================================
// クラス
// ==============================================
class DirectX11_ViewManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// レンダーターゲットビュー管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11RenderTargetView>> m_RenderTargetViews;
	// シェーダーリソースビュー管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_ShaderResourceViews;
	// 深度ステンシルビュー管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11DepthStencilView>> m_DepthStencilViews;


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
		ID3D11Texture2D* _resource,
		UINT mmipSlice,
		const Hashed_String& _name);

	// シェーダーリソースビュー作成
	const Handle ShaderResourceViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D* _resource,
		UINT mostDetailedMip,
		UINT mipLevels,
		const Hashed_String& _name);

	// 深度ステンシルビュー作成
	const Handle DepthStencilViewCreateOnGet(
		ID3D11Device* _device,
		ID3D11Texture2D* _resource,
		UINT mipSlice,
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

