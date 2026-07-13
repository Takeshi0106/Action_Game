
// ヘッダー
#include "DirectX11_ConstantBufferManager.h"
#include "ReportMessage.h"

// 定数バッファ作成
Handle DirectX11_ConstantBufferManager::ConstantBufferCreateOnGet(
	ID3D11Device* _device,
	const Hashed_String& _cbName,
	const D3D11_BUFFER_DESC& _desc,
	const D3D11_SUBRESOURCE_DATA* _initData)
{
	// すでに存在しているか確認
	if (m_ConstantBuffers.Exists(_cbName)) 
	{
		Handle handle = m_ConstantBuffers.GetHandle(_cbName);
#if defined(DEBUG) || defined(_DEBUG)
		// 同一確認
		DirectX11_DebugInfo debugInfo;
		debugInfo.BufferSize = (uint32_t)_desc.ByteWidth;
		debugInfo.Usage = _desc.Usage;
		debugInfo.CPUAccessFlags = (uint32_t)_desc.CPUAccessFlags;
		if (!m_ConstantBuffers.GetData(handle)->IsSame(debugInfo))
		{
			WarningLog::OutputToConsole(u8"定数バッファ : " + _cbName.GetString() + u8" は異なる設定で作成されようとしました");
			return Handle();
		}
#endif

		DebugLog::OutputToConsole(u8"定数バッファ : " + _cbName.GetString() + u8" はすでに存在しています");
		return handle;
	}

	// 定数バッファ
	DirectX11_ConstantBufferData bufferData;
	if (!bufferData.Create_DX11ConstantBuffer(
		_device,
		_desc,
		_initData))
	{
		ErrorLog::OutputToConsole(u8"定数バッファの作成に失敗しました: " + _cbName.GetString());
		return Handle();
	}

	// 管理配列に追加してハンドルを返す
	return m_ConstantBuffers.AddData(_cbName, bufferData);
}
