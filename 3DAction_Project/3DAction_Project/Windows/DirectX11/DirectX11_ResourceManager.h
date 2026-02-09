#pragma once

// =======================================
// クラス概要
// DirectX11用リソース管理マネージャークラス
// =======================================


// =======================================
// ヘッダー
// =======================================
// リソース管理ヘッダー
// シェーダーマネージャー
#include "DirectX11_ShaderManager.h"
// 頂点バッファマネージャー
#include "DirectX11_VertexBufferManager.h"
// インデックスバッファマネージャー
#include "DirectX11_IndexBufferManager.h"
// 定数バッファマネージャー
#include "DirectX11_ConstantBufferManager.h"
// テクスチャマネージャー (Texture2D,View,Sampler)
#include "DirectX11_TextureHandleManager.h"
#include "../../ModelLoadManager.h"
#include "../../MeshMaterialManager.h"
// 作成クラスヘッダー
#include "DirectX11_DrawCreate.h"
// 動的確保
#include <memory>
// 設定パスヘッダー
#include "../../DrawPathConfig.h"


// =======================================
// 前方宣言
// =======================================
struct ID3D11Device;


// =======================================
// クラス
// =======================================
class DirectX11_ResourceManager
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// シェーダー
	DirectX11_ShaderManager m_ShaderManager;
	// 頂点バッファ
	DirectX11_VertexBufferManager m_VertexBufferManager;
	// インデックスバッファ
	DirectX11_IndexBufferManager m_IndexBufferManager;
	// 定数バッファ
	DirectX11_ConstantBufferManager m_ConstantBufferManager;
	// テクスチャマネージャー
	DirectX11_TextureHandleManager m_TextureHandleManager;
	// モデル
	ModelLoadManager m_ModelLoadManager;
	// マテリアル
	MeshMaterialManager m_MeshMaterialManager;


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
		const DrawPathConfig& _config) : 
		m_ShaderManager(_config.shaderBinaryPath),
		m_TextureHandleManager(_config.texturePath) {
	}
	~DirectX11_ResourceManager() = default;


	// --------------------------------
	// 初期化
	// --------------------------------
	bool Init(ID3D11Device* device);

	// --------------------------------
	// ゲッター
	// --------------------------------
	DirectX11_DrawCreate* GetDrawCreate() const {
		return m_DrawCreate.get();
	}

};

