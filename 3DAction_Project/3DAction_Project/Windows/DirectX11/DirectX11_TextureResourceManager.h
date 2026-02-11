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
// テクスチャ関連構造体
#include "DirectX11_TextureStruct.h"
// ロードテクスチャデスク
#include "../../GraphicsEnums.h"
// モジュールヘッダー
#include "DirectX11_TextureLoadModule.h"


// ==========================================
// 前方宣言
// ==========================================
struct ID3D11Device;
struct ID3D11DeviceContext;


// ==========================================
// クラス
// ==========================================
class DirectX11_TextureResourceManager final
{
private:
	// メンバー変数
	TemplateManager<TextureHandle> m_TextureHandles;

	// 各リソースマネージャー
	DirectX11_Texture2DBufferManager m_Texture2DBufferManager;
	DirectX11_ViewManager m_ViewManager;

	// テクスチャロードモジュール
	DirectX11_TextureLoadModule m_TextureLoadModule;

public:
	// コンストラクタ・デストラクタ
	DirectX11_TextureResourceManager(
		const String& _texturePath) : m_TextureLoadModule(_texturePath) {
	}
	~DirectX11_TextureResourceManager() = default;

	// ------------------------------------------
	// テクスチャハンドル追加
	// ------------------------------------------
	Handle CreateTextures(
		ID3D11Device* _device,
		const Hashed_String& _name,
		const D3D11_TEXTURE2D_DESC& _desc,
		const D3D11_SHADER_RESOURCE_VIEW_DESC* _srvDesc = nullptr,
		const D3D11_RENDER_TARGET_VIEW_DESC* _rtvDesc = nullptr,
		const D3D11_DEPTH_STENCIL_VIEW_DESC* _dsvDesc = nullptr);

	// ------------------------------------------
	// テクスチャロード関数 (ファイルから)
	// ------------------------------------------
	Handle LoadFaileTexture_TextureFolder(
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const Hashed_String& _textureName,
		const TextureLoadDesc& _loadType,
		const String& _textureFolderName);

	// ------------------------------------------
	// テクスチャロード関数 (プロジェクトの相対パスから)
	// ------------------------------------------	
	Handle LoadFaileTexture(
		ID3D11Device* _device,
		ID3D11DeviceContext* _deviceContext,
		const Hashed_String& _textureName,
		const String& _texturePath);

	// ------------------------------------------
	// テクスチャハンドル取得
	// ------------------------------------------
	const DirectX11_TextureStruct GetTextureHandle(const Handle& _handle) {
		// テクスチャハンドル取得
		TextureHandle* handle = m_TextureHandles.GetData(_handle);

		// 保持しているデータをまとめて返す
		DirectX11_TextureStruct structData{};

		if (handle != nullptr) {
			structData.texture2DData = m_Texture2DBufferManager.GetTexture2DBuffer(handle->textureHandle);
			structData.srvData = m_ViewManager.GetShaderResourceView(handle->srvHandle);
			structData.rtvData = m_ViewManager.GetRenderTargetView(handle->rtvHandle);
			structData.dsvData = m_ViewManager.GetDepthStencilView(handle->dsvHandle);
		}

#if defined(DEBUG) || defined(_DEBUG)
		else {
			WarningLog::OutputToConsole(
				u8"無効なテクスチャハンドルが渡されました。");
		}
#endif

		return structData;
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

		// テクスチャハンドル全削除
		m_TextureHandles.ALLClear();
	}

};

