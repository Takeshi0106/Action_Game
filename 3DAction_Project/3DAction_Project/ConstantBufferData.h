#pragma once

//  =================================
// 【クラス概要】
// 定数バッファの作成、保持を行うクラス
// 
// *注意
// 重たいヘッダーを含めているため
// ヘッダーにインクルードするのを非推奨
// ==================================


// ==================================
// ヘッダー
// ==================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
// 名前などのデバッグ情報取得用
#include <string>        // 名前など


// ===================================
// クラス
// ===================================
class ConstantBufferData
{
private:
    // バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer = nullptr;
    // 頂点バッファのサイズ
    size_t m_Size = 0;
    // 更新フラグ
    bool m_IsUpdate = false;

public:
    // コンストラクタ・デストラクタ
    ConstantBufferData() {}
    ~ConstantBufferData() = default;

    // 定数バッファ作衛
    bool CreateConstantBuffer(
        ID3D11Device* device,
        const void* data,
        size_t size,
        D3D11_USAGE usage = D3D11_USAGE_DYNAMIC,
        D3D11_CPU_ACCESS_FLAG cpuAccess = D3D11_CPU_ACCESS_WRITE);

    // ゲッター
    ID3D11Buffer* GetBuffer() const { return m_Buffer.Get(); }
    size_t GetSize() const { return m_Size; }
    bool IsUpdate() const { return m_IsUpdate; }
    
    // セッター
    void SetUpdate(bool flag) { m_IsUpdate = flag; }
};