#include "DirectX.h"
// #include <dxgi.h>
#include <d3d11.h>
#include <wrl/client.h> // マイクロソフトが提供するスマートポインタ

#pragma comment (lib, "d3d11.lib")      // DirectXの基本APIを使用するためのライブラリー
#pragma comment(lib, "d3dcompiler.lib") // HLSLをコンパイルするためAPIを使用するためのライブラリー

#if defined(DEBUG) || defined(_DEBUG) // デバッグ時にヘッダーをコンパイル
#include <cassert>
#endif

// =======================================
// DirectX 処理
// =======================================

namespace DirectX11 {

	namespace {
		// 使用しているモニターなどを取得する変数
		Microsoft::WRL::ComPtr<ID3D11Device>        Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> DeviceContext; 
		Microsoft::WRL::ComPtr<IDXGISwapChain>      SwapChain;
		// Microsoft::WRL::ComPtr<IDXGIOutput> pOutput = null;
		// Microsoft::WRL::ComPtr<ID3D11Device> pDevice = null;
	}

	// =====================================================
	// 初期化処理
	// =====================================================
	long Init(uint16_t Width, uint16_t Height, HWND windowHandle)
	{
		HRESULT  hr;             // スワップチェインなどが作製されたかを受け取る
		D3D_FEATURE_LEVEL level; // スワップチェイン作成成功時に使われたDirectXのバージョンを入れる

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

		/*
		// 使用しているモニターなどのリフレッシュレートを取得する
		DXGI_MODE_DESC desiredDesc = {};
		desiredDesc.Width = 1920;
		desiredDesc.Height = 1080;
		desiredDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		desiredDesc.RefreshRate.Numerator = 0;
		desiredDesc.RefreshRate.Denominator = 1;
		MF
		DXGI_MODE_DESC closestDesc = {};
		pOutput->FindClosestMatchingMode(&desiredDesc, &closestDesc, pDevice.Get());
		*/

		// スワップチェインの構成 -------------------------------------------------------------------
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};                                               // C++のためこっちで初期化
		// ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));                           // 全てに0を代入して、初期化する
		swapChainDesc.BufferCount = 2;                                                         // ダブルバッファ
		swapChainDesc.BufferDesc.Width = Width;                                                // 画面の縦幅 今はウィンドウサイズと同じにしている
		swapChainDesc.BufferDesc.Height = Height;                                              // 画面の横幅 今はウィンドウサイズと同じにしている
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                          // RGBA 各8ビット 0.0~1.0に正規化　＊一般的でどの環境でも動きやすい
		swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;                                    // リフレッシュレート　分母 (0の場合,OSに任せる)
		swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;                                  // リフレッシュレート　分子 (*ウィンドウモードの時は適用されない)
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT; // バックバッファの使用用途
		swapChainDesc.OutputWindow = windowHandle;                                             // 描画するウィンドウのハンドルを渡す
		swapChainDesc.SampleDesc.Count = 1;                                                    // マルチサンプリング　アンチエイリアス 1は無効
		swapChainDesc.SampleDesc.Quality = 0;                                                  // 品質レベル　大きい値ほど良くなる(フォーマットとサンプリング数で上限が決まる)
		swapChainDesc.Windowed = TRUE;                                                         // ウィンドウモード (FALSEにするとフルスクリーンモードになる)

		// デバイスとスワップチェインの作成 -----------------------------------------------------------
		for (unsigned int i = 0; i < numDriverTypes; i++) // 使用できるドライバーを見つけるまでループする
		{
			// デバイス・デバイスコンテキスト・スワップチェインを作製する
			hr = D3D11CreateDeviceAndSwapChain(
				nullptr,                        // 使用するGPUを設定 (nullptrの場合はデフォルトが選択)
				driverTypes[i],                 // ドライバの設定
				nullptr,                        // ソフトウェアレンダラーを設定 (今は使用せずGPUを使用する)
				createDeviceFlags,              // 動作モデルのフラグ　(Debug時にデバッグレイヤーを有効化する)
				featureLevels,                  // 使用したいDirectXのバージョン一覧
				numFeatureLevels,               // 試したいバージョンの数
				D3D11_SDK_VERSION,              // SDKバージョン (これしか入れてはいけない)
				&swapChainDesc,                 // スワップチェインの構成
				&SwapChain,                     // 成功時に代入
				&Device,                        // 成功時に代入
				&level,                         // 実際に使用されたDirectXのバージョンが入る
				&DeviceContext                  // 成功時に代入される
			);							        

			if (SUCCEEDED(hr))
			{
				break; // ドライバが成功したらループを抜ける
			}
		}
		if (FAILED(hr)) // 全てのドライバが失敗したらエラー
		{ 
			assert(false && "選択されたドライバで生成することが出来ませんでした。");
			return hr;
		}

		return S_OK;
	}



}