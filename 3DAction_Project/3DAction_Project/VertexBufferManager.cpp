﻿
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
// 自作列挙型をDirectX用に変換
#include "DirectX_BufferUtils.h"
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ======================================
// 静的メンバー配列
// ======================================
std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> VertexBufferManager::m_VertexBuffers;


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
    const std::string& name,
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
        DebugLog::OutputToConsole((name + " 頂点バッファが既に作成されていました").c_str());
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
        BufferUtils::ToDXUsage(usage),
        D3D11_CPU_ACCESS_FLAG(BufferUtils::ToDXCPUAccess(access))))
    {
        ErrorLog::OutputToConsole("頂点バッファの作成に失敗");
        return false;
    }

    // 定数バッファデータを配列に代入
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
        ErrorLog::OutputToConsole("トポロギーに変換できませんでした");
        return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
    }
    }
}