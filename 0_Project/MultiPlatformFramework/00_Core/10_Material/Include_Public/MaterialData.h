#pragma once

// ==========================
// 【構造体概要】
// メッシュが持つマテリアル情報を入れる構造体
// ==========================


// ==========================
// ヘッダー
// ==========================
// 色情報
#include "Color.h"
// 文字列ヘッダー
#include "Hashed_String.h"


// ==========================
// 構造体
// ==========================
// 定数用情報
struct MeshMaterialCBData
{
	// 反射光
	Color diffuse{};
	// 環境光
	Color ambient{};
	// 鏡面光
	Color specular{};
};

// メッシュのマテリアル情報
struct MeshMaterialData
{
	// 定数バッファ用情報
	MeshMaterialCBData materialCBData;
	// テクスチャの名前(テクスチャマネージャーに登録する名前)
	Hashed_String textureName = Hashed_String(u8"");
};
