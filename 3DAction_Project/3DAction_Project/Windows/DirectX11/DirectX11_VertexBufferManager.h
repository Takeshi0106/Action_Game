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
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ==============================================
// 構造体宣言
// ==============================================
struct VertexBufferData
{
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer = nullptr;
	// 頂点数
	uint32_t vertexCount = 0;
};


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
	TemplateManager<VertexBufferData> m_VertexBuffers;


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
		const size_t _size,
		const uint32_t _vertexCount,
		D3D11_USAGE _usage,
		D3D11_CPU_ACCESS_FLAG _flag,
		const String& _name);


	// ------------------------------------------
	// 頂点バッファ取得関数
	// ------------------------------------------
	VertexBufferData* GetVertexBuffer(const Handle& _handle);
	

	// ------------------------------------------
	// 頂点バッファ削除関数
	// ------------------------------------------
	void ReleaseVertexBuffer(const Handle& _handle);


	// ------------------------------------------
	// 全頂点バッファ削除関数
	// ------------------------------------------
	void ReleaseAllVertexBuffers();


	// ------------------------------------------
	// 頂点バッファチェック
	// ------------------------------------------
	bool Exists(const String& _name) const {
		return m_VertexBuffers.Exists((Hashed_String)_name);
	}


	// ------------------------------------------
	// 頂点バッファハンドル取得関数
	// ------------------------------------------
	const Handle GetVertexBufferHandle(const String& _name) const {
		return m_VertexBuffers.GetHandle((Hashed_String)_name);
	}
};

