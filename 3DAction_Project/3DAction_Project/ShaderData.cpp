#include "ShaderData.h"
#include <d3dcompiler.h>  // シェーダーをコンパイルするヘッダー

#if defined(DEBUG) || defined(_DEBUG)
#include <iostream> // 文字出力
#endif


struct Blob{
private: // 直接アクセス不可
    ID3DBlob* blob; // アプリケーションハンドル

public:
    Blob(ID3DBlob* pBlob) : blob(pBlob) {}
    ~Blob() {
            blob->Release();
            blob = nullptr;
    }
    ID3DBlob* Get() { return blob; }
};


// =======================================================================
// 頂点シェイダー
// =======================================================================
VertexShaderData::VertexShaderData(std::string name, std::string entry, std::string type, ID3D11Device* device, Blob* blob)
    :BaseShaderData(name, entry, type) 
{
    HRESULT hr = S_OK;
    
    hr = device->CreateVertexShader(
        blob->Get(),                  // バイナリデータ
        blob->Get()->GetBufferSize(), // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_VertexShader.GetAddressOf() // 出力先
    );

    if (FAILED(hr)) {
#if defined(DEBUG) || defined(_DEBUG)
        std::cout << std::to_string(hr) << std::endl; // hr出力　失敗情報が取得
#endif
        std::string msg = name + " の頂点シェーダーの初期化に失敗";
        MessageBoxA(NULL, msg.c_str(), "エラー", MB_OK | MB_ICONERROR);
    }
}


// =======================================================================
// ピクセルシェイダー
// =======================================================================
PixelShaderData::PixelShaderData(std::string name, std::string entry, std::string type, ID3D11Device* device, Blob* blob)
    :BaseShaderData(name, entry, type) 
{
    HRESULT hr = S_OK;

    hr = device->CreatePixelShader(
        blob->Get(),                  // バイナリデータ
        blob->Get()->GetBufferSize(), // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_PixelShader.GetAddressOf()  // 出力先
    );

    if (FAILED(hr)) {
#if defined(DEBUG) || defined(_DEBUG)
        std::cout << std::to_string(hr) << std::endl; // hr出力　失敗情報が取得
#endif
        std::string msg = name + " のピクセルシェーダーの初期化に失敗";
        MessageBoxA(NULL, msg.c_str(), "エラー", MB_OK | MB_ICONERROR);
    }
}


// =======================================================================
// コンピュートシェイダー
// =======================================================================
ComputeShaderData::ComputeShaderData(std::string name, std::string entry, std::string type, ID3D11Device* device, Blob* blob)
    :BaseShaderData(name, entry, type)
{
    HRESULT hr = S_OK;

    hr = device->CreateComputeShader(
        blob->Get(),                    // バイナリデータ
        blob->Get()->GetBufferSize(),   // サイズ
        nullptr,                        // クラスリンク未使用ならnullptr
        m_ComputeShader.GetAddressOf()  // 出力先
    );

    if (FAILED(hr)) {
#if defined(DEBUG) || defined(_DEBUG)
        std::cout << std::to_string(hr) << std::endl; // hr出力　失敗情報が取得
#endif
        std::string msg = name + " のコンピュートシェーダーの初期化に失敗";
        MessageBoxA(NULL, msg.c_str(), "エラー", MB_OK | MB_ICONERROR);
    }
}