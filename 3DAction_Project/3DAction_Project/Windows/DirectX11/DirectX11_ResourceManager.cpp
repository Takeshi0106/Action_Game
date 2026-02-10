
// =========================================
// ヘッダー
// =========================================
// 必須ヘッダー
#include "DirectX11_ResourceManager.h"
// DirectXヘッダー
#include <d3d11.h>

// =========================================
// 初期化
// =========================================
bool DirectX11_ResourceManager::Init(ID3D11Device* _device)
{
	// リソース作成クラス作成
	m_DrawCreate = std::make_unique<DirectX11_DrawCreate>(
		m_ScreenWidth,
		m_ScreenHeight,
		_device,
		DirectX11_ResourceReference(
			m_ShaderManager,
			m_VertexBufferManager,
			m_IndexBufferManager,
			m_ConstantBufferManager,
			m_TextureHandleManager,
			m_ModelLoadManager,
			m_MeshMaterialManager));

	return true;
}
