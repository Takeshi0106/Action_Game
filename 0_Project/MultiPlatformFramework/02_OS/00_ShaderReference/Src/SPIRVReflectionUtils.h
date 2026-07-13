#pragma once

// =========================================
// 【補助関数概要】
// SPIR-Vリファレクションを自作リファレクションに
// 変換する関数
// =========================================
// ヘッダー
#include "RefarencesData.h"

// 前方宣言
struct SpvReflectShaderModule;

// 名前空間
namespace SPIRV::ReflectionUtils
{
	// SPIR-Vリフレクション構造体を自作のリファレクション構造体に変換する関数
	bool ConvertSpvReflectToSelfRefData(
		const SpvReflectShaderModule& _spvModule,
		SelfReflectionInfo& _selfRefInfo);
}

