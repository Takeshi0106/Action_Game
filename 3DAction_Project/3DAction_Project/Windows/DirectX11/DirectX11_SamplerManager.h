#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用サンプラーマネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// DirectXヘッダー
#include <d3d11.h>
// スマートポインタ
#include <wrl/client.h>
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../UTF8_String.h"


// ==============================================
// クラス
// ==============================================
class DirectX11_SamplerManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// サンプラー管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11SamplerState>> m_Samplers;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_SamplerManager() = default;
	~DirectX11_SamplerManager() = default;


	// ------------------------------------------
	// サンプラー作成関数
	// ------------------------------------------
	const Handle SamplerStateCreate(
		ID3D11Device* _device,
		D3D11_SAMPLER_DESC& desc,
		const String& _name);


	// ------------------------------------------
	// サンプラー取得関数
	// ------------------------------------------
	ID3D11SamplerState* GetSamplerState(const Handle& _handle);


	// ------------------------------------------
	// サンプラーチェック
	// ------------------------------------------
	bool Exists(const String& _name) const {
		return m_Samplers.Exists((Hashed_String)_name);
	}


	// ------------------------------------------
	// サンプラーハンドル取得
	// ------------------------------------------
	Handle GetHandle(const String& _name) const {
		return m_Samplers.GetHandle((Hashed_String)_name);
	}
};

