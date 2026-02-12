#pragma once

// =======================================
// クラス概要
// DirectX11用リソース管理マネージャークラス
// =======================================


// =======================================
// ヘッダー
// =======================================
// 設定パスヘッダー
#include "../../DrawPathConfig.h"
// 動的確保
#include <memory>
// 整数ヘッダー
#include <cstdint>

// ---------------------------------------
// リソース管理ヘッダー
// ---------------------------------------
// シェーダーマネージャー
#include "DirectX11_ShaderManager.h"
// 頂点バッファマネージャー
#include "DirectX11_VertexBufferManager.h"
// インデックスバッファマネージャー
#include "DirectX11_IndexBufferManager.h"
// 定数バッファマネージャー
#include "DirectX11_ConstantBufferManager.h"
// テクスチャマネージャー (Texture2D,View)
#include "DirectX11_TextureResourceManager.h"
// サンプラーマネージャー
#include "DirectX11_SamplerManager.h"
// マテリアルマネージャー
#include "../../MaterialHandleManager.h"
// モデルハンドルマネージャー
#include "../../ModelHandleManager.h"

// -----------------------------------
// 作成クラスヘッダー
// ------------------------------------
#include "DirectX11_DrawCreate.h"


// =======================================
// 前方宣言
// =======================================
struct ID3D11Device;


// =======================================
// クラス
// =======================================
class DirectX11_ResourceManager final
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// スクリーンサイズ
	uint32_t m_ScreenWidth = 0;
	uint32_t m_ScreenHeight = 0;

	// モデルフォルダパス
	const String& m_ModelFolderPath;

	// シェーダー
	DirectX11_ShaderManager m_ShaderManager;
	// 頂点バッファ
	DirectX11_VertexBufferManager m_VertexBufferManager;
	// インデックスバッファ
	DirectX11_IndexBufferManager m_IndexBufferManager;
	// 定数バッファ
	DirectX11_ConstantBufferManager m_ConstantBufferManager;
	// テクスチャ関連マネージャー
	DirectX11_TextureResourceManager m_TextureResourceManager;
	// サンプラーマネージャー
	DirectX11_SamplerManager m_SamplerManager;

	// マテリアルマネージャー
	MaterialHandleManager m_MaterialManager;
	// モデルハンドルマネージャー
	ModelHandleManager m_ModelHandleManager;


	// --------------------------------
	// ゲーム層とプラットフォーム層をつなぐ
	// インターフェイスとしての役割があるクラス
	// --------------------------------
	// リソース作成クラス
	std::unique_ptr<DirectX11_DrawCreate> m_DrawCreate;


public:
	// --------------------------------
	// コンストラクタ・デストラクタ
	// --------------------------------
	DirectX11_ResourceManager(
		const uint32_t& _screenWidth,
		const uint32_t& _screenHeight,
		const DrawPathConfig& _config) :
		m_ScreenWidth(_screenWidth),
		m_ScreenHeight(_screenHeight),
		m_ShaderManager(_config.shaderBinaryPath),
		m_TextureResourceManager(_config.texturePath),
		m_ModelFolderPath(_config.objModelPath) {
	}
	~DirectX11_ResourceManager() = default;


	// --------------------------------
	// 初期化
	// --------------------------------
	bool Init(ID3D11Device* device,ID3D11DeviceContext* _deviceContext);

	// --------------------------------
	// ゲッター
	// --------------------------------
	DirectX11_DrawCreate* GetDrawCreate() const {
		return m_DrawCreate.get();
	}

	// --------------------------------
	// リソースマネージャー全削除
	// --------------------------------
	void ReleaseAllResource() {
		// シェーダー全削除
		m_ShaderManager.ReleaseAllShader();
		// 頂点バッファ全削除
		m_VertexBufferManager.ReleaseAllVertexBuffers();
		// インデックスバッファ全削除
		m_IndexBufferManager.ReleaseAllIndexBuffer();
		// 定数バッファ全削除
		m_ConstantBufferManager.ReleaseAllConstantBuffer();
		// テクスチャ全削除
		m_TextureResourceManager.ReleaseAllTexture();
		// サンプラー全削除
		m_SamplerManager.ReleaseAllSampler();
	}

};

