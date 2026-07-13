// ヘッダー
#include "DirectX11_TextureLoadModule.h"
#include "DirectX11_Texture2DBufferManager.h"
#include "DirectX11_ViewManager.h"
#include <filesystem>
#include <d3d11.h>
#include <DirectXTex.h>
#pragma comment(lib, "DirectXTex.lib")
#include "ReportMessage.h"

// プロトタイプ宣言
// 画像ファイルをロードする
TextureHandle LoadTextureFromFile(
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	const Hashed_String& _keyName,
	const std::filesystem::path& _texturePath,
	const TextureLoadDesc& _loadDesc,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager);

// 画像ファイルをロードする
// テクスチャフォルダーからロードする
TextureHandle DirectX11_TextureLoadModule::LoadFileTexture_TextureFolder(
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	const Hashed_String& _textureName,
	const String& _textureFolderName,
	const TextureLoadDesc& _loadDesc,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager)
{
	// ファイルパス作成
	std::filesystem::path filePath;
	filePath = _textureFolderName.GetU8String();
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
	filePath.make_preferred();

	// 画像のロード
	return LoadTextureFromFile(
		_device,
		_deviceContext,
		_textureName,
		filePath,
		_loadDesc,
		_textureManager,
		_viewManager);
}

// 画像ファイルをロードする
// モデルなどの直接パスからロードする
TextureHandle DirectX11_TextureLoadModule::LoadFileTexture(
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	const Hashed_String& _textureName,
	const String& _texturePath,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager)
{
	// ファイルパス作成
	std::filesystem::path filePath = _texturePath.GetU8String();
	filePath.make_preferred();

	// テクスチャのロードを作成
	TextureLoadDesc loadDesc = {};
	loadDesc.generateMip = true;

	// 画像のロード
	return LoadTextureFromFile(
		_device,
		_deviceContext,
		_textureName,
		filePath,
		loadDesc,
		_textureManager,
		_viewManager);
}


// =======================================
// 画像ファイルをロードする
// =======================================
TextureHandle LoadTextureFromFile(
	ID3D11Device* _device,
	ID3D11DeviceContext* _deviceContext,
	const Hashed_String& _keyName,
	const std::filesystem::path& _texturePath,
	const TextureLoadDesc& _loadDesc,
	DirectX11_Texture2DBufferManager& _textureManager,
	DirectX11_ViewManager& _viewManager)
{
	// 画像のロード
	DirectX::ScratchImage image;
	HRESULT hr = DirectX::LoadFromWICFile(
		_texturePath.wstring().c_str(),
		DirectX::WIC_FLAGS_NONE,
		nullptr,
		image);
	if (FAILED(hr)) {
		ErrorLog::OutputToConsole(u8"画像のロードに失敗しました" + String::to_u8string((uint64_t)hr));
		return TextureHandle();
	}

	// メタデータ取得
	const DirectX::TexMetadata& meta = image.GetMetadata();
	const DirectX::Image* img = image.GetImage(0, 0, 0);

	// 初期化データを作成
	D3D11_SUBRESOURCE_DATA initData = {};
	initData.pSysMem = img->pixels;
	initData.SysMemPitch = static_cast<UINT>(img->rowPitch);
	initData.SysMemSlicePitch = 0;

	// Textureを作成
	D3D11_TEXTURE2D_DESC desc = {};
	desc.Width = static_cast<UINT>(meta.width);
	desc.Height = static_cast<UINT>(meta.height);
	desc.MipLevels = (_loadDesc.generateMip) ? 0 : 1;
	desc.ArraySize = static_cast<UINT>(meta.arraySize);
	desc.Format = meta.format;
	desc.SampleDesc.Count = 1;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	if (_loadDesc.generateMip)
	{
		// 自動生成する場合、レンダーターゲットも必要
		desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		// ミップマップ自動生成フラグを立てる
		desc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
	}
	Handle texHandle = _textureManager.Texture2DBufferCreateOnGet(
		_device,
		desc,
		_keyName,
		&initData);

	// SRV作成
	const DirectX11_Texture2DBufferData* data = _textureManager.GetTexture2DBuffer(texHandle);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = desc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	Handle srvHandle = _viewManager.ShaderResourceViewCreateOnGet(
		_device,
		*(data->GetTexture()),
		srvDesc,
		_keyName);

	// ミップマップ自動生成
	if (_loadDesc.generateMip)
	{
		const DirectX11_SRVData* srv = _viewManager.GetShaderResourceView(srvHandle);
		_deviceContext->GenerateMips(srv->GetSRV());
	}

	TextureHandle handle;
	handle.textureHandle = texHandle;
	handle.srvHandle = srvHandle;

	return handle;
}
