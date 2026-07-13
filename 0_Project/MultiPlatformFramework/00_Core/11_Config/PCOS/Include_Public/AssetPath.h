#pragma once

// ==================================
// 【概要」
// アセットパスの定義
// ==================================
#include "String.h"

namespace AssetPath
{
	// アセットパスの定義
    const String kAssetPath = PROJECT_ASSET_PATH;

	// 各アセットのパス
    constexpr String kShader = u8"Shader";
    constexpr String kTexture = u8"Texture";
    constexpr String kModel = u8"Model";
    constexpr String kDll = u8"Dll";
}
