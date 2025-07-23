#pragma once
#include <cstdint>

// ==============================================
// 前方定義
// ==============================================
// ウィンドウハンドルの前方宣言
struct DX_WINDOWHANDLE;                   // DierctXの初期化で使用するHWNDのラップ構造体

// ==============================================
// DirectXの関数
// ==============================================
namespace DirectX11 {
	void Init(uint16_t Width, uint16_t Height, DX_WINDOWHANDLE windowHandle);
}