// ヘッダー
#include "DirectX11_Texture2DBufferManager.h"
#include "ReportMessage.h"

// 2Dテクスチャバッファ作成
Handle DirectX11_Texture2DBufferManager::Texture2DBufferCreateOnGet(
	ID3D11Device* _device,
	const D3D11_TEXTURE2D_DESC& _desc,
	const Hashed_String& _name,
	const D3D11_SUBRESOURCE_DATA* _initialData)
{
	// 既に存在しているか確認
	if (m_Texture2DBuffers.Exists(_name))
	{
		Handle handle = m_Texture2DBuffers.GetHandle(_name);
#if defined(DEBUG) || defined(_DEBUG)
		if (!m_Texture2DBuffers.GetData(handle)->IsSame(_desc)) {
			ErrorLog::OutputToConsole(u8"同じ名前の2Dテクスチャバッファ : " + _name.GetString() + u8" が異なる記述で作成されそうになりました");
			return Handle();
		}
#endif
		DebugLog::OutputToConsole(u8"同じ2Dテクスチャバッファ : " + _name.GetString() + u8" はすでに存在しています");
		return handle;
	}

	// 作成
	DirectX11_Texture2DBufferData texture2DData;
	texture2DData.Create_DX11Texture2D(_device, _desc, _initialData);

	// データ追加
	return m_Texture2DBuffers.AddData(_name, texture2DData);
}

