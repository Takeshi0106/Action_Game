
// ============================================================
// ヘッダー
// ============================================================
// 必須ヘッダー
#include "DirectX11_VertexBufferData.h"
// ログ出力用ヘッダー
#include "../../../ReportMessage.h"


// ============================================================
// 頂点バッファ作成
// ============================================================
bool DirectX11_VertexBufferData::Create_DX11VertexBuffer(
	ID3D11Device* _device,
	const D3D11_BUFFER_DESC& _desc,
	const D3D11_SUBRESOURCE_DATA* _initialData,
	const uint32_t& _vertexCount,
	const uint32_t& _stride)
{
	// 頂点バッファ作成
	HRESULT hr = _device->CreateBuffer(
		&_desc,
		_initialData,
		m_VertexBuffer.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"頂点バッファの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// 頂点数取得
	m_VertexCount = _vertexCount;
	// スライド取得
	m_Stride = _stride;

	return true;
}
