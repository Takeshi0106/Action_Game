#pragma once

// =====================================
// 【クラス概要】
// 頂点バッファマネージャー
// 
// 作成、管理、削除を担当
// VertexBufferDataに重たいヘッダーがあります
// 
// 【注意】
// 頂点を増やす関数が用意されていますが、
// インデックスバッファのほうに増やす関数がないため
// 使用することはできません
// =====================================


// ======================================
// ヘッダー
// ======================================
// DirectXヘッダー
#include <d3d11.h>        // DirectXのAPIヘッダー
// 頂点バッファデータ
#include "VertexBufferData.h"
// スマートポインターヘッダー
#include <memory>
// 配列のヘッダー
#include <unordered_map>
// バッファ設定ヘッダー
#include "GraphicsEnums.h"
// 文字列
#include <string>
// 外部ファイルにアセット名ログ出力
#include "AssetLogger.h"


// ======================================
// クラス
// ======================================
class VertexBufferManager
{
private:
	// 頂点バッファメンバー配列
	std::unordered_map<std::string, std::unique_ptr<VertexBufferData>> m_VertexBuffers;
	AssetLogger m_Logger = { "VertexBuffers.txt" };

public:
	// コンストラクタ
	VertexBufferManager() = default;
	// デストラクタ
	~VertexBufferManager() { m_Logger.WriteLog(); }

	// 頂点バッファ作成
	bool CreateVertexBuffer(
		const std::string name,
		ID3D11Device* device,
		const void* vertices,
		int vertexCount,
		int vertexMaxCount,
		size_t stride,
		PrimitiveType type = PrimitiveType::TriangleStrip,
		BufferUsage usage = BufferUsage::Dynamic,
		CPUAccess access = CPUAccess::Write
	);

	// 頂点バッファ更新
	bool UpdateVertexBuffer(const std::string& name, ID3D11DeviceContext* context, const void* data, int size);

	// 頂点バッファをバインドして描画する頂点数を返す
	int BindVertexBuffer(const std::string& name, ID3D11DeviceContext* context) const;
	
	// 頂点バッファがあるかのチェック
	bool Exists(const std::string& name) const;

	// 後処理
	void ReleaseAllVertexBuffers();
};
