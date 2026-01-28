
// ================================================
// ヘッダー
// ================================================
// 必須ヘッダー
#include "ShaderData.h"    // 自分のヘッダー
// デバッグ用・メッセージボックス出力用
#include "ReportMessage.h" // ログ出力用
// 文字列ヘッダー
#include "UTF8_String.h"


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
// シェーダー作成
bool VertexShaderData::CreateVertexShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole(u8"引き数がおかしいです");
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
        ErrorLog::OutputToConsole(u8"頂点シェーダーの初期化に失敗");
        return false;
    }

    // シェーダーが使用する情報を代入する
    CBInfo = _CBInfo; // 定数バッファの情報

    // 入力レイアウトを作成
    std::vector<D3D11_INPUT_ELEMENT_DESC> descArray(_ILInfo.size());

    for (int i = 0; i < _ILInfo.size(); i++)
    {
        descArray[i].SemanticName = reinterpret_cast<const char*>(_ILInfo[i].GetSemanticName().GetU8Char());
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
        ErrorLog::OutputToConsole(u8"入力レイアウトの作成に失敗しました");
        return false;
    }

    m_ILayout = std::move(layout); // VertexShaderData 内に保存
    
    return true;
}

// 頂点シェーダーをバインド
void VertexShaderData::BindVertexShader(ID3D11DeviceContext* context)
{
    // 頂点シェーダーセット
    context->VSSetShader(m_VertexShader.Get(), nullptr, 0);
    // 入力レイアウトセット
    context->IASetInputLayout(m_ILayout.Get());
}


// =======================================================================
// ピクセルシェイダー
// =======================================================================
// シェーダー作成
bool PixelShaderData::CreatePixelShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo>& _CBInfo)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole(u8"引き数がおかしいです");
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
        ErrorLog::OutputToConsole(u8"ピクセルシェーダーの初期化に失敗");
        return false;
    }

    // 定数バッファの情報を代入する
    CBInfo = _CBInfo;

    return true;
}

// ピクセルシェーダーをバインド
void PixelShaderData::BindPixelShader(ID3D11DeviceContext* context)
{
    // ピクセルシェーダーセット
    context->PSSetShader(m_PixelShader.Get(), nullptr, 0);
}


// =======================================================================
// コンピュートシェイダー
// =======================================================================
// シェーダー作成
bool ComputeShaderData::CreateComputeShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo> &_CBInfo)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole(u8"引き数がおかしいです");
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
        ErrorLog::OutputToConsole(u8"コンピュートシェーダーの初期化に失敗");
        return false;
    }

    // 定数バッファを代入
    CBInfo = _CBInfo;

    return  true;
}

// コンピュートシェーダーをバインド
void ComputeShaderData::BindComputeShader(ID3D11DeviceContext* context)
{
    // コンピュートシェーダーセット
    context->CSSetShader(m_ComputeShader.Get(), nullptr, 0);
}


#if defined(DEBUG) || defined(_DEBUG)

// 入力レイアウトの名前を出力させる
void OutputILname(const std::vector<InputLayoutInfo>& ILInfo)
{
    DebugLog::OutputToConsole(u8"入力レイアウト情報");

    if (ILInfo.size() == 0)
    {
        ErrorLog::OutputToConsole(u8"入力レイアウトがありません");
    }

    for (int i = 0; i < ILInfo.size(); i++)
    {
        DebugLog::OutputToConsole(u8" " + ILInfo[i].GetSemanticName());
    }
}


// 定数バッファを出力させる
void OutputCBname(const std::vector<ConstantBufferInfo> &_CBInfo)
{
    DebugLog::OutputToConsole(u8"定数バッファ情報");

    if (_CBInfo.size() == 0)
    {
        DebugLog::OutputToConsole(u8"定数バッファがありませんでした");
    }

    for (int i = 0; i < _CBInfo.size(); i++)
    {
        DebugLog::OutputToConsole(u8"  " + _CBInfo[i].GetName());
    }
}


#endif
