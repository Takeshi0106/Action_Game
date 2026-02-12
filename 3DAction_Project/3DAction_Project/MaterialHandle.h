#pragma once

// ==========================================
// 【構造体概要】
// マテリアルハンドル
// Shader・Texture・Sampler・Materialのハンドルをまとめて管理するハンドル
// 
// のちに複数のテクスチャを管理できるように,
// テクスチャハンドル,サンプラーを配列にする予定
// ==========================================


// ==========================================
// ヘッダー
// ==========================================
// ハンドル構造体
#include "Handle.h"
// マテリアル情報
#include "MaterialData.h"
// 配列保持ヘッダー
#include <vector>


// ==========================================
// 構造体定義
// ==========================================
struct MaterialHandle
{
	// 頂点シェーダーハンドル
	Handle vsShaderHandle = {};
	// ピクセルシェーダーハンドル
	Handle psShaderHandle = {};

	// テクスチャハンドル
	Handle textureHandle = {};
	// サンプラーハンドル
	Handle samplerHandle = {};

	// マテリアルデータ
	MeshMaterialCBData materialCBData = {};
	// マテリアル定数バッファハンドル
	Handle materialCBHandle = {};
};
