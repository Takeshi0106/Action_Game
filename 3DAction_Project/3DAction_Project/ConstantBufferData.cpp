
// 自分のヘッダー
#include "ConstantBufferData.h"
// ログ出力ヘッダー
#include "ReportMessage.h"


// ===============================================
// 定数バッファ作衛
// ===============================================
bool ConstantBufferData::CreateConstantBuffer(
    ID3D11Device* device,
	const void* data,
    size_t size,
    D3D11_USAGE usage,
    D3D11_CPU_ACCESS_FLAG cpuAccess)
{
	// サイズを計算
	m_Size = (size + 15) / 16 * 16;

	D3D11_BUFFER_DESC desc{}; // 初期化
	desc.ByteWidth = UINT(m_Size); // 16バイト単位
	desc.Usage = usage;                                        // CPUから書き込み可能
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;               // 定数バッファで作成
	desc.CPUAccessFlags = cpuAccess;                           // CPUから書き込み可能
	desc.MiscFlags = 0;                                        // フラグなし
	desc.StructureByteStride = 0;                              // 構造体バッファではない

	// 初期化データ作成
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = data;

	// バッファ作成
	Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
	HRESULT hr = device->CreateBuffer(&desc, &initData, buffer.GetAddressOf());

	// メンバー変数に代入
	m_Buffer = std::move(buffer);

	// エラーチェック
	if (FAILED(hr)) {
		return false;
	}

	return true;
}


// 定数バッファ更新
bool ConstantBufferData::UpdateConstantBuffer(ID3D11DeviceContext* context, const void* data, size_t size)
{
	D3D11_MAPPED_SUBRESOURCE mapped{};
	HRESULT hr = context->Map(m_Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
	if (SUCCEEDED(hr))
	{
		memcpy(mapped.pData, data, size);
		context->Unmap(m_Buffer.Get(), 0);

		m_IsUpdate = true;
	}
	else
	{
		ErrorLog::OutputToConsole("定数バッファが更新できませんでした");
		return false;
	}

	return true;
}
