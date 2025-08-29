
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "VertexBufferManager.h"
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// 頂点バッファデータヘッダー
#include "VertexBufferData.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> VertexBufferManager::m_VertexBuffers;


// =======================================
// 頂点バッファ作成
// =======================================
// 頂点バッファ作成
bool VertexBufferManager::CreateVertexBuffer(const std::string& name,
    ID3D11Device* device,
    const void* vertices,
    int vertexCount,
    int stride)
{
    // 既に作成済み
    if (Exists(name)) { return true; }

    auto vbd = std::make_unique<VertexBufferData>(name);

    if (!vbd->CreateVertexBuffer(device,
        vertices,
        vertexCount,
        vertexCount,
        stride,
        D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP))
    {
        ErrorLog::OutputToConsole("頂点バッファの作成に失敗");
        return false;
    }

    m_VertexBuffers[name] = std::move(vbd);

    // 作製した頂点バッファの名前を保存
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
		return it->second->GetVertexBuffer();
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

