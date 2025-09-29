
// ================================================
// ヘッダー
// ================================================
// 必須ヘッダー
#include "ShaderData.h"    // 自分のヘッダー
// デバッグ用・メッセージボックス出力用
#include "ReportMessage.h" // ログ出力用
// DirectXヘッダー
#include <d3d11.h>
// マイクロソフトが提供するスマートポインタ
#include <wrl/client.h> 


// ================================================
// PImplイディオム構造体定義
// ================================================
struct VertexDataImpl{
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader; // 頂点シェーダー
    Microsoft::WRL::ComPtr<ID3D11InputLayout> iLayout; // シェーダーの入力レイアウト情報を入れる
};

struct PixelDataImpl {
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader; // ピクセルシェーダー
};

struct ComputeDataImpl {
    Microsoft::WRL::ComPtr<ID3D11ComputeShader> computeShader; // コンピュートシェーダー
};


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
// コンストラクタ・デストラクタ
VertexShaderData::VertexShaderData() : 
    m_VertexData(std::make_unique<VertexDataImpl>()) {}

VertexShaderData::~VertexShaderData() = default;

// シェーダー作成
bool VertexShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo>& _CBInfo, const std::vector<InputLayoutInfo>& _ILInfo)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole("引き数がおかしいです");
        return false;
    }

    HRESULT hr = S_OK;
    
    hr = device->CreateVertexShader(
        binary,     // バイナリデータ
        size,        // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_VertexData->vertexShader.GetAddressOf() // 出力先
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole("頂点シェーダーの初期化に失敗");
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
        ErrorLog::OutputToConsole("入力レイアウトの作成に失敗しました");
        return false;
    }

    m_VertexData->iLayout = std::move(layout); // VertexShaderData 内に保存
    
    return true;
}

// 頂点シェーダーをバインド
void VertexShaderData::VertexShaderBind(ID3D11DeviceContext* context)
{
    // 頂点シェーダーをセット
    context->VSSetShader(m_VertexData->vertexShader.Get(), nullptr, 0);

    // シェーダーに紐づく入力レイアウトをセット
    context->IASetInputLayout(m_VertexData->iLayout.Get());
}


// =======================================================================
// ピクセルシェイダー
// =======================================================================
// コンストラクタ・デストラクタ
PixelShaderData::PixelShaderData() :
    m_PixelData(std::make_unique<PixelDataImpl>()) {}

PixelShaderData::~PixelShaderData() = default;

// シェーダー作成
bool PixelShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo>& _CBInfo)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole("引き数がおかしいです");
        return false;
    }

    HRESULT hr = S_OK;;

    hr = device->CreatePixelShader(
        binary,  // バイナリデータ
        size,     // サイズ
        nullptr,                      // クラスリンク未使用ならnullptr
        m_PixelData->pixelShader.GetAddressOf()  // 出力先
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole("ピクセルシェーダーの初期化に失敗");
        return false;
    }

    // 定数バッファの情報を代入する
    CBInfo = _CBInfo;

    return true;
}


// ピクセルシェーダーをバインド
void PixelShaderData::PixelShaderBind(ID3D11DeviceContext* context)
{
    context->PSSetShader(m_PixelData->pixelShader.Get(), nullptr, 0);
}


// =======================================================================
// コンピュートシェイダー
// =======================================================================
// コンストラクタ・デストラクタ
ComputeShaderData::ComputeShaderData() :
    m_ComputeData(std::make_unique<ComputeDataImpl>()) {}

ComputeShaderData::~ComputeShaderData() = default;

// シェーダー作成
bool ComputeShaderData::CreateShader(ID3D11Device* device, void* binary, size_t size,
    const std::vector<ConstantBufferInfo> &_CBInfo)
{
    if (!device || !binary || size == 0) {
        ErrorLog::OutputToConsole("引き数がおかしいです");
        return false;
    }

    HRESULT hr = S_OK;

    hr = device->CreateComputeShader(
        binary,       // バイナリデータ
        size,          // サイズ
        nullptr,                        // クラスリンク未使用ならnullptr
        m_ComputeData->computeShader.GetAddressOf()  // 出力先
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole("コンピュートシェーダーの初期化に失敗");
        return false;
    }

    // 定数バッファを代入
    CBInfo = _CBInfo;

    return  true;
}


// コンピュートシェーダーをバインド
void ComputeShaderData::ComputeShaderBind(ID3D11DeviceContext* context)
{
    context->CSSetShader(m_ComputeData->computeShader.Get(), nullptr, 0);
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