#pragma once

// ==============================
// 【クラス概要】
// モデルマネージャー
// モデル描画に必要な情報と
// バッファの名前を知っている
// ==============================


// ==============================
// ヘッダー
// ==============================
// 配列
#include <vector>
// メッシュマテリアル情報
#include "MaterialData.h"
// 型固定整数
#include <cstdint>
// ハッシュ値配列
#include <unordered_map>


// ==============================
// 前方宣言
// ==============================
struct ModelData;


// ==============================
// 構造体
// ==============================
// モデルマネージャーが保持するデータ構造体
struct ModelManagerData
{
	// メッシュのマテリアル番号
	std::vector<uint16_t> meshMaterialIDs;
	// マテリアル配列
	std::vector<MeshMaterialData> materialData;
};


// ==============================
// クラス
// ==============================
class ModelManager
{
private:
	// モデルデータ配列
	std::unordered_map<std::string, ModelManagerData> m_Models;

public:
	// コンストラクタ・デストラクタ
	ModelManager() = default;
	~ModelManager() = default;

	// モデルマネージャーに登録
	void RegisterModel(const std::string modelName, const ModelData& modelData);
	// モデルデータ取得
	const ModelManagerData* GetModelData(const std::string modelName);
	// モデルデータがあるかチェック
	bool CheckModelData(const std::string modelName);
};
