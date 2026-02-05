
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
const Handle DirectX11_Texture2DBufferManager::Texture2DBufferCreateOnGet(
	ID3D11Device* _device,
	const D3D11_TEXTURE2D_DESC* _desc,
	const Hashed_String& _name,
	const D3D11_SUBRESOURCE_DATA* _initialData)
{
	// 既に存在しているか確認
	if (m_Texture2DBuffers.Exists(_name)) {
		WarningLog::OutputToConsole(u8"2Dテクスチャバッファ : " +
			_name.GetString() + u8" はすでに存在しています");
		return m_Texture2DBuffers.GetHandle(_name);
	}

	// エラーチェック
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
	return m_Texture2DBuffers.AddData(_name, buffer);
}


// ========================================================================
// 2Dテクスチャバッファ取得
// ========================================================================
ID3D11Texture2D* DirectX11_Texture2DBufferManager::GetTexture2DBuffer(const Handle& _handle)
{
	return m_Texture2DBuffers.GetData(_handle)->Get();
}


// ========================================================================
// 2Dテクスチャ削除
// ========================================================================
void DirectX11_Texture2DBufferManager::ReleaseTexture2D(const Handle& _handle)
{
	return m_Texture2DBuffers.Remove(_handle);
}


// =========================================================================
// 全てのテクスチャ2D削除
// =========================================================================
void DirectX11_Texture2DBufferManager::ReleaseAllTexture2D()
{
	m_Texture2DBuffers.ALLClear();
}
