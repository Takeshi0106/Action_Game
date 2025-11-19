
// ===============================
// ヘッダー
// ===============================
// 必須ヘッダー
#include "ModelManager.h"
// モデルデータ
#include "ModelData.h"
// ログ出力
#include "ReportMessage.h"


// ===============================
// 初期化
// ===============================
bool ModelManager::Init(BaseDrawManager& drawManager)
{
    Color materialData[3] = {};

	// モデルの定数バッファ作成
    if (!drawManager.CreateConstantBuffer(
        m_MaterialCBName.c_str(),
        &materialData,
        sizeof(materialData),
        BufferUsage::Dynamic,
        CPUAccess::Write))
    {
        ErrorLog::OutputToConsole("モデルマテリアル用定数バッファの作成に失敗しました");
        return false;
    }

    return true;
}


// ===============================
// モデルマネージャーに登録
// ===============================
void ModelManager::RegisterModel(const std::string modelName, const ModelData& modelData)
{
    // データを作成
	ModelManagerData modelManagerData;

	// メッシュの数だけメッシュマテリアルID配列をリサイズ
    modelManagerData.meshMaterialIDs.resize(modelData.meshDataArray.size());

    // メッシュのマテリアル番号を登録
	for (int i = 0; i < modelData.meshDataArray.size(); i++)
    {
        modelManagerData.meshMaterialIDs[i] = modelData.meshDataArray[i].materialID;
    }
    // マテリアルデータを登録
    modelManagerData.materialData = modelData.materialDataArray;
    // モデルデータを登録
	m_Models[modelName] = modelManagerData;
}


// ===============================
// モデルデータゲッター
// ===============================
const ModelManagerData* ModelManager::GetModelData(const std::string modelName)
{
    // 探す
    auto it = m_Models.find(modelName);

    if (it != m_Models.end())
    {
        return &(it->second);
    }

    ErrorLog::OutputToConsole(std::string(modelName+" が見つかりませんでした").c_str());
    return nullptr;
}


// ================================
// モデルデータがあるかのチェック
// ================================
bool ModelManager::CheckModelData(const std::string modelName)
{
    return m_Models.find(modelName) != m_Models.end();
}
