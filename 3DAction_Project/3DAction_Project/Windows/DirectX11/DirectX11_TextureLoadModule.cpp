
// =======================================
// ヘッダー
// =======================================
// 必須ヘッダー
#include "DirectX11_TextureLoadModule.h"
// ファイルパスを取得
#include <filesystem>
// 画像読込みオープンソース(MITライセンス)
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")
// ログ表示ヘッダー
#include "../../ReportMessage.h"


// =======================================
// プロトタイプ宣言
// =======================================
const TextureHandle LoadTextureFromFile(
	ID3D11Device* _device,
	const std::filesystem::path& _texturePath,
	uint16_t _mipLevels,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager,
	const Hashed_String& _keyName);


// =======================================
// 画像ファイルをロードする
// テクスチャフォルダーからロードする
// =======================================
const TextureHandle DirectX11_TextureLoadModule::LoadFaileTexture_TextureFolder(
	ID3D11Device* _device,
	const Hashed_String& _textureName,
	const String& _textureFolderName,
	uint16_t _mipLevels,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager)
{
	// ファイルパス作成
	std::filesystem::path filePath;
	filePath = _textureFolderName.GetU8String();

	// パス確認
	if (!filePath.empty())
	{
		// 相対パスを追加
		filePath = kPath.GetU8String();
		filePath /= _textureFolderName.GetU8String();
		filePath /= _textureName.GetString().GetU8String();
	}
	else
	{
		// そのまま使用
		filePath = kPath.GetU8String();
		filePath /= _textureName.GetString().GetU8String();
	}

	// 区切り文字を統一する
	filePath.make_preferred();

	// 画像のロード
	return LoadTextureFromFile(
		_device,
		filePath,
		_mipLevels,
		_textureManager,
		_viewManager,
		_textureName);
}


// =======================================
// 画像ファイルをロードする
// =======================================
const TextureHandle DirectX11_TextureLoadModule::LoadFaileTexture(
	ID3D11Device* _device,
	const Hashed_String& _textureName,
	const String& _texturePath,
	uint16_t _mipLevels,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager)
{
	std::filesystem::path filePath = _texturePath.GetU8String();

	// 画像のロード
	return LoadTextureFromFile(
		_device,
		filePath,
		_mipLevels,
		_textureManager,
		_viewManager,
		_textureName);
}


// =======================================
// 画像ファイルをロードする
// =======================================
const TextureHandle LoadTextureFromFile(
	ID3D11Device* _device,
	const std::filesystem::path& _texturePath,
	uint16_t _mipLevels,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager,
	const Hashed_String& _keyName)
{
	// 画像のロード
	DirectX::ScratchImage image;

	// 画像のロード
	HRESULT hr = DirectX::LoadFromWICFile(
		_texturePath.wstring().c_str(),
		DirectX::WIC_FLAGS_NONE,
		nullptr,
		image);

	// ロード失敗
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"画像のロードに失敗しました" + String::to_u8string((uint64_t)hr));
		return TextureHandle();
	}

	// 画像メタデータを取得する
	const DirectX::TexMetadata& meta = image.GetMetadata();

	// イメージデータ取得（mipmap0, arraySlice0）
	const DirectX::Image* img = image.GetImage(0, 0, 0);

	// 初期化データを作成
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = img->pixels;
	initData.SysMemPitch = static_cast<UINT>(img->rowPitch);
	initData.SysMemSlicePitch = 0;

	// テクスチャデスク作成
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = static_cast<UINT>(meta.width);
	desc.Height = static_cast<UINT>(meta.height);
	desc.MipLevels = static_cast<UINT>(_mipLevels);
	desc.ArraySize = static_cast<UINT>(meta.arraySize);
	desc.Format = meta.format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	// TextureManagerに登録
	Handle texHandle = _textureManager.Texture2DBufferCreateOnGet(
		_device,
		&desc,
		_keyName,
		&initData);

	// テクスチャを取得
	ID3D11Texture2D* data = _textureManager.GetTexture2DBuffer(texHandle);

	// SRVを作成して ResourceViewManager に登録
	Handle srvHandle = _viewManager.ShaderResourceViewCreateOnGet(
		_device,
		data,
		0,
		static_cast<UINT>(meta.mipLevels),
		_keyName);

	TextureHandle handle;
	handle.textureHandle = texHandle;
	handle.srvHandle = srvHandle;

	return handle;
}
