
// ヘッダー
#include "DirectX11_VertexBufferManager.h"
#include "ReportMessage.h"

// ===========================================
// 頂点バッファ作成
// ===========================================
Handle DirectX11_VertexBufferManager::VertexBufferCreateOnGet(
	ID3D11Device* _device,
	const Hashed_String& _name,
	const D3D11_BUFFER_DESC& _bufferDesc,
	const D3D11_SUBRESOURCE_DATA* _initData,
	const uint32_t& _vertexCount,
	const uint32_t& _stride)
{
	// 同じ名前の頂点バッファが存在するか確認
	if (m_VertexBuffers.Exists(_name)) 
	{
		DebugLog::OutputToConsole(u8"頂点バッファ : " + _name.GetString() + u8" はすでに存在しています");
		return m_VertexBuffers.GetHandle(_name);
	}

	// 頂点バッファデータ
	DirectX11_VertexBufferData bufferData;
	if (!bufferData.Create_DX11VertexBuffer(
		_device,
		_bufferDesc,
		_initData,
		_vertexCount,
		_stride))
	{
		ErrorLog::OutputToConsole(u8"頂点バッファの作成に失敗しました: " +_name.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_VertexBuffers.AddData((Hashed_String)_name, bufferData);
}
