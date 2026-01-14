
// =========================================
// ヘッダー
// =========================================
#include "DirectX11_ResourceManager.h"


// =========================================
// 初期化
// =========================================
bool DirectX11_ResourceManager::Init(ID3D11Device* _device,DrawPathConfig& _config)
{
	// インターフェイスに渡す構造体作成
	DirectX11_ResourceReference resourceReference = {
		m_ShaderManager,
		m_VertexBufferManager,
		m_IndexBufferManager,
		m_ConstantBufferManager,
		m_Texture2DBufferManager,
		m_SamplerManager,
		m_ViewManager,
		m_ModelLoadManager,
		m_MeshMaterialManager};

	// リソース作成クラス作成
	m_DrawCreate = std::make_unique<DirectX11_DrawCreate>(
		_device,
		resourceReference,
		_config.texturePath,
		_config.objModelPath
	);

	return true;
}
