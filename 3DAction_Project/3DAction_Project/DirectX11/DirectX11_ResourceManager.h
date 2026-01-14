#pragma once

// =======================================
// クラス概要
// DirectX11用リソース管理マネージャークラス
// =======================================


// =======================================
// ヘッダー
// =======================================
// DirectXヘッダー
#include <d3d11.h>
// リソース管理ヘッダー
#include "DirectX11_ShaderManager.h"
#include "DirectX11_VertexBufferManager.h"
#include "DirectX11_IndexBufferManager.h"
#include "DirectX11_ConstantBufferManager.h"
#include "DirectX11_Texture2DBufferManager.h"
#include "DirectX11_SamplerManager.h"
#include "DirectX11_ViewManager.h"
#include "../ModelLoadManager.h"
#include "../MeshMaterialManager.h"
// 作成クラスヘッダー
#include "DirectX11_DrawCreate.h"
// 動的確保
#include <memory>
// 設定パスヘッダー
#include "../DrawPathConfig.h"


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
	// テクスチャ2Dバッファ
	DirectX11_Texture2DBufferManager m_Texture2DBufferManager;
	// サンプラー
	DirectX11_SamplerManager m_SamplerManager;
	// ビュー
	DirectX11_ViewManager m_ViewManager;
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
	DirectX11_ResourceManager() = default;
	~DirectX11_ResourceManager() = default;


	// --------------------------------
	// 初期化
	// --------------------------------
	bool Init(ID3D11Device* device,
		DrawPathConfig& _config);

	// --------------------------------
	// ゲッター
	// --------------------------------
	DirectX11_DrawCreate* GetDrawCreate() const {
		return m_DrawCreate.get();
	}

};

