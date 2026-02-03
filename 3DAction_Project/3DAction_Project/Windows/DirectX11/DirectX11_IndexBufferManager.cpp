
// ===========================================================================
// ヘッダー
// ===========================================================================
// 必須ヘッダー
#include "DirectX11_IndexBufferManager.h"
// ログ出力用ヘッダー
#include "../../ReportMessage.h"


// ===========================================================================
// インデックスバッファ作成
// ===========================================================================
const Handle DirectX11_IndexBufferManager::IndexBufferCreate(
	ID3D11Device* _device,
	const void* _indices,
	const size_t _size,
	const uint32_t _indexCount,
	D3D11_USAGE _usage,
	D3D11_CPU_ACCESS_FLAG _flag,
	const String& _name)
{

	if (!_device || !_indices || _size == 0) {
		ErrorLog::OutputToConsole(u8"無効なインデックスバッファが作成されそうになりました");
		return Handle();
	}

	// インデックスバッファ作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	// インデックスバッファ作成情報設定
	D3D11_BUFFER_DESC desc{};
	desc.Usage = _usage;
	desc.ByteWidth = static_cast<UINT>(_size);
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = _flag;

	// 初期データ設定
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = _indices;
	
	// インデックスバッファ作成
	HRESULT hr = _device->CreateBuffer(&desc, &initData, buffer.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}
	
	// インデックスバッファデータ作成
	IndexBufferData bufferData{};
	bufferData.indexBuffer = std::move(buffer);
	bufferData.indexCount = _indexCount;

	// 管理配列に追加してハンドルを返す
	return m_IndexBuffers.AddData((Hashed_String)_name, bufferData);
}


// ===========================================================================
// インデックスバッファ取得
// ===========================================================================
IndexBufferData* DirectX11_IndexBufferManager::GetIndexBuffer(const Handle& _handle)
{
	return m_IndexBuffers.GetData(_handle);
}
