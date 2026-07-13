#pragma once

// ==================================================
// [クラス概要]
// DirectX11のレンダーステートを管理するクラス
// ==================================================

// =================================================
// ヘッダー
// =================================================
// DirectX
#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")
#include <wrl/client.h>
// 独自ヘッダー
#include "DepthStencilSetting.h"
#include "CullingSetting.h"
#include "FillModeSetting.h"
#include "AlphaDizaSetting.h"

// =================================================
// クラス
// =================================================
class DirectX11_RendererStateManager final
{
private:
	// -----------------------------------------------
	// メンバー変数
	// -----------------------------------------------
	// DirectXのデバイスコンテキスト
	ID3D11DeviceContext* m_Context = nullptr;

	// 描画設定
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_RasterStates[(size_t)CullingSetting::Culling_Setting_Max * (size_t)FillModeSetting::FillMode_Max];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_DepthStates[DepthStencilSetting::Depth_Setting_Max];
	Microsoft::WRL::ComPtr<ID3D11BlendState> m_BlendStates[AlphaDizaSetting::Blend_Setting_Max];

	// -----------------------------------------------
	// 関数
	// ------------------------------------------------
	// 描画設定の作成
	bool CreateDrawSetting(ID3D11Device* _device);
	// 深度ステンシル
	bool CreateDepthStencilSetting(ID3D11Device* _device);
	// アルファディザ
	bool CreateAlphaDizaSetting(ID3D11Device* _device);

	// 削除関数
	// 描画設定の削除
	void DeleteDrawSetting();
	// 深度ステンシル
	void DeleteDepthStencilSetting();
	// アルファディザ
	void DeleteAlphaDizaSetting();

public:
	// コンストラクタ・デストラクタ
	DirectX11_RendererStateManager() = default;
	~DirectX11_RendererStateManager() = default;

	// 初期化・後処理
	bool Init(ID3D11DeviceContext* _context, ID3D11Device* _device);
	void Uninit();

	// セッター
	// カリング・塗りつぶし設定
	void SetDrawSetting(
		CullingSetting _culling = CullingSetting::Back_Culling,
		FillModeSetting _fillMode = FillModeSetting::Solid);
	// 深度ステンシル設定
	void SetDepthStencilSetting(
		DepthStencilSetting _setting = DepthStencilSetting::DepthEnableON_DepthWriteOFF);
	// アルファディザ設定
	void SetAlphaDizaSetting(
		AlphaDizaSetting _setting = AlphaDizaSetting::Blend_Alpha);
};
