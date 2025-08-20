
// ================================================
// ヘッダー
// ================================================
// 必須ヘッダー
#include "ShaderData.h"    // 自分のヘッダー
// デバッグ用・メッセージボックス出力用
#include "ReportMessage.h" // ログ出力用


// ================================================
// プロトタイプ宣言
// ================================================
#if defined(DEBUG) || defined(_DEBUG)

// 入力レイアウトの名前を出力させる関数
void OutputILname(const std::vector<InputLayoutInfo>& ILInfo);
// 定数バッファの名前を出力させる関数
void OutputCBname(const std::vector<ConstantBufferInfo> &CBInfo);

#else
inline void OutputILname(const std::vector<InputLayoutInfo>& ILInfo) {}
inline void OutputCBname(const std::vector<ConstantBufferInfo> &CBInfo) {}

#endif


// =======================================================================
// 頂点シェイダー
// =======================================================================
bool VertexShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo)
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
        ErrorLog::OutputToConsole((m_Name + "の頂点シェーダーの初期化に失敗").c_str());
        return false;
    }

    // シェーダーが使用する情報を代入する
    CBInfo = _CBInfo;
    ILInfo = _ILInfo;

    // デバッグ用に名前を出力
    DebugLog::OutputToConsole("");
    DebugLog::OutputToConsole(m_Name.c_str());
    OutputILname(ILInfo);
    OutputCBname(CBInfo);
    DebugLog::OutputToConsole("");
    
    return true;
}


// =======================================================================
// ピクセルシェイダー
// =======================================================================
bool PixelShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo>& _CBInfo)
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

    // 定数バッファの情報を代入する
    CBInfo = _CBInfo;

    // デバッグ用に名前を出力
    DebugLog::OutputToConsole("");
    DebugLog::OutputToConsole(m_Name.c_str());
    OutputCBname(CBInfo);
    DebugLog::OutputToConsole("");

    return true;
}


// =======================================================================
// コンピュートシェイダー
// =======================================================================
bool ComputeShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo> &_CBInfo)
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

    // 定数バッファを代入
    CBInfo = _CBInfo;

    // デバッグ用に名前を出力
    DebugLog::OutputToConsole("");
    DebugLog::OutputToConsole(m_Name.c_str());
    OutputCBname(CBInfo);
    DebugLog::OutputToConsole("");

    return  true;
}


#if defined(DEBUG) || defined(_DEBUG)

// 入力レイアウトの名前を出力させる
void OutputILname(const std::vector<InputLayoutInfo>& ILInfo)
{
    DebugLog::OutputToConsole("入力レイアウト情報");

    if (ILInfo.size() == 0)
    {
        ErrorLog::OutputToConsole("入力レイアウトがありません");
    }

    for (int i = 0; i < ILInfo.size(); i++)
    {
        DebugLog::OutputToConsole((" " + ILInfo[i].semanticName).c_str());
    }
}


// 定数バッファを出力させる
void OutputCBname(const std::vector<ConstantBufferInfo> &_CBInfo)
{
    DebugLog::OutputToConsole("定数バッファ情報");

    if (_CBInfo.size() == 0)
    {
        DebugLog::OutputToConsole("定数バッファがありませんでした");
    }

    for (int i = 0; i < _CBInfo.size(); i++)
    {
        DebugLog::OutputToConsole(("  " + _CBInfo[i].name).c_str());
    }
}


#endif