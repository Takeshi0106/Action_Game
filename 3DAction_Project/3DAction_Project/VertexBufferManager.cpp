
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "VertexBufferManager.h"
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
#include <wrl/client.h>   // スマートポインター
// 頂点バッファデータ
#include "VertexBufferData.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> VertexBufferManager::m_VertexBuffers;


// =======================================
// プロトタイプ宣言
// =======================================
inline D3D11_PRIMITIVE_TOPOLOGY ToDX(PrimitiveType type);
inline D3D11_USAGE ToDX(BufferUsage usage);
inline D3D11_CPU_ACCESS_FLAG ToDX(CPUAccess access);


// =======================================
// 頂点バッファ作成
// =======================================
// 頂点バッファ作成
bool VertexBufferManager::CreateVertexBuffer(
    const std::string& name,
    ID3D11Device* device,
    const void* vertices,
    int vertexCount,
    int vertexMaxCount,
    int stride,
    PrimitiveType type,
    BufferUsage usage,
    CPUAccess access)
{
    // 既に作成済み
    if (Exists(name)) { return true; }

    auto vbd = std::make_unique<VertexBufferData>(name);

    if (!vbd->CreateVertexBuffer(
        device,
        vertices,
        vertexCount,
        vertexMaxCount,
        stride,
        ToDX(type),
        ToDX(usage),
        ToDX(access)))
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
VertexBufferData* VertexBufferManager::GetFindVertexData(const std::string& name) const
{
	// 探す
	auto it = m_VertexBuffers.find(name);

	if (it != m_VertexBuffers.end())
	{
		// 頂点データを返す
        return it->second.get();
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


// =======================================
// 関数
// =======================================
inline D3D11_PRIMITIVE_TOPOLOGY ToDX(PrimitiveType type)
{
    switch (type)
    {
    case PrimitiveType::TriangleList: {
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
        break;
    }
    case PrimitiveType::TriangleStrip: {
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
        break;
    }
    case PrimitiveType::LineList: {
        return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
        break;
    }
    case PrimitiveType::LineStrip: {
        return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
        break;
    }
    default: {
        ErrorLog::OutputToConsole("トポロギーに変換できませんでした");
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
    }
}
inline D3D11_USAGE ToDX(BufferUsage usage)
{
    switch (usage)
    {
    case BufferUsage::Default: {
        return D3D11_USAGE_DEFAULT;
        break;
    }
    case BufferUsage::Dynamic: {
        return D3D11_USAGE_DYNAMIC;
        break;
    }
    default: {
        ErrorLog::OutputToConsole("BufferUsageに変換できませんでした");
        return D3D11_USAGE_DEFAULT;
    }
    }
}

inline D3D11_CPU_ACCESS_FLAG ToDX(CPUAccess access)
{
    switch (access)
    {
    case CPUAccess::None: {
        return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
        break;
    }
    case CPUAccess::Write: {
        return D3D11_CPU_ACCESS_WRITE;
        break;
    }
    default: {
        ErrorLog::OutputToConsole("CPUAccessに変換できませんでした");
        return static_cast<D3D11_CPU_ACCESS_FLAG>(0);
    }
    }
}
