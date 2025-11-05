
// =================================
// ヘッダー
// =================================
// 必須ヘッダー
#include "SamplerData.h"
// ログ出力関数
#include "ReportMessage.h"


// ==================================
// サンプラー作成
// ==================================
bool SamplerData::CreateSmplerData(ID3D11Device* device,
	const D3D11_SAMPLER_DESC& desc)
{
    // サンプラーを作成
    Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler;
    HRESULT hr = device->CreateSamplerState(&desc, sampler.GetAddressOf());

    // エラーチェック
    if (FAILED(hr))
    {
        ErrorLog::OutputToConsole("サンプラーの作成に失敗しました");
        return false;
    }

    // メンバーに保存
    m_Sampler = std::move(sampler);
    m_Desc = desc;

    return true;
}