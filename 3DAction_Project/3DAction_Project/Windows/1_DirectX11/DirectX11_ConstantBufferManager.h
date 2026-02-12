#pragma once

// ==============================================
// 【クラス概要】
// DirectX11用定数バッファマネージャー
// APIObject を管理するマネージャー
// ==============================================


// ==============================================
// ヘッダー
// ==============================================
// 定数バッファデータ
#include "DirectX11_ConstantBufferData.h"
// データ管理テンプレートヘッダー
#include "../../TemplateManager.h"
// 文字列ヘッダー
#include "../../Hashed_String.h"


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
	TemplateManager<DirectX11_ConstantBufferData> m_ConstantBuffers;

public:
	// ------------------------------------------
	// コンストラクタ・デストラクタ
	// ------------------------------------------
	DirectX11_ConstantBufferManager() = default;
	~DirectX11_ConstantBufferManager() = default;


	// ------------------------------------------
	// 定数バッファ作成関数
	// ------------------------------------------
	Handle ConstantBufferCreateOnGet(
		ID3D11Device* _device,
		const Hashed_String& _cbName,
		const D3D11_BUFFER_DESC& _desc,
		const D3D11_SUBRESOURCE_DATA* _initData);

	// ------------------------------------------
	// 定数バッファ取得関数
	// ------------------------------------------
	const DirectX11_ConstantBufferData* GetConstantBuffer(const Handle& _handle) {
		return m_ConstantBuffers.GetData(_handle);
	}

	// ------------------------------------------
	// 定数バッファ削除
	// ------------------------------------------
	void ReleaseConstantBuffer(const Handle& _handle) {
		m_ConstantBuffers.Remove(_handle);
	}

	// ------------------------------------------
	// 全ての定数バッファ削除
	// ------------------------------------------
	void ReleaseAllConstantBuffer() {
		m_ConstantBuffers.ALLClear();
	}
};
