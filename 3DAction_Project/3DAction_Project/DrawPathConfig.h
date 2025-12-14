#pragma once

// ===================================
// 【定義概要】
// 描画情報があるパスを定義するヘッダーファイル
// ===================================


// ===================================
// 定義
// ===================================
struct DrawPathConfig
{
	// シェーダーバイナリパス (.cso)
    const char* shaderBinaryPath;
	// シェーダーソースパス (.hlsl)
    const char* shaderSourcePath;

    // シェーダーリフレクションパス
    const char* shaderReflectionPath;

	// テクスチャパス
    const char* texturePath;
	// モデルパス
    const char* objModelPath;
};
