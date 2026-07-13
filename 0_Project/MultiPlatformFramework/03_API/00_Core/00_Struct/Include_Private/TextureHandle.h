#pragma once

// =======================================
// 【構造体概要】
// テクスチャハンドル
// =======================================
#include "Handle.h"

// 構造体定義
struct TextureHandle
{
	// テクスチャハンドル
	Handle textureHandle = {};

	// シェーダーリソースビューのハンドル
	Handle srvHandle = {};
	// 2Dレンダーターゲットビューのハンドル
	Handle rtvHandle = {};
	// 深度ステンシルビューのハンドル
	Handle dsvHandle = {};
};
