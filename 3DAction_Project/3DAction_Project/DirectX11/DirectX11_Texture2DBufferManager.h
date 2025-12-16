#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用2Dテクスチャバッファマネージャー
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


// ==============================================
// クラス
// ==============================================
class DirectX11_Texture2DBufferManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// 2Dテクスチャバッファ管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11Texture2D>> m_Texture2DBuffers;

public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_Texture2DBufferManager() = default;
	~DirectX11_Texture2DBufferManager() = default;


	// ------------------------------------------
	// 2Dテクスチャバッファ作成関数
	// ------------------------------------------
	const Handle Texture2DBufferCreate(
		ID3D11Device* _device,
		const D3D11_TEXTURE2D_DESC* _desc,
		const char* _name,
		const D3D11_SUBRESOURCE_DATA* _initialData = nullptr);


	// ------------------------------------------
	// 2Dテクスチャバッファ取得関数
	// ------------------------------------------
	ID3D11Texture2D* GetTexture2DBuffer(const Handle& _handle);


	// ------------------------------------------
	// 2Dテクスチャバッファチェック
	// ------------------------------------------
	bool Exists(const char* _name) const
	{
		return m_Texture2DBuffers.Exists(_name);
	}


	// ------------------------------------------
	// ハンドル取得
	// ------------------------------------------
	const Handle GetHandle(const char* _name) const
	{
		return m_Texture2DBuffers.GetHandle(_name);
	}
};
