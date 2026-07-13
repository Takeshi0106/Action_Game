
// =====================================
// ヘッダー
// =====================================
// 必須ヘッダー
#include "DirectX11_Texture2DBufferData.h"
// レポートメッセージヘッダー
#include "ReportMessage.h"

// =====================================
// テクスチャ作成
// =====================================
bool DirectX11_Texture2DBufferData::Create_DX11Texture2D(
	ID3D11Device* _device,
	const D3D11_TEXTURE2D_DESC& _desc,
	const D3D11_SUBRESOURCE_DATA* _initialData)
{
	// 初期データあり
	if(_initialData != nullptr)
	{
		HRESULT hr = _device->CreateTexture2D(
			&_desc,
			_initialData,
			m_Texture2D.GetAddressOf());

		// 作成失敗
		if (FAILED(hr)) {
			ErrorLog::OutputToConsole(u8"テクスチャの作成に失敗しました " + 
				String::to_u8string((uint64_t)hr));
			return false;
		}
	}
	// 初期データなし
	else
	{
		HRESULT hr = _device->CreateTexture2D(
			&_desc,
			nullptr,
			m_Texture2D.GetAddressOf());

		// 作成失敗
		if (FAILED(hr)) {
			ErrorLog::OutputToConsole(u8"テクスチャの作成に失敗しました " + 
				String::to_u8string((uint64_t)hr));
			return false;
		}
	}


#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ用情報取得
	m_Desc = _desc;
#endif

	return true;
}

#if defined(DEBUG) || defined(_DEBUG)
// =====================================
// デバッグ用: テクスチャ記述が同じか確認
// =====================================
bool DirectX11_Texture2DBufferData::IsSame(
	const D3D11_TEXTURE2D_DESC& rhs) const
{
	return
		m_Desc.Width == rhs.Width &&
		m_Desc.Height == rhs.Height &&
		m_Desc.MipLevels == rhs.MipLevels &&
		m_Desc.ArraySize == rhs.ArraySize &&
		m_Desc.Format == rhs.Format &&
		m_Desc.SampleDesc.Count == rhs.SampleDesc.Count &&
		m_Desc.SampleDesc.Quality == rhs.SampleDesc.Quality &&
		m_Desc.Usage == rhs.Usage &&
		m_Desc.BindFlags == rhs.BindFlags &&
		m_Desc.CPUAccessFlags == rhs.CPUAccessFlags &&
		m_Desc.MiscFlags == rhs.MiscFlags;
}

#endif
