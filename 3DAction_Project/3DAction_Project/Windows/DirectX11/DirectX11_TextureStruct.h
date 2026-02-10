#pragma once

// ==========================================
// 【構造体概要】
// テクスチャ関連をまとめた構造体
// ==========================================

// ==========================================
// ヘッダー
// ==========================================
// テクスチャデータヘッダー
#include "DirectX11_Texture2DData.h"
// View関連ヘッダー
#include "DirectX11_ViewData.h"
// サンプラーデータヘッダー
#include "DirectX11_SamplerData.h"


// ==========================================
// 構造体
// ==========================================
struct DirectX11_TextureStruct
{
	// 2Dテクスチャデータ
	const DirectX11_Texture2DData* texture2DData = nullptr;
	// シェーダーリソースビュー
	const DirectX11_SRVData* srvData = nullptr;
	// レンダーターゲットビュー
	const DirectX11_RTVData* rtvData = nullptr;
	// 深度ステンシルビュー
	const DirectX11_DSVData* dsvData = nullptr;
	// サンプラーデータ
	const DirectX11_SamplerData* samplerData = nullptr;
};
