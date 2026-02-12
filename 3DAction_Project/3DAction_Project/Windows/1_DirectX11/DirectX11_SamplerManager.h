#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用サンプラーマネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// サンプラーデータ
#include "DirectX11_SamplerData.h"
// 自作サンプラーデーター
#include "../../SamplerSetting.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"


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
	TemplateManager<DirectX11_SamplerData, SamplerDesc, SamplerDescHash> m_Samplers;


public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_SamplerManager() = default;
	~DirectX11_SamplerManager() = default;

	// ------------------------------------------
	// サンプラー作成関数
	// ------------------------------------------
	Handle SamplerStateCreateOnGet(
		ID3D11Device* _device,
		const D3D11_SAMPLER_DESC& _dxDesc,
		const SamplerDesc& _myDesc);

	// ------------------------------------------
	// サンプラーチェック
	// ------------------------------------------
	bool Exists(const SamplerDesc& _myDesc) {
		return m_Samplers.Exists(_myDesc);
	}

	// ------------------------------------------
	// ハンドル取得
	// ------------------------------------------
	Handle GetHandle(const SamplerDesc& _myDesc) {
		return m_Samplers.GetHandle(_myDesc);
	}

	// ------------------------------------------
	// サンプラー取得関数
	// ------------------------------------------
	const DirectX11_SamplerData* GetSamplerState(const Handle& _handle) {
		return m_Samplers.GetData(_handle);
	}

	// ------------------------------------------
	// サンプラー削除
	// ------------------------------------------
	void ReleaseSampler(const Handle& _handle) {
		m_Samplers.Remove(_handle);
	}

	// ------------------------------------------
	// サンプラーすべて削除
	// ------------------------------------------
	void ReleaseAllSampler() {
		m_Samplers.ALLClear();
	}

};

