
// ===============================================
// ヘッダー
// ===============================================
// 必須ヘッダー
#include "DirectX11_IndexBufferData.h"
// レポートメッセージヘッダー
#include "ReportMessage.h"

// ===============================================
// インデックスバッファ作成
// ===============================================
bool DirectX11_IndexBufferData::Create_DX11IndexBuffer(
	ID3D11Device* _device,
	const D3D11_BUFFER_DESC& _desc,
	const D3D11_SUBRESOURCE_DATA* _initialData,
	const uint32_t& _indexCount,
	const DXGI_FORMAT& _format)
{
	// インデックスバッファ作成
	HRESULT hr = _device->CreateBuffer(
		&_desc,
		_initialData,
		m_IndexBuffer.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"インデックスバッファの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// インデックス数保存
	m_IndexCount = _indexCount;
	// フォーマット保存
	m_Format = _format;

	return true;
}
