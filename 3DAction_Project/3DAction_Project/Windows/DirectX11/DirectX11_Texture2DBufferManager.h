#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用2Dテクスチャバッファマネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// テクスチャ2Dの実態
#include "DirectX11_Texture2DData.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../Hashed_String.h"


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
	TemplateManager<DirectX11_Texture2DData> m_Texture2DBuffers;

public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_Texture2DBufferManager() = default;
	~DirectX11_Texture2DBufferManager() = default;


	// ------------------------------------------
	// 2Dテクスチャバッファ作成関数
	// ------------------------------------------
	const Handle Texture2DBufferCreateOnGet(
		ID3D11Device* _device,
		const D3D11_TEXTURE2D_DESC& _desc,
		const Hashed_String& _name,
		const D3D11_SUBRESOURCE_DATA* _initialData = nullptr);


	// ------------------------------------------
	// 2Dテクスチャバッファ取得関数
	// ------------------------------------------
	ID3D11Texture2D* GetTexture2DBuffer(const Handle& _handle);


	// ------------------------------------------
	// 2Dテクスチャ削除
	// ------------------------------------------
	void ReleaseTexture2D(const Handle& _handle);


	// ------------------------------------------
	// 全ての2Dテクスチャ削除
	// ------------------------------------------
	void ReleaseAllTexture2D();
};
