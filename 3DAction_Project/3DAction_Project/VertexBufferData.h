#pragma once

// ======================================
// 頂点バッファのデータクラス
// 
// 【注意】
// 重たいヘッダーがあるため、
// ヘッダーにインクルードするのは控えてください
// ======================================


// ======================================
// ヘッダー
// ======================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
// 名前などのデバッグ情報取得用
#include <string>        // 名前など


// ======================================
// 頂点バッファクラス
// 頂点バッファの保持と、利用の責任を持つ
// ======================================
class VertexBufferData
{
private:
    // 頂点バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer = nullptr;
    // 頂点バッファのサイズ
    size_t m_Size = 0;
    // 頂点のバイト数
    size_t m_Stride = 0;
    // 今の頂点の数
    int m_VertexCount = 0;
    // 最大頂点数
    int m_VertexMaxCount = 0;
    // 更新フラグ
    bool m_IsUpdate = false;
    // 描画タイプ
    D3D11_PRIMITIVE_TOPOLOGY m_PrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

public:
    // コンストラクタ
    VertexBufferData() {}
    // デストラクタ
    ~VertexBufferData() = default;

    // 頂点バッファ作成
    bool CreateVertexBuffer(
        ID3D11Device* device,
        const void* vertices,
        int vertexCount,
        int vertexMaxCount,
        size_t stride,
        D3D11_PRIMITIVE_TOPOLOGY primitiveType,
        D3D11_USAGE usage = D3D11_USAGE_DYNAMIC,
        D3D11_CPU_ACCESS_FLAG flag = D3D11_CPU_ACCESS_WRITE
    );

    // ゲッター
    ID3D11Buffer* GetVertexBuffer() const { return m_Buffer.Get(); }
    size_t GetSize() const { return m_Size; }
    size_t GetStride() const { return m_Stride; }
    int GetVertexCount() const { return m_VertexCount; }
    int GetVertexMaxCount() const { return m_VertexMaxCount; }
    bool GetIsUpdate() const { return m_IsUpdate; }
    D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return m_PrimitiveType; }

    // セッター
    void SetIsUpdate(bool isUpdate) { m_IsUpdate = isUpdate; }
};

