#pragma once

// =========================================
// クラス概要
// テクスチャをロードするモジュール
// =========================================
#include "TextureHandle.h"
#include "UTF8_String.h"
#include "Hashed_String.h"
#include "GraphicsEnums.h"

// 前方宣言
struct ID3D11Device;
struct ID3D11DeviceContext;
class DirectX11_Texture2DBufferManager;
class DirectX11_ViewManager;

// クラス
class DirectX11_TextureLoadModule final
{
private:
	// メンバー変数
	const String& kPath;

public:
	// コンストラクタ・デストラクタ
	DirectX11_TextureLoadModule(const String& _path) : kPath(_path) {}
	~DirectX11_TextureLoadModule() = default;

	// テクスチャロード
	// テクスチャフォルダーからロードする
	TextureHandle LoadFileTexture_TextureFolder(
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const Hashed_String& _textureName,
		const String& _textureFolderName,
		const TextureLoadDesc& _loadDesc,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);

	// テクスチャロード
	// モデルなどでプロジェクトからの相対パスを指定してロードする
	TextureHandle LoadFileTexture(
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const Hashed_String& _textureName,
		const String& _texturePath,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);

};
