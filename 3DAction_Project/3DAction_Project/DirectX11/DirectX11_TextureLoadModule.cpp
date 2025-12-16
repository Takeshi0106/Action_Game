
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
#include "../ReportMessage.h"


// =======================================
// 画像ファイルをロードする
// =======================================
const TextureHandle DirectX11_TextureLoadModule::LoadFaileTexture(
	ID3D11Device* _device,
	const char* _textureName,
	uint32_t _mipLevels,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager)
{
	// ファイルパス作成
	std::filesystem::path filePath;

	// パス確認
	if (std::filesystem::path(_textureName).parent_path().empty())
	{
		// 相対パスを追加
		filePath = m_Path;
		filePath /= _textureName;
	}
	else
	{
		// そのまま使用
		filePath = _textureName;
	}

	// 区切り文字を統一する
	filePath.make_preferred();

	// 登録名
	std::string keyName = filePath.filename().string();

	// すでにSRVが登録されているか確認
	if (_viewManager.ExistsSRV(keyName.c_str()))
	{
		TextureHandle handle;
		handle.textureHandle = _textureManager.GetHandle(keyName.c_str());
		handle.srvHandle = _viewManager.GetSRVHandle(keyName.c_str());
		return handle;
	}

	// 画像のロード
	DirectX::ScratchImage image;

	// 画像のロード
	HRESULT hr = DirectX::LoadFromWICFile(
		filePath.wstring().c_str(),
		DirectX::WIC_FLAGS_NONE,
		nullptr,
		image);
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(("画像のロードに失敗しました" + std::to_string(hr)).c_str());
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
	Handle texHandle = _textureManager.Texture2DBufferCreate(
		_device,
		&desc,
		keyName.c_str(),
		&initData);

	// テクスチャを取得
	ID3D11Texture2D* data = _textureManager.GetTexture2DBuffer(texHandle);

	// SRVを作成して ResourceViewManager に登録
	Handle srvHandle = _viewManager.ShaderResourceViewCreate(
		_device,
		data,
		0,
		static_cast<UINT>(meta.mipLevels),
		keyName.c_str());

	TextureHandle handle;
	handle.textureHandle = texHandle;
	handle.srvHandle = srvHandle;

	return handle;
}

