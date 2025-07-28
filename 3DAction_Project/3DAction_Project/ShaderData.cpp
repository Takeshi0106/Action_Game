#include "ShaderData.h"
#include "ReportMessage.h"


// =======================================================================
// 頂点シェイダー
// =======================================================================
bool VertexShaderData::Init(ID3D11Device* device, ID3DBlob* blob)
{
    HRESULT hr = S_OK;
    
    hr = device->CreateVertexShader(
        blob,                         // バイナリデータ
        blob->GetBufferSize(),        // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_VertexShader.GetAddressOf() // 出力先
    );

    return ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, std::string(m_Name + "の頂点シェーダーの初期化に失敗").c_str());
}


// =======================================================================
// ピクセルシェイダー
// =======================================================================
bool PixelShaderData::Init(ID3D11Device* device, ID3DBlob* blob)
{
    HRESULT hr = S_OK;

    hr = device->CreatePixelShader(
        blob,                  // バイナリデータ
        blob->GetBufferSize(), // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_PixelShader.GetAddressOf()  // 出力先
    );

    return  ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, std::string(m_Name + "の頂点シェーダーの初期化に失敗").c_str());
}


// =======================================================================
// コンピュートシェイダー
// =======================================================================
bool ComputeShaderData::Init(ID3D11Device* device, ID3DBlob* blob)
{
    HRESULT hr = S_OK;

    hr = device->CreateComputeShader(
        blob,                    // バイナリデータ
        blob->GetBufferSize(),   // サイズ
        nullptr,                        // クラスリンク未使用ならnullptr
        m_ComputeShader.GetAddressOf()  // 出力先
    );

    return  ErrorLog::IsSuccessHRESULTWithOutputToConsole(hr, std::string(m_Name + "の頂点シェーダーの初期化に失敗").c_str());
}