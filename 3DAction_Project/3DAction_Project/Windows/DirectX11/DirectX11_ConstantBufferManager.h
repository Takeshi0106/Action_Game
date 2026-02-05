#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用定数バッファマネージャー
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
class DirectX11_ConstantBufferManager
{
private:
	// ------------------------------------------
	// メンバー変数
	// ------------------------------------------
	// 定数バッファ管理
	TemplateManager<Microsoft::WRL::ComPtr<ID3D11Buffer>> m_ConstantBuffers;

public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_ConstantBufferManager() = default;
	~DirectX11_ConstantBufferManager() = default;


	// ------------------------------------------
	// 定数バッファ作成関数
	// ------------------------------------------
	const Handle ConstantBufferCreateOnGet(
		ID3D11Device* _device,
		size_t _size,
		D3D11_USAGE _usage,
		D3D11_CPU_ACCESS_FLAG _flag,
		const Hashed_String& _name,
		const void* _data = nullptr);


	// ------------------------------------------
	// 定数バッファ取得関数
	// ------------------------------------------
	ID3D11Buffer* GetConstantBuffer(const Handle& _handle);


	// ------------------------------------------
	// 定数バッファ削除
	// ------------------------------------------
	void ReleaseConstantBuffer(const Handle& _handle);


	// ------------------------------------------
	// 全ての定数バッファ削除
	// ------------------------------------------
	void ReleaseAllConstantBuffer();
};
