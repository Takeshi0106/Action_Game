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


// =====================================
// クラス
// =====================================
class Texture2DData
{
private:
	// Texture2D
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture;

public:
	// コンストラクタ・デストラクタ
	Texture2DData() = default;
	~Texture2DData() = default;

	// テクスチャ作成関数
	bool CreateTexture2D(ID3D11Device* device,
		unsigned int width,
		unsigned int height,
		DXGI_FORMAT format,
		D3D11_BIND_FLAG bindFlags,
		D3D11_USAGE usage = D3D11_USAGE_DEFAULT,
		D3D11_CPU_ACCESS_FLAG flag = static_cast<D3D11_CPU_ACCESS_FLAG>(0));

	// ゲッター
	ID3D11Texture2D* GetTexture() { return m_Texture.Get(); }
};

