#pragma once

// ===============================
// DirectX11Systemクラス
// DirectX11の管理を行うクラス
// ===============================
#include "DirectX11_DrawManager.h"
// V2.0で追加
#include "DirectX11_DeviceManager.h"
#include "DirectX11_RendererStateManager.h"
#include "DirectX11_ResourceManager.h"

// ===============================
// クラス
// ===============================
class DirectX11System final
{
private:
	// V1
	// 描画マネージャー
	DirectX_DrawManager m_DrawManager;

	// V2
	// デバイスマネージャー
	DirectX11_DeviceManager m_DeviceManager;
	// レンダーステートマネージャー
	DirectX11_RendererStateManager m_RendererStateManager;
	// リソースファクトリー
	DirectX11_ResourceManager m_ResourceManager;

public:
	DirectX11System(const DrawPathConfig& _config) : m_DrawManager(_config), m_ResourceManager(_config) {}
	~DirectX11System() = default;

	// 初期化
	bool Init(uint16_t _width, uint16_t _height, HWND _windowHandle);
	// 後処理
	void Uninit();
	
	// ゲッター関数
	const IDrawManager& GetDrawManager() { return m_DrawManager; }
	const IDrawCreate* GetResourceFactory() { return m_ResourceManager.GetResourceFactory(); }
};
