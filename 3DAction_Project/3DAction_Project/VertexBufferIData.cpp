
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "VertexBufferData.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ==========================================
// 頂点バッファ作成
// ==========================================
bool VertexBufferData::CreateVertexBuffer(
	ID3D11Device* device,
	const void* vertices,
	int vertexCount,
	int vertexMaxCount,
	size_t stride,
	D3D11_PRIMITIVE_TOPOLOGY primitiveType,
	D3D11_USAGE usage,
	D3D11_CPU_ACCESS_FLAG flag)
{
	if (!device || !vertices || vertexCount <= 0 || stride <= 0) {
		ErrorLog::OutputToConsole("無効な頂点バッファが作成されそうになりました");
		return false;
	}

	// メンバー変数に代入
	m_VertexCount = vertexCount;
	m_VertexMaxCount = vertexMaxCount;
	m_Stride = stride;
	m_IsUpdate = true;
	m_PrimitiveType = primitiveType;
	m_Size = size_t(stride * vertexMaxCount);

	// 頂点バッファ作成
	D3D11_BUFFER_DESC desc{};
	desc.Usage = usage;
	desc.ByteWidth = static_cast<UINT>(m_Size);
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = flag;

	// 初期情報を代入
	D3D11_SUBRESOURCE_DATA initData{};
	initData.pSysMem = vertices;

	// 作成できたかチェック
	HRESULT hr = device->CreateBuffer(&desc, &initData, m_Buffer.GetAddressOf());
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(("頂点バッファが作成できませんでした" + std::to_string(hr)).c_str());
		return false;
	}

	return true;
}


// 頂点バッファ更新
bool VertexBufferData::UpdateBuffer(ID3D11DeviceContext* context, const void* data, int size)
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
		ErrorLog::OutputToConsole("頂点バッファが更新できませんでした");
		return false;
	}

	return true;
}