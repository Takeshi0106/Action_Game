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
	// サンプラー配列　(サンプラーデスクのハッシュ値計算関数を渡す)
    std::unordered_map<SamplerDesc, std::shared_ptr<SamplerData>, SamplerDescHash> m_Samplers;
	// ログ出力
	AssetLogger m_Logger = { "Samplers.txt" };

public:
    // コンストラクタ・デストラクタ
    SamplerManager() = default;
    ~SamplerManager() { m_Logger.WriteLog(); }

    // サンプラー作成
    bool CreateSampler(
        SamplerDesc desc,
        ID3D11Device* device);

    // サンプラー取得
    SamplerData* GetSampler(const SamplerDesc& sampler);

    // 全削除
    void ReleaseAllSamplers();

};

