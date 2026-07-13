#pragma once
// ==============================================
// 【クラス概要】
// DirectX11用2Dテクスチャバッファマネージャー
// APIObject を管理するマネージャー
// ==============================================
#include "DirectX11_Texture2DBufferData.h"
#include "TemplateManager.h"
#include "Hashed_String.h"

// ==============================================
// クラス
// ==============================================
class DirectX11_Texture2DBufferManager final
{
private:
	// メンバー変数
	TemplateManager<DirectX11_Texture2DBufferData> m_Texture2DBuffers;

public:
	// コンストラクタ・デストラクタ
	DirectX11_Texture2DBufferManager() = default;
	~DirectX11_Texture2DBufferManager() = default;

	// 作成関数
	Handle Texture2DBufferCreateOnGet(
		ID3D11Device* _device,
		const D3D11_TEXTURE2D_DESC& _desc,
		const Hashed_String& _name,
		const D3D11_SUBRESOURCE_DATA* _initialData = nullptr);

	// 取得関数
	const DirectX11_Texture2DBufferData* GetTexture2DBuffer(const Handle& _handle) {
		return m_Texture2DBuffers.GetData(_handle);
	}

	// 削除
	void ReleaseTexture2D(const Handle& _handle) {
		m_Texture2DBuffers.Remove(_handle);
	}

	// 全て削除
	void ReleaseAllTexture2D() {
		m_Texture2DBuffers.ALLClear();
	}
};
