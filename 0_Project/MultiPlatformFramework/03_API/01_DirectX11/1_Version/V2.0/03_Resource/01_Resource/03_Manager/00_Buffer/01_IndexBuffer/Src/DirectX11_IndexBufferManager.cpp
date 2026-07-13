// ヘッダー
#include "DirectX11_IndexBufferManager.h"
#include "ReportMessage.h"

// インデックスバッファ作成
Handle DirectX11_IndexBufferManager::IndexBufferCreateOnGet(
	ID3D11Device* _device,
	const Hashed_String& _ibName,
	const D3D11_BUFFER_DESC& _bufferDesc,
	const D3D11_SUBRESOURCE_DATA* _initData,
	const uint32_t& _indexCount,
	const DXGI_FORMAT& _format)
{
	// 既に存在しているか確認
	if (m_IndexBuffers.Exists(_ibName))
	{
		DebugLog::OutputToConsole(u8"インデックスバッファ : " + _ibName.GetString() + u8" はすでに存在しています");
		return m_IndexBuffers.GetHandle(_ibName);
	}

	// インデックスバッファ
	DirectX11_IndexBufferData bufferData;
	if (bufferData.Create_DX11IndexBuffer(
		_device,
		_bufferDesc,
		_initData,
		_indexCount,
		_format))
	{
		ErrorLog::OutputToConsole(u8"インデックスバッファ : " + _ibName.GetString() + u8" の作成に失敗しました");
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_IndexBuffers.AddData(_ibName, bufferData);
}
