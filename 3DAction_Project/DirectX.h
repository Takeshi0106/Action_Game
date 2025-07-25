#pragma once

#include <cstdint>
#include <Windows.h>

// ==============================================
// DirectXの関数
// ==============================================

namespace DirectX11 {
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle); // DirectXの初期化
	void Uninit();                                                 // DirectXの初期化

	void BeginDraw(); // 描画バッファと深度バッファの初期化処理
	void EndDraw();   // 次のバッファに入れ替える処理

	void DebugDraw(float time); // デバッグ用描画
}