#pragma once


// ===================================
// 【クラス概要】
// DirectX11のデバイスを作成・管理するクラス
// ===================================

// ===================================
// ヘッダー
// ===================================
#include <Windows.h>
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <wrl/client.h>
#include <cstdint>

// ===================================
// クラス
// ===================================
class DirectX11_DeviceManager final
{
private:
	// -----------------------------
	// メンバー変数
	// -----------------------------
	uint16_t m_Width;		// 画面の幅
	uint16_t m_Height;		// 画面の高さ

	// DirectXのコア
	Microsoft::WRL::ComPtr<ID3D11Device> m_Device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_DeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_SwapChain;

public:
	// コンストラクタ・デストラクタ
	DirectX11_DeviceManager() = default;
	~DirectX11_DeviceManager() = default;

	// DirectX11の 初期化
	bool Init(uint16_t _width, uint16_t _height, HWND _windowHandle);
	// DirectX11の 後処理
	void Uninit();

	// ゲッター関数
	ID3D11Device* GetDevice() const { return m_Device.Get(); }
	ID3D11DeviceContext* GetDeviceContext() const { return m_DeviceContext.Get(); }
	IDXGISwapChain* GetSwapChain() const { return m_SwapChain.Get(); }
};
