#pragma once

// ===============================
// 【クラス概要】
// メッシュ・モデル情報構造体定義
// ===============================


// ===============================
// ヘッダー
// ===============================
// 頂点ヘッダー
#include "Vertex.h"
// 他のプラットフォームでも使用できるように
#include <cstdint>
// 色情報
#include "Color.h"
// 文字列
#include <string>
// 配列
#include <vector>


// ===============================
// クラス
// ===============================
// メッシュのマテリアル情報
struct MeshMaterialData 
{
	// 反射光
	Color diffuse;
	// 環境光
	Color ambient;
	// 鏡面光
	Color specular;
	// テクスチャの名前(テクスチャマネージャーに登録する名前)
	std::string textureName = "";
};

// メッシュ情報(リサイズして使用してください)
struct MeshData
{
	// 頂点情報
	std::vector<Vertex> vertices;
	// インデックス
	std::vector<uint32_t> indices;
	// マテリアルインデックス
	uint16_t materialID;
};

// モデルデータ(1モデルが持つ情報)
struct ModelData
{
	// メッシュ情報配列
	std::vector<MeshData> meshDataArray;
	// マテリアル配列
	std::vector<MeshMaterialData> materialDataArray;
};
