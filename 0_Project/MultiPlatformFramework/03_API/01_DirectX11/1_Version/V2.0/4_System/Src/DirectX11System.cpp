
#include "DirectX11System.h"
#include "ReportMessage.h"

// =========================
// 初期化
// =========================
bool DirectX11System::Init(uint16_t _width, uint16_t _height, HWND _windowHandle)
{
	// DirectX11初期化
	if(!m_DeviceManager.Init(_width, _height, _windowHandle))
	{
		DebugLog::OutputToConsole(u8"DirectX11の初期化に失敗");
		return false;
	}
	// 描画ステート初期化
	if (!m_RendererStateManager.Init(m_DeviceManager.GetDeviceContext(), m_DeviceManager.GetDevice()))
	{
		DebugLog::OutputToConsole(u8"描画ステートの初期化に失敗");
		return false;
	}
	// 描画マネージャーの初期化
	if (!m_ResourceManager.Init(
		_width, _height, m_DeviceManager.GetDevice(), m_DeviceManager.GetDeviceContext()))
	{
		DebugLog::OutputToConsole(u8"V2.0 描画マネージャーの初期化に失敗");
		return false;
	}
	// 描画マネージャーの初期化
	if (!m_DrawManager.Init(_width, _height, _windowHandle))
	{
		DebugLog::OutputToConsole(u8"V1.0 描画マネージャーの初期化に失敗");
		return false;
	}

	return true;
}

// =========================
// 後処理
// =========================
void DirectX11System::Uninit()
{
	m_DrawManager.Uninit();
	
	// V2.0
	m_ResourceManager.ReleaseAllResource();
	m_RendererStateManager.Uninit();
	m_DeviceManager.Uninit();
}
