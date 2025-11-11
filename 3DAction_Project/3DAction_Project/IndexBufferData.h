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
// 名前などのデバッグ情報取得用
#include <string>        // 名前など


// ==================================
// クラス
// ==================================
class IndexBufferData
{
private:
    // バッファ
    Microsoft::WRL::ComPtr<ID3D11Buffer> m_Buffer = nullptr;

public:
    // コンストラクタ・デストラクタ
    IndexBufferData() {}
    ~IndexBufferData() = default;

    // バッファ作成
    bool CreateIndexBuffer(
        ID3D11Device* device,
        const int* indexData,
        int indexCount);
};
