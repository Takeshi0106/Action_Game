#pragma once

// =========================================
// 【クラス概要】
// テクスチャのハンドルマネージャー
// テクスチャのハンドルを管理するマネージャー
// =========================================


// ==========================================
// ヘッダー
// ==========================================
// テクスチャハンドルヘッダー
#include "../../TextureHandleh.h"
// ハッシュ文字列ヘッダー
#include "../../Hashed_String.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// リソースマネージャー
#include "DirectX11_Texture2DBufferManager.h"
#include "DirectX11_ViewManager.h"
#include "DirectX11_SamplerManager.h"


// ==========================================
// クラス
// ==========================================
class DirectX11_TextureHandleManager final
{
private:
	// メンバー変数
	TemplateManager<TextureHandle> m_TextureHandles;

	// 各リソースマネージャー
	DirectX11_Texture2DBufferManager m_Texture2DBufferManager;
	DirectX11_ViewManager m_ViewManager;
	DirectX11_SamplerManager m_SamplerManager;

public:
	// コンストラクタ・デストラクタ
	DirectX11_TextureHandleManager() = default;
	~DirectX11_TextureHandleManager() = default;

	// テクスチャハンドル追加
	const Handle CreateTextures(
		ID3D11Device* _device,
		const D3D11_TEXTURE2D_DESC* _desc,
		const Hashed_String& _name,
		const TextureHandle& _textureHandle,
		const D3D11_SUBRESOURCE_DATA* _initialData = nullptr);

	// テクスチャ削除
	void ReleaseTexture(const Handle& _handle);
	
	// 全てのテクスチャ削除
	void ReleaseAllTexture();
};

