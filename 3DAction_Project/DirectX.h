#pragma once
// ヘッダー内にインクルード非参照
#include <cstdint>
#include <Windows.h>

// ==============================================
// DirectXの関数
// ==============================================
namespace DirectX11 {
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle); // DirectXの初期化
}