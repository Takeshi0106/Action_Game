
// =============================================================
// ヘッダー
// =============================================================
// 必須ヘッダー
#include "DirectX.h"    // 自分のヘッダー
// Windows・DirectX用
#include <Windows.h>                // ウィンドウのハンドル用
#include <d3d11.h>                  // DirectXの基本的なAPI
#pragma comment (lib, "d3d11.lib")  // DirectXの基本APIを使用するためのライブラリー
#include <wrl/client.h>             // マイクロソフトが提供するスマートポインタ
// 標準ライブラリ
#include <cstdint>      // 整数型 uintなど
// デバッグ用出力
#include "ReportMessage.h"


// =======================================
// DirectX の処理
// =======================================
namespace DirectX11 {

	// =====================================================
	// 変数
	// =====================================================
	namespace {

		// 描画する大きさ
		uint16_t RenderWidth  = 0;
		uint16_t RenderHeight = 0;

		// スワップチェインやデバイス
		Microsoft::WRL::ComPtr<ID3D11Device>              d3dDevice        = nullptr; // リソースの作成
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>       d3dDeviceContext = nullptr; // 描画コマンドをGPUに送る
		Microsoft::WRL::ComPtr<IDXGISwapChain>            d3dSwapChain     = nullptr; // バッファを制御する

		// UAV用　読み書き可能
		namespace UAV {
			Microsoft::WRL::ComPtr<ID3D11Texture2D>           d3dRTTforUAV = nullptr; // 読み書きができる記憶領域　UAV用
			Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> d3dUAV       = nullptr; // 読み書きができる記憶領域とコンピュートシェーダーなどを繋げる窓口
		}

		// 深度ステンシルバッファ
		namespace DepthStencil{

		}

	}


	// =====================================================
    // プロトタイプ宣言
    // =====================================================
	namespace { // 同じcppファイル内の無名名前空間は共通

		// デバイスとスワップチェインの初期化・後処理 --------------------------------------
		namespace DXCore {
			bool Init(HWND windowHandle); // 初期化関数
			void Uninit();                // 後処理
		}

		// UAVの初期化・後処理 ---------------------------------------------------------------
		namespace UAV {
			bool Init();   // 初期化
			void Uninit(); // 後処理
		}

		// 深度ステンシルの初期化・後処理 ---------------------------------------------------------------
		namespace DepthStencil {
			bool Init(); // 初期化
			void Uninit(); // 後処理
		}
	}


	// =====================================================
	// DirectX 初期化処理
	// =====================================================
	bool Init(uint16_t Width, uint16_t Height, HWND windowHandle)
	{
		// 描画する大きさを代入する
		RenderWidth  = Width;
		RenderHeight = Height;

		// デバイスやスワップチェインの初期化
		if(!DXCore::Init(windowHandle)){
			// メッセージボックス出力 環境の問題かもしれないためユーザーに分かるようにする
			ErrorLog::OutputToMessageBox("デバイスやスワップチェインの初期化に失敗");
			return false;
		}
		// UAVの初期化
		if (!UAV::Init()) {
			ErrorLog::OutputToConsole("URVの初期化に失敗");
			return false;
		}
		// 深度ステンシルの初期化
		if (!DepthStencil::Init()) {
			ErrorLog::OutputToConsole("深度ステンシルの初期化に失敗");
			return false;
		}

		DebugLog::OutputToConsole("DirectXの初期化に成功しました");

		return true;
	}


	// =====================================================
    // DirectX 後処理
    // =====================================================
	void Uninit()
	{
		// バインド解除
		d3dDeviceContext->ClearState();
		d3dDeviceContext->Flush();


		DepthStencil::Uninit();
		UAV::Uninit();
		DXCore::Uninit();
	}


	// =====================================================
    // ビューポート設定
    // =====================================================
	void SetViewPort(uint16_t width, uint16_t height)
	{
		D3D11_VIEWPORT viewPort = {};
		viewPort.Width = (float)width;   // ビューポートの横幅
		viewPort.Height = (float)height; // ビューポートの縦幅
		viewPort.MinDepth = 0.0f;       // 最も近い位置 
		viewPort.MaxDepth = 1.0f;       // 最も遠い位置 0~1で正規化する
		viewPort.TopLeftX = 0;          // 描画を始める位置
		viewPort.TopLeftY = 0;          // 描画を始める位置

		d3dDeviceContext->RSSetViewports(1, &viewPort);
	}


	// =====================================================
    // DirectX のゲッター
    // =====================================================
	namespace Get {
		ID3D11Device*        GetDevice()    { return d3dDevice.Get(); }
		ID3D11DeviceContext* GetContext()   { return d3dDeviceContext.Get(); }
		IDXGISwapChain*      GetSwapChain() { return d3dSwapChain.Get(); }
	}


	// =====================================================
    // プロトタイプ宣言の実態関数
    // =====================================================
	namespace {

		// ********************************************************************************
		// デバイスとスワップチェインの初期化・後処理
		// ********************************************************************************
		namespace DXCore {

			// -----------------------------------------------------
            // デバイスとスワップチェインの初期化
            // -----------------------------------------------------
			bool Init(HWND windowHandle)
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
				DXGI_SWAP_CHAIN_DESC swapChainDesc = {};                                                                      // C++のためこっちで初期化
				// ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));                                                  // 全てに0を代入して、初期化する
				swapChainDesc.BufferCount                        = 2;                                                         // ダブルバッファ
				swapChainDesc.BufferDesc.Width                   = RenderWidth;                                               // 画面の縦幅 今はウィンドウサイズと同じにしている
				swapChainDesc.BufferDesc.Height                  = RenderHeight;                                              // 画面の横幅 今はウィンドウサイズと同じにしている
				swapChainDesc.BufferDesc.Format                  = DXGI_FORMAT_R8G8B8A8_UNORM;                                // RGBA 各8ビット 0.0~1.0に正規化　＊一般的でどの環境でも動きやすい
				swapChainDesc.BufferDesc.RefreshRate.Numerator   = 0;                                                         // リフレッシュレート　分母 (0の場合,OSに任せる)
				swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;                                                         // リフレッシュレート　分子 (*ウィンドウモードの時は適用されない)
				swapChainDesc.BufferUsage                        = DXGI_USAGE_RENDER_TARGET_OUTPUT;                           // バックバッファの使用用途
				swapChainDesc.OutputWindow                       = windowHandle;                                              // 描画するウィンドウのハンドルを渡す
				swapChainDesc.SampleDesc.Count                   = 1;                                                         // マルチサンプリング　アンチエイリアス 1は無効
				swapChainDesc.SampleDesc.Quality                 = 0;                                                         // 品質レベル　大きい値ほど良くなる(フォーマットとサンプリング数で上限が決まる)
				swapChainDesc.Windowed                           = TRUE;                                                      // ウィンドウモード (FALSEにするとフルスクリーンモードになる)
				swapChainDesc.SwapEffect                         = DXGI_SWAP_EFFECT_FLIP_DISCARD;                             // 推奨されている

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

					if (SUCCEEDED(hr)){
						break; // ドライバが成功したらループを抜ける
					}
				}

				if (FAILED(hr)) {
					ErrorLog::OutputToConsole("選択されたドライバで生成することが出来ませんでした。");
					return false;
				}

				DebugLog::OutputToConsole("デバイス・スワップチェインの初期化に成功");
				
				return true;
			}


			// -----------------------------------------------------
            // デバイスとスワップチェインの後処理
            // -----------------------------------------------------
			void Uninit()
			{
#if defined(DEBUG) || defined(_DEBUG)
				// デバッグ表示用
				Microsoft::WRL::ComPtr<ID3D11Debug> debug;
				if (SUCCEEDED(d3dDevice.As(&debug)))
				{
					debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
				}
#endif

				d3dDeviceContext.Reset(); // デバイスコンテキストの解放
				d3dSwapChain.Reset();     // スワップチェインの解放
				d3dDevice.Reset();        // デバイスの解放
			}


		}


		// *********************************************************************************
        // UAVの初期化・後処理 
        // *********************************************************************************
		namespace UAV {

			// -----------------------------------------------------
			// UAVの初期化
			// -----------------------------------------------------
			bool Init()
			{
				HRESULT  hr = S_OK; // 初期化の 成功、失敗 を受け取る

                // UAV用のテクスチャの構成
				D3D11_TEXTURE2D_DESC textureDesc = {};
				textureDesc.Width      = RenderWidth;                    // テクスチャの横のピクセル数
				textureDesc.Height     = RenderHeight;                   // テクスチャの縦のピクセル数
				textureDesc.MipLevels  = 1;                              // ミップマップのレベル（1の場合、ミップマップを使用しない）
				textureDesc.ArraySize  = 1;                              // 配列のサイズ
				textureDesc.Format     = DXGI_FORMAT_R32G32B32A32_FLOAT; // ピクセルのフォーマット
				textureDesc.SampleDesc = { 1,0 };                        // マルチサンプリング（アンチエイリアス無効）
				textureDesc.Usage      = D3D11_USAGE_DEFAULT;            // GPUが主にアクセスする
				textureDesc.BindFlags  = D3D11_BIND_UNORDERED_ACCESS;    // フラグ UAVのみで使用なので読み書き可能フラグ

				// バッファを作成
				hr = d3dDevice->CreateTexture2D(&textureDesc, nullptr, d3dRTTforUAV.GetAddressOf());
				if (FAILED(hr)) {
					ErrorLog::OutputToConsole("UAVテクスチャの作成に失敗しました");
					return false;
				}

				// UAVを作成
				D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
				uavDesc.Format             = textureDesc.Format;            // UAVテクスチャフォーマットを使用する
				uavDesc.ViewDimension      = D3D11_UAV_DIMENSION_TEXTURE2D; // アクセスする値フラグを渡す
				uavDesc.Texture2D.MipSlice = 0;                             // ミップマップレベル（０の場合 自動的に最大数使用してくれる）

				hr = d3dDevice->CreateUnorderedAccessView(d3dRTTforUAV.Get(), &uavDesc, d3dUAV.GetAddressOf()); // UAVを作成
				if (FAILED(hr)) {
					ErrorLog::OutputToConsole("UAVの作成に失敗しました");
					return false;
				}

				DebugLog::OutputToConsole("UAVの初期化に成功");

				return true;
			}


			// -----------------------------------------------------
            // UAVの後処理
            // -----------------------------------------------------
			void Uninit()
			{
				d3dUAV.Reset();       // UAVを解放
				d3dRTTforUAV.Reset(); // UAVの記憶領域を解放
			}


		}


		// *********************************************************************************
        // 深度ステンシルの初期化・後処理 
        // *********************************************************************************
		namespace DepthStencil {

			// -----------------------------------------------------
            // 深度ステンシルの初期化
            // -----------------------------------------------------
			bool Init()
			{

				return true;
			}


			// -----------------------------------------------------
            // 深度ステンシルの後処理
            // -----------------------------------------------------
			void Uninit()
			{

			}


		}


	}


}
