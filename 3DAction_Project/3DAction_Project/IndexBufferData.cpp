
// ==================================
// ヘッダー
// ==================================
// 必須ヘッダー
#include "IndexBufferData.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ==================================
// バッファ作成
// ==================================
bool IndexBufferData::CreateIndexBuffer(
	ID3D11Device* device,
	const int* indexData,
	int indexNumber)
{
	// インデックスバッファ作成
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = sizeof(int) * indexNumber;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D10_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	// 初期化データを作成
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = indexData;

	// バッファ作成
	HRESULT hr = device->CreateBuffer(&desc, &initData, m_Buffer.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(("インデックスバッファが作成できませんでした" + std::to_string(hr)).c_str());
		return false;
	}

	return true;
}
