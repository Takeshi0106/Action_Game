#pragma once

// =======================================
// クラス概要
// DirectX11用リソース管理マネージャークラス
// =======================================
// 設定パスヘッダー
#include "DrawPathConfig.h"
// 動的確保
#include <memory>
// 整数ヘッダー
#include <cstdint>
// リソース管理ヘッダー
#include "DirectX11_ShaderManager.h"
#include "DirectX11_VertexBufferManager.h"
#include "DirectX11_IndexBufferManager.h"
#include "DirectX11_ConstantBufferManager.h"
#include "DirectX11_TextureResourceManager.h"
#include "DirectX11_SamplerManager.h"
#include "MaterialHandleManager.h"
#include "ModelHandleManager.h"
// 作成クラスヘッダー
#include "DirectX11_ResourceFactory.h"

// 前方宣言
struct ID3D11Device;

// =======================================
// クラス
// =======================================
class DirectX11_ResourceManager final
{
private:
	// メンバー変数
	uint32_t m_ScreenWidth = 0;
	uint32_t m_ScreenHeight = 0;
	// モデルフォルダパス
	const String& m_ModelFolderPath;
	// 各リソースマネージャー
	DirectX11_ShaderManager m_ShaderManager;
	DirectX11_VertexBufferManager m_VertexBufferManager;
	DirectX11_IndexBufferManager m_IndexBufferManager;
	DirectX11_ConstantBufferManager m_ConstantBufferManager;
	DirectX11_TextureResourceManager m_TextureResourceManager;
	DirectX11_SamplerManager m_SamplerManager;
	MaterialHandleManager m_MaterialManager;
	ModelHandleManager m_ModelHandleManager;
	// リソース作成
	std::unique_ptr<DirectX11_ResourceFactory> m_ResourceFactory;

public:
	// コンストラクタ・デストラクタ
	DirectX11_ResourceManager(
		const DrawPathConfig& _config) :
		m_ModelFolderPath(_config.objModelPath),
		m_ShaderManager(_config.shaderBinaryPath),
		m_TextureResourceManager(_config.texturePath) {
	}
	~DirectX11_ResourceManager() = default;
	
	// 初期化
	bool Init(const uint32_t& _screenWidth, const uint32_t& _screenHeight, 
		ID3D11Device* device,ID3D11DeviceContext* _deviceContext);

	// ゲッター
	const DirectX11_ResourceFactory* GetResourceFactory() const {
		return m_ResourceFactory.get();
	}

	// リソースマネージャー全削除
	void ReleaseAllResource() 
	{
		m_ResourceFactory.release();
		// マネージャー削除
		m_ShaderManager.ReleaseAllShader();
		m_VertexBufferManager.ReleaseAllVertexBuffers();
		m_IndexBufferManager.ReleaseAllIndexBuffer();
		m_ConstantBufferManager.ReleaseAllConstantBuffer();
		m_TextureResourceManager.ReleaseAllTexture();
		m_SamplerManager.ReleaseAllSampler();
	}
};
