// 必須ヘッダー
#include "ShaderData.h"    // 自分のヘッダー

// デバッグ用・メッセージボックス出力用
#include "ReportMessage.h" // ログ出力用


// =======================================================================
// 頂点シェイダー
// =======================================================================
bool VertexShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole(std::string(m_Name + " : への引き数がおかしいです").c_str());
        return false;
    }

    HRESULT hr = S_OK;
    
    hr = device->CreateVertexShader(
        binary,     // バイナリデータ
        size,        // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_VertexShader.GetAddressOf() // 出力先
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(std::string(m_Name + "の頂点シェーダーの初期化に失敗").c_str());
        return false;
    }

    return true;
}


// =======================================================================
// ピクセルシェイダー
// =======================================================================
bool PixelShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole(std::string(m_Name + " : への引き数がおかしいです").c_str());
        return false;
    }

    HRESULT hr = S_OK;;

    hr = device->CreatePixelShader(
        binary,  // バイナリデータ
        size,     // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_PixelShader.GetAddressOf()  // 出力先
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(std::string(m_Name + "のピクセルシェーダーの初期化に失敗").c_str());
        return false;
    }

    return true;
}


// =======================================================================
// コンピュートシェイダー
// =======================================================================
bool ComputeShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole(std::string(m_Name + " : への引き数がおかしいです").c_str());
        return false;
    }

    HRESULT hr = S_OK;

    hr = device->CreateComputeShader(
        binary,       // バイナリデータ
        size,          // サイズ
        nullptr,                        // クラスリンク未使用ならnullptr
        m_ComputeShader.GetAddressOf()  // 出力先
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(std::string(m_Name + "のコンピュートシェーダの初期化に失敗").c_str());
        return false;
    }

    return  true;
}