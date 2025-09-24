#pragma once

// ================================
// 【クラス概要】
// テクスチャマネージャーの管理、作成命令
// ================================


// ================================
// ヘッダー
// ================================
// 基底クラスヘッダー
#include "BaseDirectXManager.h"
// 文字列ヘッダー
#include <string>
// スマートポインターヘッダー
#include <memory> // スマートポインター
// 配列のヘッダー
#include <unordered_map> // ハッシュ値検索
// バッファ設定
#include "BufferSetting.h"

// ================================
// 前方宣言
// ================================
class Texture2DData;
struct ID3D11Device; // DirectXのデバイス

// =================================
// クラス
// =================================
class TextureManager : public BaseDirectXManager
{
private:
	// テクスチャ配列
	static std::unordered_map<std::string, std::unique_ptr<Texture2DData>> m_Textures;

public:
    TextureManager(const char* assetLog) : BaseDirectXManager(assetLog) {
    }
    ~TextureManager() = default;

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

