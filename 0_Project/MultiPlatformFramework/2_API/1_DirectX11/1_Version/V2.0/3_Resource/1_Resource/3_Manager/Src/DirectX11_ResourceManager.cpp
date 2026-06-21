
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
bool DirectX11_ResourceManager::Init(
	const uint32_t& _screenWidth, const uint32_t& _screenHeight, 
	ID3D11Device* _device, ID3D11DeviceContext* _deviceContext)
{
	m_ScreenWidth = _screenWidth;
	m_ScreenHeight = _screenHeight;

	// 渡すマネージャーの参照作成
	DirectX11_ResourceReference resourceReference{
		m_ShaderManager,
		m_VertexBufferManager,
		m_IndexBufferManager,
		m_ConstantBufferManager,
		m_TextureResourceManager,
		m_SamplerManager,
		m_MaterialManager,
		m_ModelHandleManager
	};

	// リソース作成クラス作成
	m_ResourceFactory = std::make_unique<DirectX11_ResourceFactory>(
		m_ScreenWidth,
		m_ScreenHeight,
		_device,
		_deviceContext,
		resourceReference,
		m_ModelFolderPath);

	return true;
}
