#include "DirectX.h"
#include <dxgi.h>
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
		Microsoft::WRL::ComPtr<ID3D11Device>              d3dDevice        = nullptr; // リソースの作成
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>       d3dDeviceContext = nullptr; // 描画コマンドをGPUに送る
		Microsoft::WRL::ComPtr<IDXGISwapChain>            d3dSwapChain     = nullptr; // バッファを制御する
		// SRV用　読み取り専用
		Microsoft::WRL::ComPtr<ID3D11Texture2D>           d3dRTTforSRV     = nullptr; // レンダラーターゲットテクスチャ 描画結果を1次的に保存しておくバッファ
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    d3dRTV           = nullptr; // レンダーターゲットビュー  描画結果を描画対象として渡す
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  d3dRTSRV         = nullptr; // シェーダーに計算した描画結果を渡す (読み取り専用)
		// UAV用　読み書き可能
		Microsoft::WRL::ComPtr<ID3D11Texture2D>           d3dRTTforUAV     = nullptr; // レンダラーターゲットテクスチャ 描画結果を1次的に保存しておくバッファ (UAV用)
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> d3dUAV           = nullptr; // シェーダーに計算した画像結果を渡す (読み書き可能)
	}

	// =====================================================
	// 初期化処理
	// =====================================================
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle)
	{
		HRESULT  hr;             // 初期化の 成功、失敗 を受け取る
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
				d3dSwapChain.GetAddressOf(),    // 成功時に代入
				d3dDevice.GetAddressOf(),       // 成功時に代入
				&level,                         // 実際に使用されたDirectXのバージョンが入る
				d3dDeviceContext.GetAddressOf() // 成功時に代入される
			);							        

			if (SUCCEEDED(hr))
			{
				break; // ドライバが成功したらループを抜ける
			}
		}
		if (FAILED(hr)) // 全てのドライバが失敗したらエラー
		{ 
			assert(false && "選択されたドライバで生成することが出来ませんでした。");
			return false;
		}

		// SAVのレンダラーターゲットの作成 -----------------------------------------------------------
		hr = S_OK;

		// スワップチェインが生成したバックバッファを取得する
		hr = d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)d3dRTTforSRV.GetAddressOf());
		if (FAILED(hr))
		{
			assert(false && "SRVバックバッファを取得することが出来ませんでした。");
			return false;
		}
		// レンダラーターゲットを生成
		hr = d3dDevice->CreateRenderTargetView(d3dRTTforSRV.Get(), nullptr, d3dRTV.GetAddressOf());
		if (FAILED(hr))
		{
			assert(false && "レンダラーターゲットビューを作成することが出来ませんでした");
			return false;
		}
		// シェーダーリソースの作成
		hr = d3dDevice->CreateShaderResourceView(d3dRTTforSRV.Get(), nullptr, d3dRTSRV.GetAddressOf());
		if (FAILED(hr))
		{
			assert(false && "SRVが設定できませんでした");
			return false;
		}

		// UAVのレンダラーターゲットの作成 -----------------------------------------------------------
		// UAV用のテクスチャの構成
		D3D11_TEXTURE2D_DESC desc = {};
		desc.Width = Width;                           // テクスチャの横のピクセル数
		desc.Height = Height;                         // テクスチャの縦のピクセル数
		desc.MipLevels = 1;                           // ミップマップ
		desc.ArraySize = 1;                           // 配列のサイズ
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT; // ピクセルのフォーマット
		desc.SampleDesc = { 1,0 };                    // マルチサンプリング（アンチエイリアス無効）
		desc.Usage = D3D11_USAGE_DEFAULT;             // GPUが主にアクセスする
		desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS; // フラグ UAVのみで使用なので読み書き可能フラグ
		
		// バッファを作成
		d3dDevice->CreateTexture2D(&desc, nullptr, d3dRTTforUAV.GetAddressOf());

		// UAVを作成


		return true;
	}



}