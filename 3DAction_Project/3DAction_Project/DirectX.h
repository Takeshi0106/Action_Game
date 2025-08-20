#pragma once

// ==============================================
// 【関数概要】
// DirectXの初期化、後処理関数
// ==============================================


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
	bool Init(unsigned int Width, unsigned int Height, HWND windowHandle);
	// DirectXの後処理
	void Uninit();

	// デバッグ描画
	void BeginDraw(); // 描画バッファと深度バッファの初期化処理
	void EndDraw();   // 次のバッファに入れ替える処理

	void DebugDraw(float time); // デバッグ用描画

	// ゲッター関数
	namespace Get 
	{
		ID3D11Device*        GetDevice();
		ID3D11DeviceContext* GetContext();
		IDXGISwapChain*      GetSwapChain();
	}
}