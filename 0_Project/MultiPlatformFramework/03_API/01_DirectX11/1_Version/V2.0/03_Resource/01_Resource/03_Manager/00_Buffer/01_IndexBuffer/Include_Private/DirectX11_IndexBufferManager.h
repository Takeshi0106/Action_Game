#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用インデックスバッファマネージャー
// APIObject を管理するマネージャー
// ==============================================
#include "DirectX11_IndexBufferData.h"
#include "TemplateManager.h"
#include "Hashed_String.h"

// ==============================================
// クラス
// ==============================================
class DirectX11_IndexBufferManager final
{
private:
	// メンバー変数
	TemplateManager<DirectX11_IndexBufferData> m_IndexBuffers;

public:
	// コンストラクタ・デストラクタ
	DirectX11_IndexBufferManager() = default;
	~DirectX11_IndexBufferManager() = default;
	
	// インデックスバッファ作成関数
	Handle IndexBufferCreateOnGet(
		ID3D11Device* _device,
		const Hashed_String& _ibName,
		const D3D11_BUFFER_DESC& _bufferDesc,
		const D3D11_SUBRESOURCE_DATA* _initData,
		const uint32_t& _indexCount,
		const DXGI_FORMAT& _format = DXGI_FORMAT_R32_UINT);
	
	// インデックスバッファ取得関数
	const DirectX11_IndexBufferData* GetIndexBuffer(const Handle& _handle) {
		return m_IndexBuffers.GetData(_handle);
	}
	
	// インデックスバッファ削除関数
	void ReleaseIndexBuffer(const Handle& _handle) {
		m_IndexBuffers.Remove(_handle);
	}
	
	// 全てのインデックスバッファ削除関数
	void ReleaseAllIndexBuffer() {
		m_IndexBuffers.ALLClear();
	}
};

