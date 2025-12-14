
// ===========================================
// ヘッダー
// ===========================================
// 必須ヘッダー
#include "DirectX11_VertexBufferManager.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ===========================================
// 頂点バッファ作成
// ===========================================
const Handle DirectX11_VertexBufferManager::VertexBufferCreate(
	ID3D11Device* _device,
	const void* _vertices,
	size_t _size,
	D3D11_USAGE _usage,
	D3D11_CPU_ACCESS_FLAG _flag,
	const char* _name)
{
	if (!_device || !_vertices || _size == 0) {
		ErrorLog::OutputToConsole("無効な頂点バッファが作成されそうになりました");
		return Handle();
	}

	// 頂点バッファ作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	// 頂点バッファ作成情報設定
	D3D11_BUFFER_DESC desc{};
	desc.Usage = _usage;
	desc.ByteWidth = static_cast<UINT>(_size);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = _flag;
	
	// 初期データ設定
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = _vertices;

	// 頂点バッファ作成
	HRESULT hr = _device->CreateBuffer(&desc, &initData, buffer.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_VertexBuffers.AddData(_name, buffer);
}


// ===========================================
// 頂点バッファ取得
// ===========================================
ID3D11Buffer* DirectX11_VertexBufferManager::GetVertexBuffer(const Handle& _handle)
{
	return m_VertexBuffers.GetData(_handle)->Get();
}
