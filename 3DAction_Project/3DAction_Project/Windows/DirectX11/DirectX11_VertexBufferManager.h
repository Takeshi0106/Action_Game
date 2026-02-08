#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用頂点バッファマネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// 頂点シェーダーデータ
#include "DirectX11_VertexBufferData.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// ハッシュ文字列ヘッダー
#include "../../Hashed_String.h"
// ハンドルヘッダー
#include "../../Handle.h"


// ==============================================
// クラス
// ==============================================
class DirectX11_VertexBufferManager final
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// 頂点バッファ管理
	TemplateManager<DirectX11_VertexBufferData> m_VertexBuffers;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_VertexBufferManager() = default;
	~DirectX11_VertexBufferManager() = default;


	// ------------------------------------------
	// 頂点バッファ作成関数
	// ------------------------------------------
	Handle VertexBufferCreateOnGet(
		ID3D11Device* _device,
		const Hashed_String& _vbName,
		const D3D11_BUFFER_DESC& _bufferDesc,
		const D3D11_SUBRESOURCE_DATA* _initData,
		const uint32_t& _vertexCount,
		const uint32_t& _stride);


	// ------------------------------------------
	// 頂点バッファ取得関数
	// ------------------------------------------
	const DirectX11_VertexBufferData* GetVertexBuffer(const Handle& _handle) {
		return m_VertexBuffers.GetData(_handle);
	}


	// ------------------------------------------
	// 頂点バッファ削除関数
	// ------------------------------------------
	void ReleaseVertexBuffer(const Handle& _handle) {
		m_VertexBuffers.Remove(_handle);
	}


	// ------------------------------------------
	// 全頂点バッファ削除関数
	// ------------------------------------------
	void ReleaseAllVertexBuffers() {
		m_VertexBuffers.ALLClear();
	}
};
