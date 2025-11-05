#pragma once

// ===============================
// 【クラス概要】
// サンプラーの作成保持、バインドなどを行います
// 
// ＊注意
// 重たいヘッダーがヘッダーにあるため
// ヘッダーにインクルード非推奨
// ===============================


// ================================
// ヘッダー
// ================================
// DirectX用
#include <d3d11.h>       // DirectXのAPI
#include <wrl/client.h>  // マイクロソフトが提供するスマートポインタ
// 名前などのデバッグ情報取得用
#include <string>        // 名前など


// ================================
// クラス
// ================================
class SamplerData
{
private:
    // サンプラー
    Microsoft::WRL::ComPtr<ID3D11SamplerState> m_Sampler;
    // デスクを保持しておく(後にデバッグなどで使用できるように保持しておく)
    D3D11_SAMPLER_DESC m_Desc;

public:
    // コンストラクタ・デストラクタ
    SamplerData() {
        m_Desc = {};
    };
    ~SamplerData() = default;

    // サンプラー作成
    bool CreateSmplerData(ID3D11Device* device, 
        const D3D11_SAMPLER_DESC& desc);

    // ゲッター
    ID3D11SamplerState* GetSampler() const { return m_Sampler.Get(); }
};

