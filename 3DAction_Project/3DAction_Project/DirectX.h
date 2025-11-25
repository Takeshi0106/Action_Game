#pragma once

// ==============================================
// 【関数概要】
// DirectXの初期化、後処理関数
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
#include <cstdint>


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


// ================================================
// DirectXの初期化関数群
// SRVやUAVなどは別で作成する
// RTも最終描画用で１つしか持たない
// ================================================
namespace DirectX11 
{
	// DirectXの初期化
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle);
	// DirectXの後処理
	void Uninit();

	// ビューポート設定
	void SetViewPort(uint16_t width, uint16_t height);

	// ゲッター関数
	namespace Get 
	{
		ID3D11Device*        GetDevice();
		ID3D11DeviceContext* GetContext();
		IDXGISwapChain*      GetSwapChain();
	}
}
