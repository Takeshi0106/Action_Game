#pragma once

// =====================================
// 【クラス概要】
// 頂点バッファマネージャー
// 
// 作成、管理、削除を担当
// VertexBufferDataに重たいヘッダーがあります
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

	// 頂点バッファを探して、戻り値で返す
	VertexBufferData* GetFindVertexData(const std::string& name) const;
	// 頂点バッファがあるかのチェック
	bool Exists(const std::string& name) const;

	// 後処理
	void ReleaseAllVertexBuffers();

};

