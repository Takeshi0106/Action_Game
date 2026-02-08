#pragma once

// ==============================================
// 【クラス概要】
// DirectX11 用頂点バッファデータ
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// 整数ヘッダー
#include <cstdint>


// ==============================================
// クラス
// ==============================================
class DirectX11_VertexBufferData final
{
private:
	// -----------------------------------
	// メンバー変数
	// -----------------------------------
	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_VertexBuffer = nullptr;
	// 頂点数
	uint32_t m_VertexCount = 0;
	// スライド
	uint32_t m_Stride = 0;


public:
	// -----------------------------------
	// コンストラクタ・デストラクタ
	// -----------------------------------
	DirectX11_VertexBufferData() = default;
	~DirectX11_VertexBufferData() = default;


	// -----------------------------------
	// 頂点シェーダー作成関数
	// -----------------------------------
	bool Create_DX11VertexBuffer(
		ID3D11Device* _device,
		const D3D11_BUFFER_DESC& _bufferDesc,
		const D3D11_SUBRESOURCE_DATA* _initData,
		const uint32_t& _vertexCount,
		const uint32_t& _stride);

	// -----------------------------------
	// ゲッター
	// -----------------------------------
	// 頂点バッファ取得
	ID3D11Buffer* GetVertexBuffer() const { return m_VertexBuffer.Get(); }
	// 頂点数取得
	uint32_t GetVertexCount() const { return m_VertexCount; }

	// -----------------------------------
	// 削除
	// -----------------------------------
	void Reset() { 
		m_VertexBuffer.Reset();
		m_VertexCount = 0;
		m_Stride = 0;
	}
};

