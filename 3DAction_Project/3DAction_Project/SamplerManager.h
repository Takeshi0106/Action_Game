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
// サンプラー
#include "SamplerSetting.h"


// =========================
// クラス
// =========================
class SamplerManager
{
private:
	// サンプラー配列　(サンプラーデスクのハッシュ値計算関数を渡す)
    std::unordered_map<SamplerDesc, std::shared_ptr<SamplerData>, SamplerDescHash> m_Samplers;

public:
    // コンストラクタ・デストラクタ
    SamplerManager() = default;
    ~SamplerManager() = default;

    // サンプラー作成
    bool CreateSampler(
        SamplerDesc desc,
        ID3D11Device* device);

    // サンプラー取得
    bool BindSampler(const SamplerDesc& sampler, ID3D11DeviceContext* context);

    // 全削除
    void ReleaseAllSamplers();

};

