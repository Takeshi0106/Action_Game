#pragma once

// ===============================
// 【クラス概要】
// リソースビュ―の作成・データ保持
// 
// ＊注意
// 重たいヘッダーがヘッダーにあるため
// ヘッダーにインクルード非推奨
// ===============================


// ================================
// ヘッダー
// ================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
#include <cstdint>      // 整数型 uintなど

// ================================
// クラス
// ================================
// SRVDataクラス
class SRVData
{
private:
	// SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

public:
	// コンストラクタ・デストラクタ
	SRVData() = default;
	~SRVData() = default;

	// ビュー作成
	bool CreateSRV(ID3D11Device* device, 
		ID3D11Texture2D* resource,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		UINT mostDetailedMip = 0,
		UINT mipLevels = -1);

	// ゲッター
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }
};


// UAVデータクラス
class UAVData
{
private:
	// UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;

public:
	// コンストラクタ・デストラクタ
	UAVData() = default;
	~UAVData() = default;

	// ビュー作成
	bool CreateUAV(ID3D11Device* device,
		ID3D11Texture2D* resource,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		UINT mipSlice = 0);

	// ゲッター
	ID3D11UnorderedAccessView* GetUAV() { return m_UAV.Get(); }
};


// RTVクラス
class RTVData
{
private:
	// RTV
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;
	uint32_t m_Width = 0;
	uint32_t m_Height = 0;

public:
	// コンストラクタ・デストラクタ
	RTVData() = default;
	~RTVData() = default;

	// ビュー作成
	bool CreateRTV(ID3D11Device* device,
		ID3D11Texture2D* resource,
		UINT mipSlice = 0);

	// ゲッター
	ID3D11RenderTargetView* GetRTV() { return m_RTV.Get(); }
	ID3D11RenderTargetView** GetRTVAddress() { return m_RTV.GetAddressOf(); }
	uint32_t GetWidth() { return m_Width; }
	uint32_t GetHeight() { return m_Height; }
};


// DSVクラス
class DSVData
{
private:
	// DSV
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;

public:
	// コンストラクタ・デストラクタ
	DSVData() = default;
	~DSVData() = default;

	// ビュー作成
	bool CreateDSV(ID3D11Device* device, 
		ID3D11Texture2D* resource,
		DXGI_FORMAT format);

	// ゲッター
	ID3D11DepthStencilView* GetDSV() { return m_DSV.Get(); }
};
