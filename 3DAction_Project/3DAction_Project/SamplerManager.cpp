
// ==============================
// ヘッダー
// ==============================
// 必須ヘッダー
#include "SamplerManager.h"
// フォーマット変換
#include "DirectX_FormatConverter.h"
// ログ出力
#include "ReportMessage.h"


// サンプラー作成
bool SamplerManager::CreateSampler(
    std::string name,
    ID3D11Device* device,
    const D3D11_SAMPLER_DESC& desc)
{
    // すでに存在するかチェック
    if (m_Samplers.count(name))
    {
        WarningLog::OutputToConsole((name + " サンプラーが既に存在します").c_str());
        return true;
    }

    // サンプラー作成
    auto samplerData = std::make_shared<SamplerData>();
    if (!samplerData->CreateSmplerData(device, desc))
    {
        ErrorLog::OutputToConsole((name + " サンプラーの作成に失敗しました").c_str());
        return false;
    }

    // 保存
    m_Samplers[name] = samplerData;
    m_Logger.Log(name.c_str());

    return true;
}

// サンプラー作成
bool SamplerManager::CreateSampler(
    std::string name,
    ID3D11Device* device,
    SamplerFilter filter,
    SamplerAddressMode addressMode,
    SamplerComparisonFunc comparisonFunc)
{
    D3D11_SAMPLER_DESC desc = {};
    desc.Filter = DirectX_FormatConverter::ConvertFilter(filter);
    desc.AddressU = DirectX_FormatConverter::ConvertAddressMode(addressMode);
    desc.AddressV = DirectX_FormatConverter::ConvertAddressMode(addressMode);
    desc.AddressW = DirectX_FormatConverter::ConvertAddressMode(addressMode);
    desc.ComparisonFunc = DirectX_FormatConverter::ConvertComparisonFunc(comparisonFunc);
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    return CreateSampler(name, device, desc);
}


// サンプラー取得
SamplerData* SamplerManager::GetSampler(const std::string& samplerName)
{
    auto it = m_Samplers.find(samplerName);
    if (it != m_Samplers.end())
    {
        return it->second.get();
    }

    ErrorLog::OutputToConsole(("サンプラーが見つかりません: " + samplerName).c_str());
    return nullptr;
}