
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
#include "1_Platform/Windows/1_DirectX11/DirectX11_FormatConverter.h"


// ============================
// 画像ファイルをロードする
// ============================
bool TextureLoader::ImageFileLoader(
    const Hashed_String& fileName, 
    ID3D11Device* device)
{
    // ハッシュ
	Hashed_String hashedName = fileName;
    // ファイルパス作成
    std::filesystem::path filePath;

    // パス確認
    if (std::filesystem::path(fileName.GetString().GetU8String()).parent_path().empty())
    {
        // 相対パスを追加
        filePath = m_ImageFailePath.GetU8String();
        filePath /= fileName.GetString().GetU8String();
    }
    else
    {
        // そのまま使用
        filePath = fileName.GetString().GetU8String();
		hashedName = Hashed_String(filePath.filename().u8string());
    }

    // 区切り文字を統一する
    filePath.make_preferred();

	// すでに登録されているか確認
    if (m_TextureManager->IsExistTexture(hashedName)) {
        DebugLog::OutputToConsole(hashedName.GetString() + u8" はすでに登録されています。");
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
        hashedName,
        device,
        (unsigned int)(meta.width),
        (unsigned int)(meta.height),
        DirectX11_FormatConverter::ToSelfFormat(meta.format),
        BindFlag::Bind_ShaderResource,
        BufferUsage::Default,
        CPUAccess::None,
        &initData))
    {
        ErrorLog::OutputToConsole(hashedName.GetString() + u8" のテクスチャの作成に失敗しました。");
        return false;
    }

    // テクスチャを取得
    Texture2DData* data = m_TextureManager->GetFindTexture2DData(hashedName);

    // SRVを作成して ResourceViewManager に登録
    if (!m_ViewManager->CreateSRV(
        hashedName,
        device,
        data->GetTexture(),
        DirectX11_FormatConverter::ToSelfFormat(meta.format)))
    {
        ErrorLog::OutputToConsole(hashedName.GetString() + u8" のSRVの作成に失敗しました。");
        return false;
    }

    // テクスチャ作成ログ出力
    DebugLog::OutputToConsole(hashedName.GetString() + u8" のロードに成功しました。");
    return true;
}
