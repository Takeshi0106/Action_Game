#pragma once

// =====================================
// 【クラス概要】
// テクスチャデータ
// テクスチャの作成、データ保持するクラス
// 
// ＊注意
// 重たいヘッダーが含まれているため、
// ヘッダーにインクルード非推奨
// =====================================


// =====================================
// ヘッダー
// =====================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ


// ======================================
// 前方宣言
// ======================================
class TextureManager;



// =====================================
// クラス
// =====================================
// テクスチャ2Dデータクラス
class Texture2DData
{
protected:
	// Texture2D
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;

public:
	// コンストラクタ・デストラクタ
	Texture2DData() = default;
	~Texture2DData() = default;

	// ゲッター
	ID3D11Texture2D* GetTexture() { return m_Texture.Get(); }
};

// スワップチェインから作成するテクスチャデータクラス
class SwapchainTextureData : public Texture2DData
{
public:
	// コンストラクタ・デストラクタ
	SwapchainTextureData() = default;
	~SwapchainTextureData() = default;

	// スワップチェインからテクスチャ作成関数
	bool CreateTextureFromSwapChain(IDXGISwapChain* swapChain);
};

// バインド可能なテクスチャデータクラス
class BindableTextureData : public Texture2DData
{
public:
	// コンストラクタ・デストラクタ
	BindableTextureData() = default;
	~BindableTextureData() = default;

	// テクスチャ作成関数
	bool CreateTexture2D(ID3D11Device* device,
		unsigned int width,
		unsigned int height,
		DXGI_FORMAT format,
		D3D11_BIND_FLAG bindFlags,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		D3D11_CPU_ACCESS_FLAG flag = static_cast<D3D11_CPU_ACCESS_FLAG>(0),
		D3D11_SUBRESOURCE_DATA* initData = nullptr);
};
