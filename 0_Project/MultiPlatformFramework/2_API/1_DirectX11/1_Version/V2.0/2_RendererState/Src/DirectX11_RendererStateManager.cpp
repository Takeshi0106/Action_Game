// ===========================================
// ヘッダー
// ===========================================
#include "DirectX11_RendererStateManager.h"
#include "ReportMessage.h"

// ===========================================
// 初期化
// ===========================================
bool DirectX11_RendererStateManager::Init(ID3D11DeviceContext* _context, ID3D11Device* _device)
{
	// デバイスコンテキストの保存
	m_Context = _context;

	// 描画設定の作成
	if (!CreateDrawSetting(_device)) {
		ErrorLog::OutputToConsole(u8"描画設定の作成に失敗しました");
		return false;
	}
	// 深度ステンシル設定の作成
	if (!CreateDepthStencilSetting(_device)) {
		ErrorLog::OutputToConsole(u8"深度ステンシル設定の作成に失敗しました");
		return false;
	}
	// アルファディザ設定の作成
	if (!CreateAlphaDizaSetting(_device)) {
		ErrorLog::OutputToConsole(u8"アルファディザ設定の作成に失敗しました");
		return false;
	}
	return true;
}

// ===========================================
// 後処理
// ===========================================
void DirectX11_RendererStateManager::Uninit()
{
	// アルファディザ設定の解放
	DeleteAlphaDizaSetting();
	// 深度ステンシル設定の解放
	DeleteDepthStencilSetting();
	// 描画設定の解放
	DeleteDrawSetting();
}

// ===========================================
// カリング設定のセッター
// ===========================================
void DirectX11_RendererStateManager::SetDrawSetting(CullingSetting _culling, FillModeSetting _fillMode)
{
	// 描画設定のセッター
	m_Context->RSSetState(m_RasterStates[static_cast<int>(_culling) * 2 + static_cast<int>(_fillMode)].Get());
}

// ===========================================
// 深度ステンシル設定のセッター
// ===========================================
void DirectX11_RendererStateManager::SetDepthStencilSetting(DepthStencilSetting _setting)
{
	// 深度ステンシル設定のセッター
	m_Context->OMSetDepthStencilState(m_DepthStates[static_cast<int>(_setting)].Get(), 0);
}

// ===========================================
// アルファディザ設定のセッター
// ===========================================
void DirectX11_RendererStateManager::SetAlphaDizaSetting(AlphaDizaSetting _setting)
{
	// アルファディザ設定のセッター
	m_Context->OMSetBlendState(m_BlendStates[static_cast<int>(_setting)].Get(), nullptr, 0xffffffff);
}

// ===========================================
// カリングの描画設定の作成関数
// ===========================================
bool DirectX11_RendererStateManager::CreateDrawSetting(ID3D11Device* _device)
{
	// カリング設定
	D3D11_RASTERIZER_DESC rasterDesc = {};
	rasterDesc.FillMode = D3D11_FILL_SOLID;      // ポリゴンの塗りつぶし方法 (線画・塗りつぶし)
	rasterDesc.CullMode = D3D11_CULL_BACK;       // カリング方法 (裏面をカリング)
	rasterDesc.FrontCounterClockwise = false;    // 頂点の並び順 (時計回りが表面)
	rasterDesc.DepthClipEnable = true;           // 深度クリッピングを有効化

	// カリング設定パターン
	D3D11_CULL_MODE cullModePattern[] = {
		D3D11_CULL_NONE,   // カリング無し
		D3D11_CULL_FRONT,  // 表面カリング
		D3D11_CULL_BACK };   // 裏面カリング
	// 塗りつぶし設定パターン
	D3D11_FILL_MODE fillModePattern[] = {
		D3D11_FILL_SOLID,     // 塗りつぶし
		D3D11_FILL_WIREFRAME }; // ワイヤーフレーム

	// カリング設定の作成
	for (size_t i = 0; i < (size_t)CullingSetting::Culling_Setting_Max * (size_t)FillModeSetting::FillMode_Max; i++)
	{
		// カリング設定を作成
		rasterDesc.CullMode = cullModePattern[i / (size_t)FillModeSetting::FillMode_Max];
		rasterDesc.FillMode = fillModePattern[i % (size_t)FillModeSetting::FillMode_Max];

		HRESULT hr = _device->CreateRasterizerState(&rasterDesc, m_RasterStates[i].GetAddressOf());

		if (FAILED(hr)) {
			DebugLog::OutputToConsole(u8"カリング設定の作成に失敗しました");
			return false;
		}
	}

	// 初期状態は裏面カリングにする
	SetDrawSetting(CullingSetting::Back_Culling, FillModeSetting::Solid);
	return true;
}

// ===========================================
// 深度ステンシル設定の作成関数
// ===========================================
bool DirectX11_RendererStateManager::CreateDepthStencilSetting(ID3D11Device* _device)
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

	// 深度ステンシル設定パターン
	bool enablePattern[] = { true, true, false };
	// 深度書き込み設定パターン
	D3D11_DEPTH_WRITE_MASK maskPattern[] = {
		D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_DEPTH_WRITE_MASK_ZERO,
		D3D11_DEPTH_WRITE_MASK_ZERO};

	for (int i = 0; i < DepthStencilSetting::Depth_Setting_Max; i++)
	{
		// 深度ステンシル状態を作成
		dsDesc.DepthEnable = enablePattern[i];
		dsDesc.StencilEnable = false;
		dsDesc.DepthWriteMask = maskPattern[i];

		HRESULT hr = _device->CreateDepthStencilState(&dsDesc, m_DepthStates[i].GetAddressOf());

		if (FAILED(hr)) {
			DebugLog::OutputToConsole(u8"深度ステンシルの作成に失敗しました");
			return false;
		}
	}

	// 初期状態は深度テスト・書き込みONにする
	SetDepthStencilSetting(DepthEnableON_DepthWriteON);
	return true;
}

// ===========================================
// アルファディザ設定の作成関数
// ===========================================
bool DirectX11_RendererStateManager::CreateAlphaDizaSetting(ID3D11Device* _device)
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

	// アルファブレンド設定パターン
	D3D11_BLEND blend[AlphaDizaSetting::Blend_Setting_Max][2] = {
		{D3D11_BLEND_ONE, D3D11_BLEND_ZERO},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_SRC_ALPHA},
		{D3D11_BLEND_ONE, D3D11_BLEND_ONE},
		{D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_ONE},
		{D3D11_BLEND_ZERO, D3D11_BLEND_INV_SRC_COLOR},
		{D3D11_BLEND_INV_DEST_COLOR, D3D11_BLEND_ONE}};

	// ブレンド設定の作成
	for (int i = 0; i < AlphaDizaSetting::Blend_Setting_Max; i++)
	{
		blendDesc.RenderTarget[0].SrcBlend = blend[i][0];
		blendDesc.RenderTarget[0].DestBlend = blend[i][1];

		HRESULT hr = _device->CreateBlendState(&blendDesc, m_BlendStates[i].GetAddressOf());

		if (FAILED(hr)) {
			DebugLog::OutputToConsole(u8"アルファディザ設定の作成に失敗しました");
			return false;
		}
	}

	// 初期状態は標準アルファブレンドを設定する
	SetAlphaDizaSetting(AlphaDizaSetting::Blend_Alpha);
	return true;
}

// ===========================================
// 描画設定の解放関数
// ===========================================
void DirectX11_RendererStateManager::DeleteDrawSetting()
{
	for (size_t i = 0; i < (size_t)CullingSetting::Culling_Setting_Max * (size_t)FillModeSetting::FillMode_Max; i++)
	{
		m_RasterStates[i].Reset();
	}
}
// ===========================================
// 深度ステンシル設定の解放関数
// ===========================================
void DirectX11_RendererStateManager::DeleteDepthStencilSetting()
{
	for (size_t i = 0; i < DepthStencilSetting::Depth_Setting_Max; i++)
	{
		m_DepthStates[i].Reset();
	}
}
// ===========================================
// アルファディザ設定の解放関数
// ===========================================
void DirectX11_RendererStateManager::DeleteAlphaDizaSetting()
{
	for (size_t i = 0; i < AlphaDizaSetting::Blend_Setting_Max; i++)
	{
		m_BlendStates[i].Reset();
	}
}
