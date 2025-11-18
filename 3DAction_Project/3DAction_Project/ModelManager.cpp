
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
// モデルマネージャーに登録
// ===============================
void ModelManager::RegisterModel(const std::string modelName, const ModelData& modelData)
{

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
