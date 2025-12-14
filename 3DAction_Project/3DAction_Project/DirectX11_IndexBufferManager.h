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
#include "TemplateManager.h"


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
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_IndexBuffers;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_IndexBufferManager() = default;
	~DirectX11_IndexBufferManager() = default;


	// ------------------------------------------
	// インデックスバッファ作成関数
	// ------------------------------------------
	const Handle IndexBufferCreate(
		ID3D11Device* _device,
		const void* _indices,
		size_t _size,
		D3D11_USAGE _usage,
		D3D11_CPU_ACCESS_FLAG _flag,
		const char* _name);


	// ------------------------------------------
	// インデックスバッファ取得関数
	// ------------------------------------------
	ID3D11Buffer* GetIndexBuffer(const Handle& _handle);
};

