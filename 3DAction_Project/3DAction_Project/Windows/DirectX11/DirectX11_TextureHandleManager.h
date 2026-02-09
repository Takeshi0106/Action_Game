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
// モジュールヘッダー
#include "DirectX11_TextureLoadModule.h"


// ==========================================
// 前方宣言
// ==========================================
struct ID3D11Device;


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

	// テクスチャロードモジュール
	DirectX11_TextureLoadModule m_TextureLoadModule;

public:
	// コンストラクタ・デストラクタ
	DirectX11_TextureHandleManager() = default;
	~DirectX11_TextureHandleManager() = default;

	// ------------------------------------------
	// テクスチャハンドル追加
	// ------------------------------------------
	Handle CreateTextures(
		ID3D11Device* _device,
		const Hashed_String& _name,
		const D3D11_TEXTURE2D_DESC& _desc,
		const D3D11_SAMPLER_DESC& _dxDesc,
		const SamplerDesc& _myDesc,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc = nullptr,
		const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc = nullptr,
		const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc = nullptr);

	// ------------------------------------------
	// テクスチャロード関数 (ファイルから)
	// ------------------------------------------
	Handle LoadFaileTexture_TextureFolder(
		ID3D11Device* _device,
		const Hashed_String& _textureName,
		const String& _textureFolderName);

	// ------------------------------------------
	// テクスチャロード関数 (プロジェクトの相対パスから)
	// ------------------------------------------	
	Handle LoadFaileTexture(
		ID3D11Device* _device,
		const Hashed_String& _textureName,
		const String& _texturePath);

	// ------------------------------------------
	// テクスチャハンドル取得
	// ------------------------------------------
	const TextureHandle* GetTextureHandle(const Handle& _handle) {
		return m_TextureHandles.GetData(_handle);
	}

	// ------------------------------------------
	// テクスチャ削除
	// ------------------------------------------
	void ReleaseTexture(const Handle& _handle) {
		// テクスチャハンドル取得
		TextureHandle* textureHandle = m_TextureHandles.GetData(_handle);

		// 無効チェック
		if (!textureHandle) { return; }

		// 各リソース削除
		m_Texture2DBufferManager.ReleaseTexture2D(textureHandle->textureHandle);
		m_ViewManager.ReleaseRTV(textureHandle->rtvHandle);
		m_ViewManager.ReleaseDSV(textureHandle->dsvHandle);
		m_ViewManager.ReleaseSRV(textureHandle->srvHandle);
		m_SamplerManager.ReleaseSampler(textureHandle->samplerHandle);

		// テクスチャハンドル削除
		m_TextureHandles.Remove(_handle);
	}
	
	// ------------------------------------------
	// 全てのテクスチャ削除
	// ------------------------------------------
	void ReleaseAllTexture() {
		// 各リソース削除
		m_Texture2DBufferManager.ReleaseAllTexture2D();
		m_ViewManager.ReleaseAllView();
		m_SamplerManager.ReleaseAllSampler();

		// テクスチャハンドル全削除
		m_TextureHandles.ALLClear();
	}

};

