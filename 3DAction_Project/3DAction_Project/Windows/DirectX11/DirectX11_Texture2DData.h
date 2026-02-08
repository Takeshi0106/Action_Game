#pragma once

// ===================================
// 【クラス概要】
// DirectX11 のテクスチャクラス
// ===================================


// ===================================
// ヘッダー
// ===================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>


// ===================================
// クラス
// ===================================
class DirectX11_Texture2DData final
{
private:
	// -----------------------------------
	// メンバー変数
	// -----------------------------------
	// テクスチャ本体
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_Texture2D = nullptr;


#if defined(DEBUG) || defined(_DEBUG)
	// -----------------------------------
    // デバッグ用
    // -----------------------------------
	D3D11_TEXTURE2D_DESC m_Desc {};
#endif


public:
	// -----------------------------------
	// コンストラクタ・デストラクタ
	// -----------------------------------
	DirectX11_Texture2DData() = default;
	~DirectX11_Texture2DData() = default;


	// -----------------------------------
	// メンバー関数
	// -----------------------------------
	// 作成
	bool Create_DX11Texture2D(
		ID3D11Device* _device,
		const D3D11_TEXTURE2D_DESC& _desc,
		const D3D11_SUBRESOURCE_DATA* _initialData = nullptr);

	// ゲッター
	ID3D11Texture2D* GetTexture() const { return m_Texture2D.Get(); }

	// 削除
	void Reset() { 
		m_Texture2D.Reset();
#if defined(DEBUG) || defined(_DEBUG)
		m_Desc = {};
#endif
	}

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用: テクスチャ記述が同じか確認
	bool IsSame(const D3D11_TEXTURE2D_DESC& desc) const;
#endif

};
