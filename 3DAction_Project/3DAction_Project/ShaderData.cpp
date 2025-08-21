
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
    CBInfo = _CBInfo; // 定数バッファの情報

    // 入力レイアウトを作成
    std::vector<D3D11_INPUT_ELEMENT_DESC> descArray(_ILInfo.size());

    for (int i = 0; i < _ILInfo.size(); i++)
    {
        descArray[i].SemanticName = _ILInfo[i].GetSemanticName().c_str();
        descArray[i].SemanticIndex = _ILInfo[i].GetSemanticIndex();
        descArray[i].InputSlot = _ILInfo[i].GetInputSlot();
        descArray[i].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT; // 自動オフセット
        descArray[i].Format = static_cast<DXGI_FORMAT>(_ILInfo[i].GetFormat());
        descArray[i].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;  // 頂点単位
        descArray[i].InstanceDataStepRate = 0;
    }

    // 入力レイアウト作成
    Microsoft::WRL::ComPtr<ID3D11InputLayout> layout;
    HRESULT hrLayout = device->CreateInputLayout(
        descArray.data(),
        static_cast<UINT>(descArray.size()),
        binary,
        size,
        layout.GetAddressOf()
    );

    if (FAILED(hrLayout)) {
        ErrorLog::OutputToConsole((m_Name + " : 入力レイアウトの作成に失敗しました").c_str());
        return false;
    }

    m_ILayout = layout; // VertexShaderData 内に保


    // デバッグ用に名前を出力
    DebugLog::OutputToConsole("");
    DebugLog::OutputToConsole(m_Name.c_str());
    OutputILname(_ILInfo);
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
        DebugLog::OutputToConsole((" " + ILInfo[i].GetSemanticName()).c_str());
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
        DebugLog::OutputToConsole(("  " + _CBInfo[i].GetName()).c_str());
    }
}


#endif