#pragma once

// ===================================
// 【定義概要】
// 描画情報があるパスを定義するヘッダーファイル
// ===================================

// ===================================
// ヘッダー
// ===================================
#include "UTF8_String.h"


// ===================================
// 定義
// ===================================
struct DrawPathConfig
{
	// シェーダーバイナリパス (.cso)
    const String& shaderBinaryPath;
	// シェーダーソースパス (.hlsl)
    const String& shaderSourcePath;

    // シェーダーリフレクションパス
    const String& shaderReflectionPath;

	// テクスチャパス
    const String& texturePath;
	// モデルパス
    const String& objModelPath;
};
