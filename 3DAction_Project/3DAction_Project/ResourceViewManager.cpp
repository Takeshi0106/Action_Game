
// ============================
// ヘッダー
// ============================
// 必須ヘッダー
#include "ResourceViewManager.h"
// Dataヘッダー
#include "ResourceViewData.h"


// =============================
// 静的変数
// =============================
std::unordered_map<std::string, std::unique_ptr<SRVData>> ResourceViewManager::m_SRVs;
std::unordered_map<std::string, std::unique_ptr<UAVData>> ResourceViewManager::m_UAVs;
std::unordered_map<std::string, std::unique_ptr<RTVData>> ResourceViewManager::m_RTVs;
std::unordered_map<std::string, std::unique_ptr<DSVData>> ResourceViewManager::m_DSVs;


// =============================
// メンバー関数
// =============================
// ゲッター
SRVData* ResourceViewManager::GetFindSRV(const std::string& name)
{
	auto it = m_SRVs.find(name);

	if (it != m_SRVs.end()) {
		return it->second.get();
	}

	return nullptr;
}
UAVData* ResourceViewManager::GetFindUAV(const std::string& name)
{
	auto it = m_UAVs.find(name);

	if (it != m_UAVs.end()) {
		return it->second.get();
	}

	return nullptr;
}
RTVData* ResourceViewManager::GetFindRTV(const std::string& name)
{
	auto it = m_RTVs.find(name);

	if (it != m_RTVs.end()) {
		return it->second.get();
	}

	return nullptr;
}
DSVData* ResourceViewManager::GetFindDSV(const std::string& name)
{
	auto it = m_DSVs.find(name);

	if (it != m_DSVs.end()) {
		return it->second.get();
	}

	return nullptr;
}
