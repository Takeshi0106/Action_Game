#pragma once

// ========================================
// 【クラス概要】
// DirectX11 用インデックスバッファデータ
// ========================================


// ========================================
// ヘッダー
// ========================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// 整数型ヘッダー
#include <cstdint>


// ========================================
// クラス
// ========================================
class DirectX11_IndexBufferData final
{
private:
	// -----------------------------------
	// メンバー変数
	// -----------------------------------
	// インデックスバッファ本体
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_IndexBuffer = nullptr;
	// インデックス数
	uint32_t m_IndexCount = 0;
	// フォーマット
	DXGI_FORMAT m_Format = DXGI_FORMAT_R32_UINT;


public:
	// -----------------------------------
	// コンストラクタ・デストラクタ
	// -----------------------------------
	DirectX11_IndexBufferData() = default;
	~DirectX11_IndexBufferData() = default;


	// -----------------------------------
	// インデックスバッファ作成関数
	// -----------------------------------
	bool Create_DX11IndexBuffer(
		ID3D11Device* _device,
		const D3D11_BUFFER_DESC& _bufferDesc,
		const D3D11_SUBRESOURCE_DATA* _initData,
		const uint32_t& _indexCount,
		const DXGI_FORMAT& _format);


	// -----------------------------------
	// ゲッター
	// -----------------------------------
	// インデックスバッファ取得
	ID3D11Buffer* GetIndexBuffer() const { return m_IndexBuffer.Get(); }
	// インデックス数取得
	uint32_t GetIndexCount() const { return m_IndexCount; }
	// フォーマット取得
	DXGI_FORMAT GetFormat() const { return m_Format; }
};

