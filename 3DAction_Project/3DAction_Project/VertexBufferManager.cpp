
// ======================================
// ヘッダー
// ======================================
// 必須ヘッダー
#include "VertexBufferManager.h"
// 自作列挙型をDirectX用に変換
#include "Windows/DirectX11/DirectX11_FormatConverter.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "dxguid.lib")
#include "BinaryView.h"
#endif


// ======================================
// プロトタイプ宣言
// ======================================
    // 自作列挙型をDirectXに変換数関数
inline D3D11_PRIMITIVE_TOPOLOGY ToDXPrimitive(PrimitiveType type);


// =======================================
// 頂点バッファ作成
// =======================================
// 頂点バッファ作成
bool VertexBufferManager::CreateVertexBuffer(
    const Hashed_String& name,
    ID3D11Device* device,
    const void* vertices,
    int vertexCount,
    int vertexMaxCount,
    size_t stride,
    PrimitiveType type,
    BufferUsage usage,
    CPUAccess access)
{
    // 既に作成済み
    if (Exists(name)) { 
        WarningLog::OutputToConsole(name.GetString() + 
            u8" 頂点バッファが既に作成されていました");
        return true; 
    }

    auto vbd = std::make_unique<VertexBufferData>();

    // 定数バッファ作成呼び出し
    if (!vbd->CreateVertexBuffer(
        device,
        vertices,
        vertexCount,
        vertexMaxCount,
        stride,
        ToDXPrimitive(type),
        DirectX11_FormatConverter::ToDXUsage(usage),
        D3D11_CPU_ACCESS_FLAG(DirectX11_FormatConverter::ToDXCPUAccess(access))))
    {
        ErrorLog::OutputToConsole(u8"頂点バッファの作成に失敗");
        return false;
    }

    // 定数バッファデータを配列に代入
    m_VertexBuffers[name] = std::move(vbd);

    // 作製した頂点バッファの名前を保存
    m_Logger.Log(name.GetString());

#if defined(DEBUG) || defined(_DEBUG)
    DebugLog::OutputToConsole(u8"頂点バッファ " + name.GetString() + u8" を作成しました");

	// 名前を設定
    m_VertexBuffers[(Hashed_String)name]->GetVertexBuffer()->SetPrivateData(
        WKPDID_D3DDebugObjectName,
        UINT(name.GetString().GetBinaryView().GetSize()),
        name.GetString().GetBinaryView().GetData());
#endif

    return true;
}


// =======================================
// 頂点バッファ更新
// =======================================
bool VertexBufferManager::UpdateVertexBuffer(
    const Hashed_String& name, 
    ID3D11DeviceContext* context,
    const void* data, 
    int size)
{
    // 探す
    auto it = m_VertexBuffers.find(name);

    if (it != m_VertexBuffers.end())
    {
        // 更新
        it->second.get()->UpdateBuffer(context, data, size);
        return true;
    }

    ErrorLog::OutputToConsole(u8"頂点バッファ" + name.GetString() + u8" が見つかりませんでした");
    return false;
}


// =======================================
// 頂点バッファをバインド
// =======================================
int VertexBufferManager::BindVertexBuffer(
    const Hashed_String& name, 
    ID3D11DeviceContext* context) const
{
    // 探す
    auto it = m_VertexBuffers.find(name);

    if (it != m_VertexBuffers.end())
    {
        // 入力アセンブラ
        ID3D11Buffer* vbuffers = it->second.get()->GetVertexBuffer();
        UINT stride = UINT(it->second.get()->GetStride());
        UINT offset = 0;

        // 頂点バッファをセット
        context->IASetVertexBuffers(0, 1, &vbuffers, &stride, &offset);
        it->second.get()->SetIsUpdate(false);

        // トポロギー設定
       context->IASetPrimitiveTopology(it->second.get()->GetPrimitiveType());

        // 頂点データを返す
       return it->second.get()->GetVertexCount();
    }

    return -1;
}


// ========================================
// 頂点バッファがあるかのチェック
// ========================================
bool VertexBufferManager::Exists(const Hashed_String& name) const
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


// ========================================
// トポロギーに変換
// ========================================
inline D3D11_PRIMITIVE_TOPOLOGY ToDXPrimitive(PrimitiveType type)
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
        ErrorLog::OutputToConsole(u8"トポロギーに変換できませんでした");
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
    }
}
