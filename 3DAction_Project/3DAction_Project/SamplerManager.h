#pragma once

// ==========================
// [クラス概要]
// サンプラーを保存するマネージャー
// ==========================


// ==========================
// ヘッダー
// ==========================
// リソースクラス
#include "SamplerData.h"
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索
// バッファセッティング
#include "GraphicsEnums.h"
// アセットログ出力
#include "AssetLogger.h"
// 文字列
#include <string>


// =========================
// クラス
// =========================
class SamplerManager
{
private:
	// サンプラー配列
	std::unordered_map<std::string, std::shared_ptr<SamplerData>> m_Samplers;
	// ログ出力
	AssetLogger m_Logger = { "Samplers.txt" };

public:
    // コンストラクタ・デストラクタ
    SamplerManager() = default;
    ~SamplerManager() { m_Logger.WriteLog(); }

    // サンプラー作成
    bool CreateSampler(
        std::string name,
        ID3D11Device* device,
        const D3D11_SAMPLER_DESC& desc);

    bool CreateSampler(
        std::string name,
        ID3D11Device* device,
        SamplerFilter filter,
        SamplerAddressMode addressMode,
        SamplerComparisonFunc comparisonFunc);


    // サンプラー取得
    SamplerData* GetSampler(const std::string& samplerName);

    // 全削除
    void ReleaseAllSamplers();

};

