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

// 名前などのデバッグ情報取得用
#include <string>        // 名前など


// ================================
// クラス
// ================================
// Viewの基底クラス
class BaseViewData {
protected:

#if defined(DEBUG) || defined(_DEBUG)
	std::string m_Name; // ViewNameを取得しておく

	// 使用できないようにしています
	BaseViewData(std::string name) : m_Name(name) {
	}
#else
	BaseViewData(std::string name) {}

#endif

	virtual ~BaseViewData() = default;
};


// SRVDataクラス
class SRVData : public BaseViewData
{
private:
	// SRV
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_SRV;

public:
	// コンストラクタ・デストラクタ
	SRVData(std::string name) : BaseViewData(name) {}
	~SRVData() = default;

	// ビュー作成
	bool CreateSRV(ID3D11Device* device, 
		ID3D11Resource* resource,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_SRV_DIMENSION viewDim = D3D11_SRV_DIMENSION_TEXTURE2D,
		UINT mostDetailedMip = 0,
		UINT mipLevels = -1);

	// ゲッター
	ID3D11ShaderResourceView* GetSRV() { return m_SRV.Get(); }
};


// UAVデータクラス
class UAVData : public BaseViewData
{
private:
	// UAV
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> m_UAV;

public:
	// コンストラクタ・デストラクタ
	UAVData(std::string name) : BaseViewData(name) {}
	~UAVData() = default;

	// ビュー作成
	bool CreateUAV(ID3D11Device* device,
		ID3D11Resource* resource,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_UAV_DIMENSION viewDim = D3D11_UAV_DIMENSION_TEXTURE2D,
		UINT mipSlice = 0);

	// ゲッター
	ID3D11UnorderedAccessView* GetUAV() { return m_UAV.Get(); }
};


// RTVクラス
class RTVData : public BaseViewData
{
private:
	// RTV
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_RTV;

public:
	// コンストラクタ・デストラクタ
	RTVData(std::string name) : BaseViewData(name) {}
	~RTVData() = default;

	// ビュー作成
	bool CreateRTV(ID3D11Device* device, 
		ID3D11Resource* resource,
		DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM,
		D3D11_RTV_DIMENSION viewDim = D3D11_RTV_DIMENSION_TEXTURE2D,
		UINT mipSlice = 0);

	// ゲッター
	ID3D11RenderTargetView* GetRTV() { return m_RTV.Get(); }
};


// DSVクラス
class DSVData : public BaseViewData
{
private:
	// DSV
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DSV;

public:
	// コンストラクタ・デストラクタ
	DSVData(std::string name) : BaseViewData(name) {}
	~DSVData() = default;

	// ビュー作成
	bool CreateDSV(ID3D11Device* device, 
		ID3D11Resource* resource,
		DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT,
		D3D11_DSV_DIMENSION viewDim = D3D11_DSV_DIMENSION_TEXTURE2D,
		UINT mipSlice = 0);

	// ゲッター
	ID3D11DepthStencilView* GetDSV() { return m_DSV.Get(); }
};