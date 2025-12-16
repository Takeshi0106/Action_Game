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


// =========================================
// クラス
// =========================================
class DirectX11_TextureLoadModule final
{
private:
	// テクスチャフォルダパス
	const char* m_Path;

public:
	// ----------------------------------
	// コンストラクタ・デストラクタ
	// ----------------------------------
	DirectX11_TextureLoadModule(const char* _config) : m_Path(_config) {}
	~DirectX11_TextureLoadModule() = default;


	// ----------------------------------
	// テクスチャロード
	// ----------------------------------
	const TextureHandle LoadFaileTexture(
		ID3D11Device* _device,
		const char* _textureName,
		uint32_t _mipLevels,
		DirectX11_Texture2DBufferManager& _textureManager,
		DirectX11_ViewManager& _viewManager);
};

