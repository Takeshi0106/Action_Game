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
#include "GraphicsEnums.h"
// 文字列ヘッダー
#include "UTF8_String.h"
#include "Hashed_String.h"


// =================================
// 構造体
// =================================
// テクスチャの初期化データ構造体
struct TextureInitData {
    const void* data = nullptr;   // バッファ先頭
    size_t rowPitch = 0;          // 1行あたりのバイト数
    size_t slicePitch = 0;        // テクスチャ配列/3D用
};


// =================================
// クラス
// =================================
class TextureManager
{
private:
	// テクスチャ配列
	std::unordered_map<Hashed_String, std::unique_ptr<Texture2DData>> m_Textures;

public:
	// -------------------------------
    // コンストラクタ・デストラクタ
	// -------------------------------
    TextureManager() = default;
	~TextureManager() = default;


	// スワップチェインからテクスチャを作成
    bool CreateTextureFromSwapChain(
        const String& name,
        IDXGISwapChain* swapChain);

    // テクスチャを作成
    bool CreateTexture(
        const String& name,
        ID3D11Device* device,
        unsigned int width,
        unsigned int height,
        Format format,
        BindFlag bindFlag,
        BufferUsage usage = BufferUsage::Default,
        CPUAccess flag = CPUAccess::None,
        TextureInitData* initData = nullptr);

    // テクスチャを探して、戻り値で返す
    Texture2DData* GetFindTexture2DData(const String& name);
	// テクスチャが存在するか
	bool IsExistTexture(const String& name);

    // 後処理
    void ReleaseAllTexture();
};

