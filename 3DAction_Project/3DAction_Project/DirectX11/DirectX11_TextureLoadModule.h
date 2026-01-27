#pragma once

// =========================================
// クラス概要
// テクスチャをロードするモジュール
// =========================================


// =========================================
// ヘッダー
// =========================================
// DirectXヘッダー
#include <d3d11.h>
// テクスチャハンドル
#include "../TextureHandleh.h"
// SRVマネージャー
#include "DirectX11_ViewManager.h"
// テクスチャ作成ヘッダー
#include "DirectX11_Texture2DBufferManager.h"
// 文字列ヘッダー
#include "../UTF8_String.h"


// =========================================
// クラス
// =========================================
class DirectX11_TextureLoadModule final
{
private:
	// ----------------------------------
	// メンバー変数
	// ----------------------------------
	// テクスチャフォルダパス
	const String& kPath;


	// ----------------------------------
	// メンバー関数
	// ----------------------------------
	const TextureHandle LoadTextureFromFile(
		ID3D11Device* _device,
		const String& _texturePath,
		uint16_t _mipLevels,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);


public:
	// ----------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------
	DirectX11_TextureLoadModule(const String& _path) : kPath(_path) {}
	~DirectX11_TextureLoadModule() = default;


	// ----------------------------------
	// テクスチャロード
	// テクスチャフォルダーからロードする
	// ----------------------------------
	const TextureHandle LoadFaileTexture_TextureFolder(
		ID3D11Device* _device,
		const String& _textureName,
		const String& _textureFolderName,
		uint16_t _mipLevels,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);


	// ----------------------------------
	// テクスチャロード
	// モデルなどで直接パスを指定してロードする
	// ----------------------------------
	const TextureHandle LoadFaileTexture(
		ID3D11Device* _device,
		const String& _texturePath,
		uint16_t _mipLevels,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);
};

