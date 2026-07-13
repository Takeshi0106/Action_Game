// ヘッダー
#include <Windows.h>
#include "DirectX11_DeviceManager.h"
#include "ReportMessage.h"

// ========================================
// DirectX11の初期化
// ========================================
bool DirectX11_DeviceManager::Init(uint32_t _width, uint32_t _height, IWindowHandle& _windowHandle)
{
	// ハンドルに変換
	HWND hWnd = static_cast<HWND>(_windowHandle.GetWindowHandle());

	// 画面の幅と高さを保存
	m_Width = _width;
	m_Height = _height;

	// 初期化の 成功、失敗 を受け取る
	HRESULT hr;
	// スワップチェイン作成成功時に使われたDirectXのバージョンを入れる
	D3D_FEATURE_LEVEL level;

	// デバイス作成用のフラグ
	unsigned int createDeviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ時にデバッグレイヤーを有効可
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
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

	swapChainDesc.BufferCount = 2;                                                      // ダブルバッファ
	swapChainDesc.BufferDesc.Width = m_Width;                                           // 画面の縦幅 今はウィンドウサイズと同じにしている
	swapChainDesc.BufferDesc.Height = m_Height;                                         // 画面の横幅 今はウィンドウサイズと同じにしている
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;                       // RGBA 各8ビット 0.0~1.0に正規化　＊一般的でどの環境でも動きやすい
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;                                 // リフレッシュレート　分母 (0の場合,OSに任せる)
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;                               // リフレッシュレート　分子 (*ウィンドウモードの時は適用されない)
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;                        // バックバッファの使用用途
	swapChainDesc.OutputWindow = hWnd;                                                  // 描画するウィンドウのハンドルを渡す
	swapChainDesc.SampleDesc.Count = 1;                                                 // マルチサンプリング　アンチエイリアス 1は無効
	swapChainDesc.SampleDesc.Quality = 0;                                               // 品質レベル　大きい値ほど良くなる(フォーマットとサンプリング数で上限が決まる)
	swapChainDesc.Windowed = TRUE;                                                      // ウィンドウモード (FALSEにするとフルスクリーンモードになる)
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;                           // 推奨されている

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
			m_SwapChain.GetAddressOf(),     // 成功時に代入
			m_Device.GetAddressOf(),        // 成功時に代入
			&level,                         // 実際に使用されたDirectXのバージョンが入る
			m_DeviceContext.GetAddressOf()  // 成功時に代入される
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

// ========================================
// DirectX11の終了
// ========================================
void DirectX11_DeviceManager::Uninit()
{
	// バインド解除
	m_DeviceContext->ClearState();
	m_DeviceContext->Flush();

	// デバイスコンテキスト・スワップチェイン解放
	m_DeviceContext.Reset();
	m_SwapChain.Reset();

#if defined(DEBUG) || defined(_DEBUG)
	// デバッグ表示用
	Microsoft::WRL::ComPtr<ID3D11Debug> debug;
	if (SUCCEEDED(m_Device.As(&debug)))
	{
		debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL | D3D11_RLDO_IGNORE_INTERNAL);
	}
	debug.Reset();
#endif

	// デバイスの解放
	m_Device.Reset();
}
