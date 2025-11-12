
// ==============================
// ヘッダー
// ==============================
// 必須ヘッダー
#include "SamplerManager.h"
// フォーマット変換
#include "DirectX_FormatConverter.h"
// ログ出力
#include "ReportMessage.h"


// ===============================
// プロトタイプ宣言
// ===============================
std::string SamplerDescToString(const SamplerDesc* _desc);


// =======================================
// サンプラー作成
// =======================================
bool SamplerManager::CreateSampler(
    SamplerDesc _desc,
    ID3D11Device* device)
{
    // すでに存在するかチェック
    if (m_Samplers.count(_desc))
    {
        // ログ出力
        WarningLog::OutputToConsole((
            std::to_string(static_cast<int>(_desc.filter)) + "\n" +
            std::to_string(static_cast<int>(_desc.addressU)) + "\n" +
            std::to_string(static_cast<int>(_desc.addressV)) + "\n" +
            std::to_string(static_cast<int>(_desc.addressW)) + "\n" +
            std::to_string(static_cast<int>(_desc.comparisonFunc)) + "\n" +
            " サンプラーが既に存在します").c_str());

        return true;
    }

    // DirectX11用に変換
    D3D11_SAMPLER_DESC directxDesc = {};
    directxDesc.Filter = DirectX_FormatConverter::ConvertFilter(_desc.filter);
    directxDesc.AddressU = DirectX_FormatConverter::ConvertAddressMode(_desc.addressU);
    directxDesc.AddressV = DirectX_FormatConverter::ConvertAddressMode(_desc.addressV);
    directxDesc.AddressW = DirectX_FormatConverter::ConvertAddressMode(_desc.addressW);
    directxDesc.ComparisonFunc = DirectX_FormatConverter::ConvertComparisonFunc(_desc.comparisonFunc);
    directxDesc.MinLOD = 0;
    directxDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // サンプラー作成
    auto samplerData = std::make_shared<SamplerData>();
    if (!samplerData->CreateSmplerData(device, directxDesc)) {
        // エラーログ出力
        ErrorLog::OutputToConsole((
            SamplerDescToString(&_desc) +
            " サンプラーの作成に失敗しました").c_str());

        return false;
    }

    // 配列に保存
    m_Samplers[_desc] = samplerData;

    return true;
}


// ==========================================
// サンプラー取得
// ==========================================
bool SamplerManager::BindSampler(const SamplerDesc& _sampler,ID3D11DeviceContext* context)
{
    // サンプラーを探す
    auto it = m_Samplers.find(_sampler);

    if (it != m_Samplers.end())
    {
        ID3D11SamplerState* sampler = it->second.get()->GetSampler();
        context->PSSetSamplers(0, 1, &sampler);

        return true;
    }


    ErrorLog::OutputToConsole((
        SamplerDescToString(&_sampler) +
        "サンプラーが見つかりませんでした").c_str());
    return false;
}


// =========================================
// サンプラー削除
// =========================================
void SamplerManager::ReleaseAllSamplers()
{
    // 削除
    m_Samplers.clear();
}


// ==================================
// 文字列に変換
// ==================================
std::string SamplerDescToString(const SamplerDesc* _desc)
{
    std::string data;

    data = (std::to_string(static_cast<int>(_desc->filter)) + "\n" +
        std::to_string(static_cast<int>(_desc->addressU)) + "\n" +
        std::to_string(static_cast<int>(_desc->addressV)) + "\n" +
        std::to_string(static_cast<int>(_desc->addressW)) + "\n" +
        std::to_string(static_cast<int>(_desc->comparisonFunc)) + "\n");

    return data;
}
