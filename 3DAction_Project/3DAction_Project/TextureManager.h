#pragma once

// ================================
// 【クラス概要】
// テクスチャマネージャーの管理、作成命令
// ================================


// ================================
// ヘッダー
// ================================
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


// ================================
// 前方宣言
// ================================
class Texture2DData;
struct ID3D11Device; // DirectXのデバイス

// =================================
// クラス
// =================================
class TextureManager
{
private:
	// テクスチャ配列
	static std::unordered_map<std::string, std::unique_ptr<Texture2DData>> m_Textures;
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
};

