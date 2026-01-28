
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
#include "DirectX11/DirectX11_FormatConverter.h"


// ============================
// 画像ファイルをロードする
// ============================
bool TextureLoader::ImageFileLoader(
    const String& fileName, 
    ID3D11Device* device)
{
    // ファイルパス作成
    std::filesystem::path filePath;

    // パス確認
    if (std::filesystem::path(fileName.GetU8String()).parent_path().empty())
    {
        // 相対パスを追加
        filePath = m_ImageFailePath.GetU8String();
        filePath /= fileName.GetU8String();
    }
    else
    {
        // そのまま使用
        filePath = fileName.GetU8String();
    }

    // 区切り文字を統一する
    filePath.make_preferred();

    // 登録名
    String keyName = filePath.filename().u8string();

	// すでに登録されているか確認
    if (m_TextureManager->IsExistTexture(keyName)) {
        DebugLog::OutputToConsole(keyName + u8" はすでに登録されています。");
		return true;
    }

    // 画像のロード
    DirectX::ScratchImage image;
    HRESULT hr = DirectX::LoadFromWICFile(
        filePath.wstring().c_str(),
        DirectX::WIC_FLAGS_NONE,
        nullptr,
        image
    );

    if (FAILED(hr)) {
        ErrorLog::OutputToConsole(u8"画像のロードに失敗しました" + String::to_u8string((uint64_t)hr));
        return false;
    }

    // 画像メタデータを取得する
    const DirectX::TexMetadata& meta = image.GetMetadata();

    // イメージデータ取得（mipmap0, arraySlice0）
    const DirectX::Image* img = image.GetImage(0, 0, 0);
    if (!img) {
        ErrorLog::OutputToConsole(u8"画像データの取得に失敗しました");
        return false;
    }

    // 初期化データを作成
    TextureInitData initData = {};
    initData.data = img->pixels;
    initData.rowPitch = img->rowPitch;
    initData.slicePitch = img->slicePitch;

    // TextureManagerに登録
    if (!m_TextureManager->CreateTexture(
        keyName,
        device,
        (unsigned int)(meta.width),
        (unsigned int)(meta.height),
        DirectX11_FormatConverter::ToSelfFormat(meta.format),
        BindFlag::Bind_ShaderResource,
        BufferUsage::Default,
        CPUAccess::None,
        &initData))
    {
        ErrorLog::OutputToConsole(keyName + u8" のテクスチャの作成に失敗しました。");
        return false;
    }

    // テクスチャを取得
    Texture2DData* data = m_TextureManager->GetFindTexture2DData(keyName);

    // SRVを作成して ResourceViewManager に登録
    if (!m_ViewManager->CreateSRV(
        keyName,
        device,
        data->GetTexture(),
        DirectX11_FormatConverter::ToSelfFormat(meta.format)))
    {
        ErrorLog::OutputToConsole(keyName + u8" のSRVの作成に失敗しました。");
        return false;
    }

    // テクスチャ作成ログ出力
    DebugLog::OutputToConsole(keyName + u8" のロードに成功しました。");
    return true;
}
