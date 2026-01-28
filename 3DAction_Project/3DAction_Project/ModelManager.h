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
// 描画マネージャー
#include "BaseDrawManager.h"
// 文字列ヘッダー
#include "UTF8_String.h"
#include "Hashed_String.h"


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
	std::vector<uint32_t> meshMaterialIDs;
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
	std::unordered_map<Hashed_String, ModelManagerData> m_Models;
	// マテリアル情報
	String m_MaterialCBName = u8"Material";

public:
	// コンストラクタ・デストラクタ
	ModelManager() = default;
	~ModelManager() = default;

	bool Init(BaseDrawManager& drawManager);

	// モデルマネージャーに登録
	void RegisterModel(const String& modelName, const ModelData& modelData);
	// モデルデータ取得
	const ModelManagerData* GetModelData(const String& modelName);
	// マテリアル定数バッファ名ゲッター
	const String GetMaterialCBName() const { return m_MaterialCBName; }
	// モデルデータがあるかチェック
	bool CheckModelData(const String& modelName);
};
