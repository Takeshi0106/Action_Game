#include "DirectX.h"
#include <d3d11.h>
#include <wrl/client.h> // マイクロソフトが提供するスマートポインタ


// =======================================
// DirectX 処理
// =======================================

namespace DirectX11 {

	// =====================================================
	// 初期化処理
	// =====================================================
	void Init(uint16_t Width, uint16_t Height)
	{
		unsigned int createDeviceFlags = 0; // デバイス作成用のフラグ
#if defined(DEBUG) || defined(_DEBUG)
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; // デバッグ時にデバッグレイヤーを有効可　（ビットORで＋する）
#endif

		// ドライバーの優先度を作成　(環境に配慮) ------------------------------------------------
		// 使用可能なドライバータイプを順に試すための配列 (優先順位順)
		D3D_DRIVER_TYPE driverTypes[] = {
			D3D_DRIVER_TYPE_HARDWARE,  // GPUを使用して高速描画
			D3D_DRIVER_TYPE_WARP,      // CPUでGPUの代わりを行う。（マルチスレッドのため、高速レンダリング）
			D3D_DRIVER_TYPE_REFERENCE, // CPUで描画するソフトウェアレンダリング　（１スレッドのため遅い、開発・検証向け）
		};
		unsigned int numDriverTypes = sizeof(driverTypes) / sizeof(D3D_DRIVER_TYPE); // ループする回数を取得する

		// 機能のレベルの優先度を作成 ------------------------------------------------------------
		D3D_FEATURE_LEVEL featureLevels[] = {
			D3D_FEATURE_LEVEL_11_0, // DirectX11対応 (コンピュートシェーダーなどが使用可能)
			
			// もし対応できるようにする場合シェーダーを別に書かないといけない
			// D3D_FEATURE_LEVEL_10_1, コンピュートシェーダーなどは使用できない
			// D3D_FEATURE_LEVEL_10_0,
		};
		unsigned int numFeatureLevels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL); // ループする回数を取得する

		// スワップチェイン作成 -------------------------------------------------------------------
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {}; // C++のためこっちで初期化
		// ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));  // 全てに0を代入して、初期化する
		swapChainDesc.BufferCount = 2;                                // ダブルバッファ
		swapChainDesc.BufferDesc.Width = Width;                 // 画面の縦幅 今はウィンドウサイズと同じにしている
		swapChainDesc.BufferDesc.Height = Height;               // 画面の横幅 今はウィンドウサイズと同じにしている
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // RGBA 各8ビット 0.0~1.0に正規化　＊一般的でどの環境でも動きやすい
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;          // 
	}



}