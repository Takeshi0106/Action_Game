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
#include "../TemplateManager.h"
// 文字列ヘッダー
#include "../UTF8_String.h"


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
	const Handle RenderTargetViewCreate(
		ID3D11Device* _device,
		ID3D11Texture2D* _resource,
		UINT mmipSlice,
		const String& _name);

	// シェーダーリソースビュー作成
	const Handle ShaderResourceViewCreate(
		ID3D11Device* _device,
		ID3D11Texture2D* _resource,
		UINT mostDetailedMip,
		UINT mipLevels,
		const String& _name);

	// 深度ステンシルビュー作成
	const Handle DepthStencilViewCreate(
		ID3D11Device* _device,
		ID3D11Texture2D* _resource,
		UINT mipSlice,
		const String& _name);


	// ------------------------------------------
	// ビュー取得関数
	// ------------------------------------------
	ID3D11RenderTargetView* GetRenderTargetView(const Handle& _handle);
	ID3D11ShaderResourceView* GetShaderResourceView(const Handle& _handle);
	ID3D11DepthStencilView* GetDepthStencilView(const Handle& _handle);


	// ------------------------------------------
	// ビューチェック関数
	// ------------------------------------------
	bool ExistsRTV(const String& _name) const {
		return m_RenderTargetViews.Exists((Hashed_String)_name);
	}
	bool ExistsSRV(const String& _name) const {
		return m_ShaderResourceViews.Exists((Hashed_String)_name);
	}
	bool ExistsDSV(const String& _name) const {
		return m_DepthStencilViews.Exists((Hashed_String)_name);
	}


	// ------------------------------------------
	// ビューハンドル取得関数
	// ------------------------------------------
	const Handle GetRTVHandle(const String& _name) const {
		return m_RenderTargetViews.GetHandle((Hashed_String)_name);
	}
	const Handle GetSRVHandle(const String& _name) const {
		return m_ShaderResourceViews.GetHandle((Hashed_String)_name);
	}
	const Handle GetDSVHandle(const String& _name) const {
		return m_DepthStencilViews.GetHandle((Hashed_String)_name);
	}
};

