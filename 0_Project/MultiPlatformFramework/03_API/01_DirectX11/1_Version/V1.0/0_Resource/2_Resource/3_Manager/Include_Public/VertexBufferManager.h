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
#include "Hashed_String.h"


// ======================================
// クラス
// ======================================
class VertexBufferManager
{
private:
	// 頂点バッファメンバー配列
	std::unordered_map<Hashed_String, std::unique_ptr<VertexBufferData>> m_VertexBuffers;

public:
	// コンストラクタ
	VertexBufferManager() = default;

	// 頂点バッファ作成
	bool CreateVertexBuffer(
		const Hashed_String& name,
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
	bool UpdateVertexBuffer(const Hashed_String& name, ID3D11DeviceContext* context, const void* data, int size);

	// 頂点バッファをバインドして描画する頂点数を返す
	int BindVertexBuffer(const Hashed_String& name, ID3D11DeviceContext* context) const;
	
	// 頂点バッファがあるかのチェック
	bool Exists(const Hashed_String& name) const;

	// 後処理
	void ReleaseAllVertexBuffers();
};
