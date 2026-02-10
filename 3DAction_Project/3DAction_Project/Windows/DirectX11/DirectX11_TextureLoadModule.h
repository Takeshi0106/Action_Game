#pragma once

// =========================================
// クラス概要
// テクスチャをロードするモジュール
// =========================================


// =========================================
// ヘッダー
// =========================================
// テクスチャハンドル
#include "../../TextureHandleh.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"
// ハッシュ文字列ヘッダー
#include "../../Hashed_String.h"
// テクスチャロードデスクヘッダー
#include "../../GraphicsEnums.h"


// =========================================
// 前方宣言
// =========================================
// DirectX11デバイス
struct ID3D11Device;
struct ID3D11DeviceContext;
// DirectX11各リソースマネージャー
class DirectX11_Texture2DBufferManager;
class DirectX11_ViewManager;


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
	TextureHandle LoadFaileTexture_TextureFolder(
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const Hashed_String& _textureName,
		const String& _textureFolderName,
		const TextureLoadDesc& _loadDesc,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);


	// ----------------------------------
	// テクスチャロード
	// モデルなどでプロジェクトからの相対パスを指定してロードする
	// ----------------------------------
	TextureHandle LoadFaileTexture(
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const Hashed_String& _textureName,
		const String& _texturePath,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);

};

