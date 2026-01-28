
// ============================================================
// ヘッダー
// ============================================================
// 必須ヘッダー
#include "DirectX11_ConstantBufferManager.h"
// ログ出力用ヘッダー
#include "../ReportMessage.h"


// ============================================================
// 定数バッファ作成
// ============================================================
const Handle DirectX11_ConstantBufferManager::ConstantBufferCreate(
	ID3D11Device* _device,
	size_t _size,
	D3D11_USAGE _usage,
	D3D11_CPU_ACCESS_FLAG _flag,
	const String& _name,
	const void* _data)
{
	if (!_device || _size == 0) {
		ErrorLog::OutputToConsole(u8"無効な定数バッファが作成されそうになりました");
		return Handle();
	}

	// 定数バッファ作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

	if(_data != nullptr) 
	{
		// 初期データがある場合
		// 定数バッファ作成情報設定
		D3D11_BUFFER_DESC desc{};
		desc.Usage = _usage;
		desc.ByteWidth = static_cast<UINT>(_size);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = _flag;

		// 初期データ設定
		D3D11_SUBRESOURCE_DATA initData{};
		initData.pSysMem = _data;

		// 定数バッファ作成
		HRESULT hr = _device->CreateBuffer(&desc, &initData, buffer.GetAddressOf());
		if (FAILED(hr)) {
			return Handle();
		}
	}
	else 
	{
		// 初期データが無い場合
		// 定数バッファ作成情報設定
		D3D11_BUFFER_DESC desc{};
		desc.Usage = _usage;
		desc.ByteWidth = static_cast<UINT>(_size);
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = _flag;

		// 定数バッファ作成
		HRESULT hr = _device->CreateBuffer(&desc, nullptr, buffer.GetAddressOf());
		if (FAILED(hr)) {
			return Handle();
		}
	}

	// 管理配列に追加してハンドルを返す
	return m_ConstantBuffers.AddData((Hashed_String)_name, buffer);
}


// ============================================================
// 定数バッファ取得
// ============================================================
ID3D11Buffer* DirectX11_ConstantBufferManager::GetConstantBuffer(const Handle& _handle)
{
	return m_ConstantBuffers.GetData(_handle)->Get();
}
