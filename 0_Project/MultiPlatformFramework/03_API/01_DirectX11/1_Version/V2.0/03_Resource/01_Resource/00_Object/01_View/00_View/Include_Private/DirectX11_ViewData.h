#pragma once

// ==============================================
// 【クラス概要】
// DirectX11 用ビュー　データ
// ==============================================
// d3d ヘッダー
#include <d3d11.h>
#include <wrl/client.h>

// ==============================================
// RTV データ クラス
// ==============================================
class DirectX11_RTVData final
{
private:
	// メンバー変数
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV = nullptr;
#if defined(DEBUG) || defined(_DEBUG)
	D3D11_RENDER_TARGET_VIEW_DESC m_Desc {};
#endif

public:
	// コンストラクタ・デストラクタ
	DirectX11_RTVData() = default;
	~DirectX11_RTVData() = default;

	// 作成
	bool Create_DX11RTV(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_RENDER_TARGET_VIEW_DESC& _desc);

	// ゲッター
	ID3D11RenderTargetView* GetRTV() const { return m_RTV.Get(); }

	// 削除
	void Reset() { 
		m_RTV.Reset(); 
#if defined(DEBUG) || defined(_DEBUG)
		m_Desc = {};
#endif
	}

#if defined(DEBUG) || defined(_DEBUG)
	// 同一確認
	bool IsSame(const D3D11_RENDER_TARGET_VIEW_DESC& _desc) const;
#endif
};


// ==============================================
// SRV データ クラス
// ==============================================
class DirectX11_SRVData final
{
private:
	// メンバー変数
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV = nullptr;
#if defined(DEBUG) || defined(_DEBUG)
	D3D11_SHADER_RESOURCE_VIEW_DESC m_Desc {};
#endif

public:
	// コンストラクタ・デストラクタ
	DirectX11_SRVData() = default;
	~DirectX11_SRVData() = default;

	// 作成
	bool Create_DX11SRV(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc);

	// ゲッター
	ID3D11ShaderResourceView* GetSRV() const { return m_SRV.Get(); }

	// 削除
	void Reset() { 
		m_SRV.Reset(); 
#if defined(DEBUG) || defined(_DEBUG)
		m_Desc = {};
#endif
	}

#if defined(DEBUG) || defined(_DEBUG)
	// 同一確認
	bool IsSame(const D3D11_SHADER_RESOURCE_VIEW_DESC& _desc) const;
#endif
};


// ==============================================
// DSV データ クラス
// ==============================================
class DirectX11_DSVData final
{
private:
	// メンバー変数
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV = nullptr;
#if defined(DEBUG) || defined(_DEBUG)
	D3D11_DEPTH_STENCIL_VIEW_DESC m_Desc {};
#endif

public:
	// コンストラクタ・デストラクタ
	DirectX11_DSVData() = default;
	~DirectX11_DSVData() = default;

	// 作成
	bool Create_DX11DSV(
		ID3D11Device* _device,
		ID3D11Texture2D& _resource,
		const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc);

	// ゲッター
	ID3D11DepthStencilView* GetDSV() const { return m_DSV.Get(); }
	
	// 削除
	void Reset() { 
		m_DSV.Reset();
#if defined(DEBUG) || defined(_DEBUG)
		m_Desc = {};
#endif
	}

#if defined(DEBUG) || defined(_DEBUG)
	// 同一確認
	bool IsSame(const D3D11_DEPTH_STENCIL_VIEW_DESC& _desc) const;
#endif
};
