#pragma once

// ===============================
// DirectX11Systemクラス
// DirectX11の管理を行うクラス
// ===============================
#include "ISystemr.h"
#include "DirectX11_DeviceManager.h"
#include "DirectX11_RendererStateManager.h"
#include "DirectX11_ResourceManager.h"

// ===============================
// クラス
// ===============================
class DirectX11System final : public IAPISystem
{
private:
	// デバイスマネージャー
	DirectX11_DeviceManager m_DeviceManager;
	// レンダーステートマネージャー
	DirectX11_RendererStateManager m_RendererStateManager;
	// リソースファクトリー
	DirectX11_ResourceManager m_ResourceManager;

public:
	// コンストラクタ・デストラクタ
	DirectX11System() = default;
	~DirectX11System() = default;

	// 初期化・後処理
	bool Init(uint32_t _width, uint32_t _height, IWindowHandle& _windowHandle) override;
	void Uninit() override;
	
	// ゲッター関数
	const IDrawCreate* GetResourceFactory() override { return m_ResourceManager.GetResourceFactory(); }
};
