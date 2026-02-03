
// ========================================================================
// ヘッダー
// ========================================================================
// 必須ヘッダー
#include "DirectX11_Texture2DBufferManager.h"
// ログ出力用ヘッダー
#include "../../ReportMessage.h"


// ========================================================================
// 2Dテクスチャバッファ作成
// ========================================================================
const Handle DirectX11_Texture2DBufferManager::Texture2DBufferCreate(
	ID3D11Device* _device,
	const D3D11_TEXTURE2D_DESC* _desc,
	const String& _name,
	const D3D11_SUBRESOURCE_DATA* _initialData)
{
	if (!_device || !_desc) {
		ErrorLog::OutputToConsole(u8"無効な2Dテクスチャバッファが作成されそうになりました");
		return Handle();
	}

	// 2Dテクスチャバッファ作成
	Microsoft::WRL::ComPtr<ID3D11Texture2D> buffer;

	// 2Dテクスチャバッファ作成
	HRESULT hr = _device->CreateTexture2D(_desc, _initialData, buffer.GetAddressOf());
	if (FAILED(hr)) {
		return Handle();
	}
	
	// 管理配列に追加してハンドルを返す
	return m_Texture2DBuffers.AddData((Hashed_String)_name, buffer);
}


// ========================================================================
// 2Dテクスチャバッファ取得
// ========================================================================
ID3D11Texture2D* DirectX11_Texture2DBufferManager::GetTexture2DBuffer(const Handle& _handle)
{
	return m_Texture2DBuffers.GetData(_handle)->Get();
}
