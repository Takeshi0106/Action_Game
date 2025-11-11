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
// 文字列
#include <string>
// 外部ファイルにアセット名ログ出力
#include "AssetLogger.h"


// =====================================
// クラス
// =====================================
class IndexBufferManager
{
private:
	// 頂点バッファメンバー配列
	std::unordered_map<std::string, std::unique_ptr<IndexBufferData>> m_IndexBuffers;
	AssetLogger m_Logger = { "IndexBuffers.txt" };

public:
	// コンストラクタ・デストラクタ
	IndexBufferManager() {}
	~IndexBufferManager() = default;

	// バッファ作成
	bool CreateIndexBuffer(
		std::string name,
		ID3D11Device* device,
		const int* indexData,
		int indexNumber);

	// インデックスバッファを探して、戻り値で返す
	IndexBufferData* GetFindIndexData(const std::string& name) const;
	// インデックスバッファがあるかのチェック
	bool Exists(const std::string& name) const;

	// 後処理
	void ReleaseAllIndexBuffers();
};
