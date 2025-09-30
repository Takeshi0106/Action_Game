#pragma once

// ================================
// 【クラス概要】
// テクスチャマネージャーの管理、作成命令
// ================================


// ================================
// ヘッダー
// ================================
// テクスチャデータヘッダー
#include "TextureData.h"
// 文字列ヘッダー
#include <string>
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索
// バッファ設定
#include "BufferSetting.h"
// アセットログ出力クラス
#include "AssetLogger.h"


// =================================
// クラス
// =================================
class TextureManager
{
private:
	// テクスチャ配列
	std::unordered_map<std::string, std::unique_ptr<Texture2DData>> m_Textures;
    AssetLogger m_Logger{"Texture.txt"};

public:
    // コンストラクタ・デストラクタ
    TextureManager() = default;
    ~TextureManager() { m_Logger.WriteLog(); }

    // テクスチャを作成
    bool CreateTexture(
        const std::string& name,
        ID3D11Device* device,
        unsigned int width,
        unsigned int height,
        Format format,
        BindFlag bindFlag,
        BufferUsage usage = BufferUsage::Default,
        CPUAccess flag = CPUAccess::None);

    // テクスチャを探して、戻り値で返す
    Texture2DData* GetFindTexture2DData(const std::string& name);
};

