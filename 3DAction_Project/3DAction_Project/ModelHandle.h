#pragma once

// ===============================================
// 構造体概要
// モデルハンドル構造体
// ===============================================


// ===============================================
// ヘッダー
// ===============================================
// ハンドル構造体
#include "Handle.h"
// テクスチャハンドル構造体
#include "TextureHandleh.h"
// 配列保持ヘッダー
#include <vector>


// ===============================================
// 構造体定義
// ===============================================
// メッシュハンドル構造体
struct MeshHandle
{
	// メッシュ頂点バッファハンドル配列
	Handle meshVertexBufferHandles = {};
	Handle meshIndexBufferHandles = {};
	// マテリアルハンドル
	Handle materialHandle = {};
	// テクスチャハンドル
	TextureHandle textureHandle = {};
};

// モデルハンドル構造体
struct ModelHandle
{
	// メッシュハンドル配列
	std::vector<MeshHandle> meshHandles = {};
};
