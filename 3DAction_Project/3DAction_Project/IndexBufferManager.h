#pragma once

// =====================================
// インデックスバッファマネージャー
// 
// 【注意】
// 頂点が増えることを想定していません
// =====================================


// ======================================
// ヘッダー
// ======================================
// DirectXヘッダー
#include <d3d11.h>
// 頂点バッファデータ
#include "IndexBufferData.h"
// スマートポインターヘッダー
#include <memory>
// 配列のヘッダー
#include <unordered_map>
// テンプレートマネージャー
#include "TemplateManager.h"
// 文字列
#include "UTF8_String.h"
#include "Hashed_String.h"
// 必須ヘッダー
#include <cstdint>


// =====================================
// クラス
// =====================================
class IndexBufferManager
{
private:
	// インデックスバッファ
	TemplateManager<IndexBufferData> m_IndexBufferr;

	// 頂点バッファメンバー配列
	std::unordered_map<Hashed_String, std::unique_ptr<IndexBufferData>> m_IndexBuffers;

public:
	// コンストラクタ・デストラクタ
	IndexBufferManager() {}
	~IndexBufferManager() = default;

	// --------------------------------
	// バッファ作成
	// --------------------------------
	bool CreateIndexBuffer(
		const Hashed_String& name,
		ID3D11Device* device,
		const uint32_t* indexData,
		uint32_t indexNumber);

	// --------------------------------
	// インデックスバッファを探して、戻り値で返す
	// --------------------------------
	uint32_t BindIndexData(const Hashed_String& name, ID3D11DeviceContext* context) const;

	// --------------------------------
	// インデックスバッファがあるかのチェック
	// --------------------------------
	bool Exists(const Hashed_String& name) const;

	// --------------------------------
	// 後処理
	// --------------------------------
	void ReleaseAllIndexBuffers();
};
