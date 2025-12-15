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
struct ModelHandle
{
	// メッシュ頂点バッファハンドル配列
	std::vector<Handle> meshVertexBufferHandles = {};
	std::vector<Handle> meshIndexBufferHandles = {};
	// マテリアルハンドル
	std::vector<Handle> materialHandle = {};
	// テクスチャハンドル
	std::vector<TextureHandle> textureHandle = {};
};
