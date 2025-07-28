#pragma once

// 標準ヘッダー
#include <cstdint>   // 整数型 uintなど


// ==============================================
// 前方宣言
// ==============================================
// ウィンドウハンドルの前方宣言
struct HWND__;                   
using  HWND = HWND__*;
// DirectXのコアの前方宣言
struct ID3D11Device;
struct ID3D11DeviceContext;
struct IDXGISwapChain;

// ==============================================
// DirectXの関数
// ==============================================
namespace DirectX11 {
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle); // DirectXの初期化
	void Uninit();                                                 // DirectXの初期化

	void BeginDraw(); // 描画バッファと深度バッファの初期化処理
	void EndDraw();   // 次のバッファに入れ替える処理

	void DebugDraw(float time); // デバッグ用描画

	namespace Get { // ゲッター関数
		ID3D11Device* GetDevice();
		ID3D11DeviceContext* GetContext();
		IDXGISwapChain* GetSwapChain();
	}
}