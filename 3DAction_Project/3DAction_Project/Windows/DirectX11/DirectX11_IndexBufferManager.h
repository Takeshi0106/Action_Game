#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用インデックスバッファマネージャー
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
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ==============================================
// 構造体宣言
// ==============================================
struct IndexBufferData
{
	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer = nullptr;
	// インデックス数
	uint32_t indexCount = 0;
};


// ==============================================
// クラス
// ==============================================
class DirectX11_IndexBufferManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// インデックスバッファ管理
	TemplateManager<IndexBufferData> m_IndexBuffers;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_IndexBufferManager() = default;
	~DirectX11_IndexBufferManager() = default;


	// ------------------------------------------
	// インデックスバッファ作成関数
	// ------------------------------------------
	const Handle IndexBufferCreateOnGet(
		ID3D11Device* _device,
		const void* _indices,
		const size_t _size,
		const uint32_t _indexCount,
		D3D11_USAGE _usage,
		D3D11_CPU_ACCESS_FLAG _flag,
		const Hashed_String& _name);


	// ------------------------------------------
	// インデックスバッファ取得関数
	// ------------------------------------------
	IndexBufferData* GetIndexBuffer(const Handle& _handle);


	// ------------------------------------------
	// インデックスバッファ削除関数
	// ------------------------------------------
	void ReleaseIndexBuffer(const Handle& _handle);


	// ------------------------------------------
	// 全てのインデックスバッファ削除関数
	// ------------------------------------------
	void ReleaseAllIndexBuffer();

};

