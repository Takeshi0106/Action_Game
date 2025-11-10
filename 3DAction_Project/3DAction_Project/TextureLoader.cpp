
// ============================
// ヘッダー
// ============================
#include "TextureLoader.h"
// ファイルパスを取得
#include <filesystem>
// 画像読込みオープンソース(MITライセンス)
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")
// ログ表示ヘッダー
#include "ReportMessage.h"
// 自作列挙型をDirectXように変換する関数
#include "DirectX_FormatConverter.h"


// ============================
// 画像ファイルをロードする
// ============================
bool TextureLoader::ImageFileLoader(const std::string faileName, ID3D11Device* device)
{
    // UTF-16 に変換
    std::filesystem::path filePath = m_ImageFailePath;
    filePath /= faileName;
    
    // 区切り文字を統一する
    filePath.make_preferred();

    // 画像のロード
    DirectX::ScratchImage image;
    HRESULT hr = DirectX::LoadFromWICFile(
        filePath.wstring().c_str(),
        DirectX::WIC_FLAGS_FORCE_RGB | DirectX::WIC_FLAGS_IGNORE_SRGB,
        nullptr,
        image
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(("画像のロードに失敗しました" + std::to_string(hr)).c_str());
        return false;
    }

    // 画像メタデータを取得する
    const DirectX::TexMetadata& meta = image.GetMetadata();

    // イメージデータ取得（mipmap0, arraySlice0）
    const DirectX::Image* img = image.GetImage(0, 0, 0);
    if (!img) {
        ErrorLog::OutputToConsole("画像データの取得に失敗しました");
        return false;
    }

    // 初期化データを作成
    TextureInitData initData = {};
    initData.data = img->pixels;
    initData.rowPitch = img->rowPitch;
    initData.slicePitch = img->slicePitch;

    // TextureManagerに登録
    if (!m_TextureManager->CreateTexture(
        filePath.filename().string(),
        device,
        (unsigned int)(meta.width),
        (unsigned int)(meta.height),
        DirectX_FormatConverter::ToSelfFormat(meta.format),
        BindFlag::ShaderResource,
        BufferUsage::Default,
        CPUAccess::None,
        &initData))
    {
        ErrorLog::OutputToConsole((filePath.filename().string() + " のテクスチャの作成に失敗しました。").c_str());
        return false;
    }

    // テクスチャを取得
    Texture2DData* data = m_TextureManager->GetFindTexture2DData(filePath.filename().string());

    // SRVを作成して ResourceViewManager に登録
    if (!m_ViewManager->CreateSRV(
        filePath.filename().string(),
        device,
        data->GetTexture(),
        DirectX_FormatConverter::ToSelfFormat(meta.format)))
    {
        ErrorLog::OutputToConsole((filePath.filename().string() + " のSRVの作成に失敗しました。").c_str());
        return false;
    }

    // テクスチャ作成ログ出力
    DebugLog::OutputToConsole((filePath.filename().string() + " のロードに成功しました。").c_str());
    return true;
}
