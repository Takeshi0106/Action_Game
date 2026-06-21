
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
// 独自深度ステンシル設定ヘッダー
#include "DepthStencilSetting.h"
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
		uint16_t RenderWidth = 0;
		uint16_t RenderHeight = 0;

		// スワップチェインやデバイス
		Microsoft::WRL::ComPtr<ID3D11Device>              d3dDevice = nullptr; // リソースの作成
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>       d3dDeviceContext = nullptr; // 描画コマンドをGPUに送る
		Microsoft::WRL::ComPtr<IDXGISwapChain>            d3dSwapChain = nullptr; // バッファを制御する

		// 描画設定
		namespace DrawSetting {
			// 描画設定配列
			Microsoft::WRL::ComPtr<ID3D11RasterizerState> drawSetting[(size_t)CullingSetting::Culling_Setting_Max * (size_t)FillModeSetting::FillMode_Max];
		}

		// 深度ステンシルバッファ
		namespace DepthStencil {
			// 深度ステンシル設定配列
			Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilSetting[DepthStencilSetting::Depth_Setting_Max];
		}

		// アルファディザ
		namespace AlphaDiza {
			// アルファディザ設定配列
			Microsoft::WRL::ComPtr<ID3D11BlendState> alphaDizaSetting[AlphaDizaSetting::Blend_Setting_Max];
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

		// 描画設定の初期化・後処理
		namespace DrawSetting {
			bool Init(); // 初期化
			void Uninit(); // 後処理
		}

		// 深度ステンシルの初期化・後処理 ---------------------------------------------------------------
		namespace DepthStencil {
			bool Init(); // 初期化
			void Uninit(); // 後処理
		}

		// アルファディザ設定の初期化・後処理 -----------------------------------------------------------
		namespace AlphaDiza {
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
		RenderWidth = Width;
		RenderHeight = Height;

		// デバイスやスワップチェインの初期化
		if (!DXCore::Init(windowHandle)) {
			// メッセージボックス出力 環境の問題かもしれないためユーザーに分かるようにする
			ErrorLog::OutputToMessageBox(u8"デバイスやスワップチェインの初期化に失敗");
			return false;
		}

		// 描画設定の初期化
		if (!DrawSetting::Init()) {
			ErrorLog::OutputToConsole(u8"描画設定の初期化に失敗");
			return false;
		}

		// 深度ステンシルの初期化
		if (!DepthStencil::Init()) {
			ErrorLog::OutputToConsole(u8"深度ステンシルの初期化に失敗");
			return false;
		}

		// アルファディザ設定の初期化
		if (!AlphaDiza::Init()) {
			ErrorLog::OutputToConsole(u8"アルファディザ設定の初期化に失敗");
			return false;
		}

		DebugLog::OutputToConsole(u8"DirectXの初期化に成功しました");

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

		// 後処理
		AlphaDiza::Uninit();
		DepthStencil::Uninit();
		DrawSetting::Uninit();
		DXCore::Uninit();
	}


	// =====================================================
	// ビューポート設定
	// =====================================================
	void SetViewPort(uint32_t width, uint32_t height)
	{
		D3D11_VIEWPORT viewPort = {};
		viewPort.Width = (FLOAT)width;   // ビューポートの横幅
		viewPort.Height = (FLOAT)height; // ビューポートの縦幅
		viewPort.MinDepth = 0.0f;       // 最も近い位置 
		viewPort.MaxDepth = 1.0f;       // 最も遠い位置 0~1で正規化する
		viewPort.TopLeftX = 0;          // 描画を始める位置
		viewPort.TopLeftY = 0;          // 描画を始める位置

		d3dDeviceContext->RSSetViewports(1, &viewPort);
	}


	// =====================================================
	// 描画設定
	// =====================================================
	void SetDrawSetting(CullingSetting culling, FillModeSetting fillMode)
	{
		d3dDeviceContext->RSSetState(DrawSetting::drawSetting[(size_t)culling * (size_t)FillModeSetting::FillMode_Max + fillMode].Get());
	}


	// =====================================================
	// 深度ステンシル設定
	// =====================================================
	void SetDepthStencilSetting(DepthStencilSetting setting)
	{
		d3dDeviceContext->OMSetDepthStencilState(DepthStencil::depthStencilSetting[setting].Get(), 0);
	}


	// =====================================================
	// アルファディザ設定
	// =====================================================
	void SetAlphaDizaSetting(AlphaDizaSetting setting)
	{
		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		UINT  sampleMask = 0xffffffff;
		d3dDeviceContext->OMSetBlendState(AlphaDiza::alphaDizaSetting[setting].Get(), blendFactor, sampleMask);
	}


	// =====================================================
	// DirectX のゲッター
	// =====================================================
	namespace Get {
		ID3D11Device* GetDevice() { return d3dDevice.Get(); }
		ID3D11DeviceContext* GetContext() { return d3dDeviceContext.Get(); }
		IDXGISwapChain* GetSwapChain() { return d3dSwapChain.Get(); }
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
				swapChainDesc.BufferCount = 2;                                                         // ダブルバッファ
				swapChainDesc.BufferDesc.Width = RenderWidth;                                               // 画面の縦幅 今はウィンドウサイズと同じにしている
				swapChainDesc.BufferDesc.Height = RenderHeight;                                              // 画面の横幅 今はウィンドウサイズと同じにしている
				swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                                // RGBA 各8ビット 0.0~1.0に正規化　＊一般的でどの環境でも動きやすい
				swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;                                                         // リフレッシュレート　分母 (0の場合,OSに任せる)
				swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;                                                         // リフレッシュレート　分子 (*ウィンドウモードの時は適用されない)
				swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                           // バックバッファの使用用途
				swapChainDesc.OutputWindow = windowHandle;                                              // 描画するウィンドウのハンドルを渡す
				swapChainDesc.SampleDesc.Count = 1;                                                         // マルチサンプリング　アンチエイリアス 1は無効
				swapChainDesc.SampleDesc.Quality = 0;                                                         // 品質レベル　大きい値ほど良くなる(フォーマットとサンプリング数で上限が決まる)
				swapChainDesc.Windowed = TRUE;                                                      // ウィンドウモード (FALSEにするとフルスクリーンモードになる)
				swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                             // 推奨されている

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

					if (SUCCEEDED(hr)) {
						break; // ドライバが成功したらループを抜ける
					}
				}

				if (FAILED(hr)) {
					ErrorLog::OutputToConsole(u8"選択されたドライバで生成することが出来ませんでした。");
					return false;
				}

				DebugLog::OutputToConsole(u8"デバイス・スワップチェインの初期化に成功");

				return true;
			}


			// -----------------------------------------------------
			// デバイスとスワップチェインの後処理
			// -----------------------------------------------------
			void Uninit()
			{

				d3dDeviceContext.Reset(); // デバイスコンテキストの解放
				d3dSwapChain.Reset();     // スワップチェインの解放

#if defined(DEBUG) || defined(_DEBUG)
				// デバッグ表示用
				Microsoft::WRL::ComPtr<ID3D11Debug> debug;
				if (SUCCEEDED(d3dDevice.As(&debug)))
				{
					debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
				}
				debug.Reset();
#endif

				d3dDevice.Reset();        // デバイスの解放
			}


		}


		// *********************************************************************************
		// 描画設定の初期化・後処理 
		// *********************************************************************************
		namespace DrawSetting {

			// -----------------------------------------------------
			// 描画設定の初期化
			// -----------------------------------------------------
			bool Init()
			{
				// カリング設定
				D3D11_RASTERIZER_DESC rasterDesc = {};
				rasterDesc.FillMode = D3D11_FILL_SOLID;                   // ポリゴンの塗りつぶし方法 (線画・塗りつぶし)
				rasterDesc.CullMode = D3D11_CULL_BACK;                    // カリング方法 (裏面をカリング)
				rasterDesc.FrontCounterClockwise = false;                 // 頂点の並び順 (時計回りが表面)
				rasterDesc.DepthClipEnable = true;                        // 深度クリッピングを有効化

				// カリング設定パターン
				D3D11_CULL_MODE cullModePattern[] = {
					D3D11_CULL_NONE,   // カリング無し
					D3D11_CULL_FRONT,  // 表面カリング
					D3D11_CULL_BACK,   // 裏面カリング
				};

				D3D11_FILL_MODE fillModePattern[] = {
					D3D11_FILL_SOLID,     // 塗りつぶし
					D3D11_FILL_WIREFRAME, // ワイヤーフレーム
				};

				// カリング設定の作成
				for (size_t i = 0; i < (size_t)CullingSetting::Culling_Setting_Max * (size_t)FillModeSetting::FillMode_Max; i++)
				{
					// カリング設定を作成
					rasterDesc.CullMode = cullModePattern[i / (size_t)FillModeSetting::FillMode_Max];
					rasterDesc.FillMode = fillModePattern[i % (size_t)FillModeSetting::FillMode_Max];

					HRESULT hr = d3dDevice->CreateRasterizerState(&rasterDesc, drawSetting[i].GetAddressOf());

					if (FAILED(hr)) {
						DebugLog::OutputToConsole(u8"カリング設定の作成に失敗しました");
						return false;
					}
				}

				// 初期状態は裏面カリングにする
				SetDrawSetting(CullingSetting::Back_Culling, FillModeSetting::Solid);
				return true;
			}


			// -----------------------------------------------------
			// カリング設定の後処理
			// -----------------------------------------------------
			void Uninit()
			{
				for (size_t i = 0; i < (size_t)CullingSetting::Culling_Setting_Max * (size_t)FillModeSetting::FillMode_Max; i++)
				{
					drawSetting[i].Reset();
				}
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
				// 深度設定
				D3D11_DEPTH_STENCIL_DESC dsDesc = {};
				dsDesc.DepthEnable = true; // 深度テストを有効化
				dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL; // 描画する深度
				dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL; // 深度の書き込みを有効化
				dsDesc.StencilEnable = true; // ステンシルテストを有効化
				dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK; // ステンシルの読み取りマスク
				dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK; // ステンシルの書き込みマスク
				dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP; // ステンシルテスト失敗時の動作 (変更しない)
				dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // ステンシルテストは成功、深度テスト失敗時の動作 (変更しない)
				dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR; // ステンシルテスト・深度テスト共に成功時の動作 (値を増やす)
				dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL; // ステンシルテストの比較方法 (大きいか等しい場合成功)
				dsDesc.BackFace = dsDesc.FrontFace; // 裏面も同じ設定にする

				bool enablePattern[] = { true, true, false };

				D3D11_DEPTH_WRITE_MASK maskPattern[] = {
					D3D11_DEPTH_WRITE_MASK_ALL,
					D3D11_DEPTH_WRITE_MASK_ZERO,
					D3D11_DEPTH_WRITE_MASK_ZERO,
				};

				for (int i = 0; i < DepthStencilSetting::Depth_Setting_Max; i++)
				{
					// 深度ステンシル状態を作成
					dsDesc.DepthEnable = enablePattern[i];
					dsDesc.StencilEnable = false;
					dsDesc.DepthWriteMask = maskPattern[i];

					HRESULT hr = d3dDevice->CreateDepthStencilState(&dsDesc, depthStencilSetting[i].GetAddressOf());

					if (FAILED(hr)) {
						DebugLog::OutputToConsole(u8"深度ステンシルの作成に失敗しました");
						return false;
					}
				}

				// 初期状態は深度テスト・書き込みONにする
				SetDepthStencilSetting(DepthEnableON_DepthWriteON);

				return true;
			}


			// -----------------------------------------------------
			// 深度ステンシルの後処理
			// -----------------------------------------------------
			void Uninit()
			{
				for (int i = 0; i < DepthStencilSetting::Depth_Setting_Max; i++)
				{
					depthStencilSetting[i].Reset();
				}
			}


		}


		// *********************************************************************************
		// アルファディザ設定の初期化・後処理 
		// *********************************************************************************
		namespace AlphaDiza {

			// -----------------------------------------------------
			// アルファディザ設定の初期化
			// -----------------------------------------------------
			bool Init()
			{
				// ブレンドステートの設定
				D3D11_BLEND_DESC blendDesc = {};
				blendDesc.AlphaToCoverageEnable = FALSE;
				blendDesc.IndependentBlendEnable = FALSE;
				blendDesc.RenderTarget[0].BlendEnable = TRUE;
				blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
				blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
				blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
				blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

				D3D11_BLEND blend[AlphaDizaSetting::Blend_Setting_Max][2] = {
					{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},
					{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},
					{D3D11_BLEND_ONE, D3D11_BLEND_ONE},
					{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},
					{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},
					{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE},
				};

				// ブレンド設定の作成
				for (int i = 0; i < AlphaDizaSetting::Blend_Setting_Max; i++)
				{
					blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
					blendDesc.RenderTarget[0].DestBlend = blend[i][1];

					HRESULT hr = d3dDevice->CreateBlendState(&blendDesc, alphaDizaSetting[i].GetAddressOf());

					if (FAILED(hr)) {
						DebugLog::OutputToConsole(u8"アルファディザ設定の作成に失敗しました");
						return false;
					}
				}

				// 初期状態は標準アルファブレンドを設定する
				SetAlphaDizaSetting(AlphaDizaSetting::Blend_Alpha);

				return true;
			}


			// -----------------------------------------------------
			// アルファディザ設定の後処理
			// -----------------------------------------------------
			void Uninit()
			{
				for (int i = 0; i < AlphaDizaSetting::Blend_Setting_Max; i++)
				{
					alphaDizaSetting[i].Reset();
				}
			}
		}
	}
}
