#pragma once

// ==============================================
// 【クラス概要】
// DirectX11 用サンプラーデータ
// ==============================================
// DirectXヘッダー
#include <d3d11.h>
#include <wrl/client.h>

// ==============================================
// クラス
// ==============================================
class DirectX11_SamplerData final
{
private:
	// メンバー変数
	Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
#if defined(DEBUG) || defined(_DEBUG)
	// サンプラー
	D3D11_SAMPLER_DESC m_Desc {};
#endif

public:
	// コンストラクタ・デストラクタ
	DirectX11_SamplerData() = default;
	~DirectX11_SamplerData() = default;

	// 作成
	bool Create_DX11SamplerState(
		ID3D11Device* _device,
		const D3D11_SAMPLER_DESC& _desc);

	// ゲッター
	const ID3D11SamplerState* GetSampler() const { return m_Sampler.Get(); }

	// 削除
	void Rest() { 
		m_Sampler.Reset(); 
#if defined(DEBUG) || defined(_DEBUG)
		m_Desc = {};
#endif
	}

#if defined(DEBUG) || defined(_DEBUG)
	// チェック
	bool IsSame(const D3D11_SAMPLER_DESC& _desc) const;
#endif
};

