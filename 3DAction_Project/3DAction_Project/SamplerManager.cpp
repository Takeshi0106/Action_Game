
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

    // 作成ログ出力
    DebugLog::OutputToConsole((
        SamplerDescToString(&_desc) +
        "サンプラーを作成しました").c_str());

    return true;
}


// ==========================================
// サンプラー取得
// ==========================================
SamplerData* SamplerManager::GetSampler(const SamplerDesc& _sampler)
{
    auto it = m_Samplers.find(_sampler);
    if (it != m_Samplers.end())
    {
        return it->second.get();
    }

    ErrorLog::OutputToConsole((
        SamplerDescToString(&_sampler) +
        "サンプラーが見つかりませんでした").c_str());

    return nullptr;
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