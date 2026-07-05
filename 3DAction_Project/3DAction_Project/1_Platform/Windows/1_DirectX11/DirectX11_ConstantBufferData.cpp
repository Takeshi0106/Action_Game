
// ============================================================
// ヘッダー
// ============================================================
// 必須ヘッダー
#include "DirectX11_ConstantBufferData.h"
// レポートメッセージヘッダー
#include "../../../ReportMessage.h"


// ============================================================
// 定数バッファ作成
// ============================================================
bool DirectX11_ConstantBufferData::Create_DX11ConstantBuffer(
	ID3D11Device* _device,
	const D3D11_BUFFER_DESC& _desc,
	const D3D11_SUBRESOURCE_DATA* _initialData)
{
	// 定数バッファ作成
	HRESULT hr = _device->CreateBuffer(
		&_desc,
		_initialData,
		m_ConstantBuffer.GetAddressOf());

	// 作成失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"定数バッファの作成に失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// 更新関数設定
	switch (_desc.Usage)
	{
	case D3D11_USAGE_DYNAMIC:
#if defined(DEBUG) || defined(_DEBUG)
		// CPUアクセスフラグ確認
		if ((m_DebugInfo.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == 0)
		{
			ErrorLog::OutputToConsole(u8"CPUアクセスフラグが不正です。");
			return false;
		}
#endif
		m_UpdateFunc = &DirectX11_ConstantBufferData::Update_DX11ConstantBuffer_Dynamic;
		break;

	case D3D11_USAGE_DEFAULT:
		m_UpdateFunc = &DirectX11_ConstantBufferData::Update_DX11ConstantBuffer_Default;
		break;

	default:
		ErrorLog::OutputToConsole(u8"このUsageでは定数バッファを更新できません。");
		return false;
	}

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ情報保存
	m_DebugInfo.BufferSize = (uint32_t)_desc.ByteWidth;
	m_DebugInfo.Usage = _desc.Usage;
	m_DebugInfo.CPUAccessFlags = (uint32_t)_desc.CPUAccessFlags;
#endif

	return true;
}


// ============================================================
// 定数バッファ更新
// ============================================================
bool DirectX11_ConstantBufferData::Update_DX11ConstantBuffer(
	ID3D11DeviceContext* _context,
	const BinaryView& _binaryView)
{
#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ情報サイズ確認
	if (m_DebugInfo.BufferSize != _binaryView.GetSize()) {
		ErrorLog::OutputToConsole(u8"定数バッファのサイズが異なります。");
		return false;
	}
#endif

	// 更新関数呼び出し
	(this->*m_UpdateFunc)(_context, _binaryView);

	return true;
}


// ============================================================
// 定数バッファ更新　（毎フレーム更新）	
// ============================================================
bool DirectX11_ConstantBufferData::Update_DX11ConstantBuffer_Dynamic(
	ID3D11DeviceContext* _context,
	const BinaryView& _binaryView)
{
	// マップ
	D3D11_MAPPED_SUBRESOURCE mappedResource{};

	// バッファマップ
	HRESULT hr = _context->Map(
		m_ConstantBuffer.Get(),
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource);

	// マップ失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"定数バッファのマップに失敗しました " +
			String::to_u8string((uint64_t)hr));
		return false;
	}

	// データコピー
	std::memcpy(
		mappedResource.pData,
		_binaryView.GetData(),
		_binaryView.GetSize());

	// アンマップ
	_context->Unmap(m_ConstantBuffer.Get(), 0);
	
	return true;
}


// ============================================================
// 定数バッファ更新　（たまに変わるデータ更新）
// ============================================================
bool DirectX11_ConstantBufferData::Update_DX11ConstantBuffer_Default(
	ID3D11DeviceContext* _context,
	const BinaryView& _binaryView)
{
	_context->UpdateSubresource(
		m_ConstantBuffer.Get(),
		0,
		nullptr,
		_binaryView.GetData(),
		0,
		0);
	
	return true;
}


// ============================================================
// デバッグ情報同一確認
// ============================================================
#if defined(DEBUG) || defined(_DEBUG)
bool DirectX11_ConstantBufferData::IsSame(
	const DirectX11_DebugInfo& _info) const
{
	// バッファサイズが異なる
	if (m_DebugInfo.BufferSize != _info.BufferSize) {
		return false;
	}

	// 使用方法が異なる
	if (m_DebugInfo.Usage != _info.Usage) {
		return false;
	}

	// CPUアクセスフラグが異なる
	if (m_DebugInfo.CPUAccessFlags != _info.CPUAccessFlags) {
		return false;
	}

	return true;
}
#endif
