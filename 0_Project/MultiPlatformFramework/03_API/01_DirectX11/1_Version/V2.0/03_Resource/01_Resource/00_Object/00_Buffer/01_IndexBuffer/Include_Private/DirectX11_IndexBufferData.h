#pragma once

// ========================================
// 【クラス概要】
// DirectX11 用インデックスバッファデータ
// ========================================
// DirectXヘッダー
#include <d3d11.h>
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
	uint32_t m_IndexCount = 0;
	DXGI_FORMAT m_Format = DXGI_FORMAT_R32_UINT;

public:
	// コンストラクタ・デストラクタ
	DirectX11_IndexBufferData() = default;
	~DirectX11_IndexBufferData() = default;

	// 作成関数
	bool Create_DX11IndexBuffer(
		ID3D11Device* _device,
		const D3D11_BUFFER_DESC& _bufferDesc,
		const D3D11_SUBRESOURCE_DATA* _initData,
		const uint32_t& _indexCount,
		const DXGI_FORMAT& _format);

	// ゲッター
	ID3D11Buffer* GetIndexBuffer() const { return m_IndexBuffer.Get(); }
	uint32_t GetIndexCount() const { return m_IndexCount; }
	DXGI_FORMAT GetFormat() const { return m_Format; }

	// 削除
	void Reset() {
		m_IndexBuffer.Reset();
		m_IndexCount = 0;
		m_Format = DXGI_FORMAT_R32_UINT;
	}
};

