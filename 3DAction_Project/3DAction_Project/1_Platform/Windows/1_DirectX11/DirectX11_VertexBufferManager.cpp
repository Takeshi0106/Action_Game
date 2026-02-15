
// ===========================================
// ヘッダー
// ===========================================
// 必須ヘッダー
#include "DirectX11_VertexBufferManager.h"
// ログ出力用ヘッダー
#include "../../../ReportMessage.h"


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
		// ログ出力
		WarningLog::OutputToConsole(
			_name.GetString() + u8" 同じ名前の頂点バッファが再作成されました");

		// 存在する場合はハンドルを返す
		return m_VertexBuffers.GetHandle(_name);
	}

	// 頂点バッファデータ
	DirectX11_VertexBufferData bufferData;

	// 頂点バッファ作成
	if (!bufferData.Create_DX11VertexBuffer(
		_device,
		_bufferDesc,
		_initData,
		_vertexCount,
		_stride))
	{
		// ログ出力
		ErrorLog::OutputToConsole(
			u8"頂点バッファの作成に失敗しました: " +
			_name.GetString());

		// 失敗したら空ハンドルを返す
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_VertexBuffers.AddData((Hashed_String)_name, bufferData);
}
