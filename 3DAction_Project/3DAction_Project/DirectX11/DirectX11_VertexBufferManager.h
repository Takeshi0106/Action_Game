#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用頂点バッファマネージャー
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
class DirectX11_VertexBufferManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// 頂点バッファ管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_VertexBuffers;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_VertexBufferManager() = default;
	~DirectX11_VertexBufferManager() = default;


	// ------------------------------------------
	// 頂点バッファ作成関数
	// ------------------------------------------
	const Handle VertexBufferCreate(
		ID3D11Device* _device,
		const void* _vertices,
		size_t _size,
		D3D11_USAGE _usage,
		D3D11_CPU_ACCESS_FLAG _flag,
		const char* _name);


	// ------------------------------------------
	// 頂点バッファ取得関数
	// ------------------------------------------
	ID3D11Buffer* GetVertexBuffer(const Handle& _handle);


	// ------------------------------------------
	// 頂点バッファチェック
	// ------------------------------------------
	bool Exists(const char* _name) const {
		return m_VertexBuffers.Exists(_name);
	}


	// ------------------------------------------
	// 頂点バッファハンドル取得関数
	// ------------------------------------------
	const Handle GetVertexBufferHandle(const char* _name) const {
		return m_VertexBuffers.GetHandle(_name);
	}
};

