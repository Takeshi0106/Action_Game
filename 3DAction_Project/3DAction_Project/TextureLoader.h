#pragma once

// ============================
// 外部ファイルから画像をロードして
// テクスチャとSRVを作成する情報を取得する
// ============================


// ============================
// ヘッダー
// ============================
// リソースマネージャー
#include "TextureManager.h"
#include "ResourceViewManager.h"
// 文字列
#include <string>


// ============================
// クラス
// ============================
class TextureLoader
{
private:
    // 保持するマネージャー
    TextureManager* m_TextureManager;
    ResourceViewManager* m_ViewManager;

    // 画像ファイルを入れておく相対パス
    const std::string m_ImageFailePath;

public:
    // コンストラクタ・デストラクタ
    TextureLoader(TextureManager* texMgr, ResourceViewManager* viewMgr, const char* failePath)
        : m_TextureManager(texMgr), m_ViewManager(viewMgr), m_ImageFailePath(failePath) {
    }
    ~TextureLoader() = default;

    // 外部ファイルの画像ファイルをロード
    bool ImageFileLoader(const std::string fileName, ID3D11Device* device);
};

