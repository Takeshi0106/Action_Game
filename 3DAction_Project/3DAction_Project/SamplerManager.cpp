
// ==============================
// ヘッダー
// ==============================
// 必須ヘッダー
#include "SamplerManager.h"
// フォーマット変換
#include "Windows/1_DirectX11/DirectX11_FormatConverter.h"
// ログ出力
#include "ReportMessage.h"
// 文字列
#include "UTF8_String.h"


// ===============================
// プロトタイプ宣言
// ===============================
String SamplerDescToString(const SamplerDesc* _desc);


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
        WarningLog::OutputToConsole(
            String::to_u8string(static_cast<int>(_desc.filter)) + u8"\n" +
            String::to_u8string(static_cast<int>(_desc.addressU)) + u8"\n" +
            String::to_u8string(static_cast<int>(_desc.addressV)) + u8"\n" +
            String::to_u8string(static_cast<int>(_desc.addressW)) + u8"\n" +
            String::to_u8string(static_cast<int>(_desc.comparisonFunc)) + u8"\n" +
            u8" サンプラーが既に存在します");

        return true;
    }

    // DirectX11用に変換
    D3D11_SAMPLER_DESC directxDesc = {};
    directxDesc.Filter = DirectX11_FormatConverter::ConvertFilter(_desc.filter);
    directxDesc.AddressU = DirectX11_FormatConverter::ConvertAddressMode(_desc.addressU);
    directxDesc.AddressV = DirectX11_FormatConverter::ConvertAddressMode(_desc.addressV);
    directxDesc.AddressW = DirectX11_FormatConverter::ConvertAddressMode(_desc.addressW);
    directxDesc.ComparisonFunc = DirectX11_FormatConverter::ConvertComparisonFunc(_desc.comparisonFunc);
    directxDesc.MinLOD = 0;
    directxDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // サンプラー作成
    auto samplerData = std::make_shared<SamplerData>();
    if (!samplerData->CreateSmplerData(device, directxDesc)) {
        // エラーログ出力
        ErrorLog::OutputToConsole(
            SamplerDescToString(&_desc) +
            u8" サンプラーの作成に失敗しました");

        return false;
    }

    // 配列に保存
    m_Samplers[_desc] = samplerData;

    return true;
}


// ==========================================
// サンプラー取得
// ==========================================
bool SamplerManager::BindSampler(const SamplerDesc& _sampler,ID3D11DeviceContext* _context)
{
    // サンプラーを探す
    auto it = m_Samplers.find(_sampler);

    if (it != m_Samplers.end())
    {
        // バインド
        it->second.get()->BindSampler(_context);

        return true;
    }


    ErrorLog::OutputToConsole(
        SamplerDescToString(&_sampler) +
        u8"サンプラーが見つかりませんでした");
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
String SamplerDescToString(const SamplerDesc* _desc)
{
    String data;

    data = (
        String::to_u8string(static_cast<int>(_desc->filter)) + u8"\n" +
        String::to_u8string(static_cast<int>(_desc->addressU)) + u8"\n" +
        String::to_u8string(static_cast<int>(_desc->addressV)) + u8"\n" +
        String::to_u8string(static_cast<int>(_desc->addressW)) + u8"\n" +
        String::to_u8string(static_cast<int>(_desc->comparisonFunc)) + u8"\n");

    return data;
}
