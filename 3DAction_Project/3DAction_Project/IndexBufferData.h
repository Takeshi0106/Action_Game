#pragma once

// ==================================
// 【クラス概要】
// インデックスバッファデータ
// 
// 【注意】
// 頂点が増えることは想定していません
// １次元でインデックスを渡すことを想定しています。
// ３つの頂点インデックスで１つです。
// インデックスの数は配列の要素数を入れてください
// ==================================


// ==================================
// ヘッダー
// ==================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
// 標準ヘッダー
#include <cstdint>


// ==================================
// クラス
// ==================================
class IndexBufferData
{
private:
    // バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer = nullptr;
	// インデックス数
	uint32_t m_IndexCount = 0;

public:
    // コンストラクタ・デストラクタ
    IndexBufferData() {}
    ~IndexBufferData() = default;

    // バッファ作成
    bool CreateIndexBuffer(
        ID3D11Device* device,
        const uint32_t* indexData,
        uint32_t indexCount);

	// バッファゲッター
	ID3D11Buffer* GetBuffer() const { return m_Buffer.Get(); }
	// インデックス数ゲッター
	uint32_t GetIndexCount() const { return m_IndexCount; }
};
