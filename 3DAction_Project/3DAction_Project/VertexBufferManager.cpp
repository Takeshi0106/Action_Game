
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "VertexBufferManager.h"
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> VertexBufferManager::m_VertexBuffers;


// ======================================
// 前方宣言
// ======================================
struct VertexBufferData
{
    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer = nullptr;
    size_t size = 0;
    int stride = 0;
    int offset = 0;
    int slot = 0;
    int vertexCount = 0; // 頂点数

    VertexBufferData() = default;
    VertexBufferData(ID3D11Buffer* buf, size_t _size, int _stride, int _vertexCount, int _slot = 0)
        : buffer(buf), size(_size), stride(_stride), slot(_slot), vertexCount(_vertexCount), offset(0) {
    }
};


// =======================================
// 頂点バッファ作成
// =======================================
// 頂点バッファ作成
bool VertexBufferManager::CreateVertexBuffer(const std::string& name,
    ID3D11Device* device,
    const void* vertices,
    int vertexCount,
    int stride,
    int slot)
{
    if (Exists(name)) return true; // 既に作成済み

    D3D11_BUFFER_DESC desc{};
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.ByteWidth = stride * vertexCount;
    desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    desc.CPUAccessFlags = 0;

    D3D11_SUBRESOURCE_DATA initData{};
    initData.pSysMem = vertices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;
    HRESULT hr = device->CreateBuffer(&desc, &initData, buffer.GetAddressOf());
    if (FAILED(hr)) return false;

    auto vbd = std::make_unique<VertexBufferData>(buffer.Get(), desc.ByteWidth, stride, vertexCount, slot);
    m_VertexBuffers[name] = std::move(vbd);

    Log(name.c_str());

    return true;
}


// =======================================
// 頂点バッファを探して、戻り値で返す
// =======================================
ID3D11Buffer* VertexBufferManager::GetFindVertexBuffer(const std::string& name) const
{
	// 探す
	auto it = m_VertexBuffers.find(name);

	if (it != m_VertexBuffers.end())
	{
		// 頂点バッファを返す
		return it->second->buffer.Get();
	}
	else {
		WarningLog::OutputToConsole(std::string(" 頂点バッファ : " + name + " が見つかりませんでした").c_str());
	}

	return nullptr;
}


// ========================================
// 頂点バッファがあるかのチェック
// ========================================
bool VertexBufferManager::Exists(const std::string& name) const
{
    return m_VertexBuffers.find(name) != m_VertexBuffers.end();
}


// =======================================
// 後処理
// =======================================
void VertexBufferManager::ReleaseAllVertexBuffers()
{
    m_VertexBuffers.clear();
}

