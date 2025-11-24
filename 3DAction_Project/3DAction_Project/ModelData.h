#pragma once

// ===============================
// 【クラス概要】
// メッシュ・モデル情報構造体定義
// アシンプから読み込んだモデルのメタデータ
// ===============================


// ===============================
// ヘッダー
// ===============================
// 頂点ヘッダー
#include "Vertex.h"
// 他のプラットフォームでも使用できるように
#include <cstdint>
// 配列
#include <vector>
// メッシュマテリアル情報
#include "MaterialData.h"


// ===============================
// クラス
// ===============================
// メッシュ情報(リサイズして使用してください)
struct MeshData
{
	// 頂点情報
	std::vector<Vertex> vertices;
	// インデックス
	std::vector<uint32_t> indices;
	// マテリアルインデックス
	uint16_t materialID = UINT16_MAX;
};

// モデルデータ(1モデルが持つ情報)
struct ModelData
{
	// メッシュ情報配列
	std::vector<MeshData> meshDataArray;
	// マテリアル配列
	std::vector<MeshMaterialData> materialDataArray;
};
