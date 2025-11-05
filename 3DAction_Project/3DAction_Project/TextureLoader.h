#pragma once

// ============================
// 外部ファイルから画像をロードして
// テクスチャとSRVを作成する情報を取得する
// ============================


// ============================
// ヘッダー
// ============================
#include "TextureManager.h"
#include "ResourceViewManager.h"


// ============================
// クラス
// ============================
class TextureLoader
{
private:
    // 保持するマネージャー
    TextureManager* m_TextureManager;
    ResourceViewManager* m_ViewManager;

public:
    // コンストラクタ・デストラクタ
    TextureLoader(TextureManager* texMgr, ResourceViewManager* viewMgr)
        : m_TextureManager(texMgr), m_ViewManager(viewMgr) {
    }
    ~TextureLoader() = default;

    // 外部ファイルの画像ファイルをロード
    bool ImageFileLoader(const std::string& filePath, ID3D11Device* device);
};

