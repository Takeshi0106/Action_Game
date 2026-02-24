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
// 描画パス
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

#if defined(DEBUG) || defined(_DEBUG)
// 開発用デバッグパス
struct DevelopmentPath{
    // SPIR-Vバイナリパス
    const String& kSPIRVFolderPath;
	// SPIR-Vリフレクション情報パス
	const String& kSPIRVReflectionInfoFolderPath;
	// SPIR-Vリフレクション情報テキストファイル保存先パス
	const String& kSPIRVReflectionInfoTextPath;
};

#endif
