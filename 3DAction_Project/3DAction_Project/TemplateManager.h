#pragma once


// ===================================
// 【クラス概要】
// テンプレートを使用したマネージャー
// vectorで実際のデータを保存し、
// unordered_mapで名前からインデックスを取得する
// ===================================


// ===================================
// ヘッダー
// ===================================
// 配列用ヘッダー
#include <vector>
#include <unordered_map>
// 文字列ヘッダー
#include <string>
// 固定長整数ヘッダー
#include <cstdint>
// ログ出力用ヘッダー
#include "ReportMessage.h"


// ===================================
// クラス
// ===================================
// テンプレートマネージャークラス
template<typename T>
class TemplateManager
{
private:
	// --------------------------------
	// メンバー変数
	// --------------------------------
	// データを保存する配列
	std::vector<T> m_DataArray;
	// IDとデータ保存配列の添え字を紐づける配列
	std::unordered_map<uint32_t, uint32_t> m_IDToIndex;
	// 添え字からIDに逆変換する配列 (削除時に更新するために使用する)
	std::vector<uint32_t> m_IDArray;

	// 名前からIDを取得する配列
	std::unordered_map<std::string, uint32_t> m_NameToIDMap;

	// 次に追加するインデックス
	uint32_t m_NextID = 0;

public:
	TemplateManager() = default;
	~TemplateManager() = default;

	// ==============================
	// リサーブ
	// ==============================
	void Reserve(const uint32_t& size)
	{
		// 各配列のリザーブ
		m_DataArray.reserve(size);
		m_IDArray.reserve(size);
		m_IDToIndex.reserve(size);
		m_NameToIDMap.reserve(size);
	}


	// ==============================
	// データ追加
	// ==============================
	uint32_t AddData(const std::string& name, const T& data)
	{
		// 名前が既に存在する場合はIDを返す
		if (m_NameToIDMap.find(name) != m_NameToIDMap.end()) {
			return m_NameToIDMap[name];
		}

		// 新しいIDを取得
		uint32_t currentID = m_NextID;
		m_NextID++;

		// データを配列に追加.
		m_DataArray.push_back(data);
		// 添え字を配列に追加
		m_IDArray.push_back(currentID);
		// IDをキーに添え字をマップに追加
		m_IDToIndex[currentID] = static_cast<uint32_t>(m_DataArray.size() - 1);
		// 名前とインデックスをマップに追加
		m_NameToIDMap[name] = currentID;

		// IDを返す
		return currentID;
	}


	// ================================
	// データ取得
	// ================================
	T* GetData(const uint32_t& ID)
	{
		// 存在チェック
		auto it = m_IDToIndex.find(ID);

		if (it == m_IDToIndex.end()) {
			return nullptr;
		}

		return &m_DataArray[it->second];
	}
	

	// ================================
	// 存在チェック
	// ================================
	bool Exists(const std::string& name) const
	{
		return m_NameToIDMap.find(name) != m_NameToIDMap.end();
	}


	// ================================
	// 削除
	// ================================
	void Remove(uint32_t entityID)
	{
		// 存在チェック
		auto it = m_IDToIndex.find(entityID);

		if (it == m_IDToIndex.end()) { return; }
		
		// 添え字取得
		uint32_t index = it->second;
		// 最後の要素の添え字取得
		uint32_t lastIndex = static_cast<uint32_t>(m_DataArray.size() - 1);

		// 最後の要素を穴に移動
		m_DataArray[index] = m_DataArray[lastIndex];
		// 添え字も移動
		uint32_t lastID = m_IDArray[lastIndex];

		// ID配列とマップも更新
		m_IDArray[index] = lastID;
		m_IDToIndex[lastID] = index;

		// 配列の最後を削除
		m_DataArray.pop_back();
		m_IDArray.pop_back();
		m_IDToIndex.erase(entityID);

		// 名前マップも削除
		for (auto itName = m_NameToIDMap.begin(); itName != m_NameToIDMap.end(); ++itName)
		{
			if (itName->second == entityID) {
				m_NameToIDMap.erase(itName);
				break;
			}
		}
	}


	// ================================
	// データ削除
	// ================================
	void ALLClear()
	{
		m_DataArray.clear();
		m_IDArray.clear();
		m_IDToIndex.clear();

		m_NameToIDMap.clear();
		m_NextID = 0;
	}

};

