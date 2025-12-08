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
// 標準整数ヘッダー
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
	// データを保存する配列
	std::vector<T> m_DataArray;
	// 名前からインデックスを取得する配列
	std::unordered_map<std::string, uint32_t> m_NameToIndexMap;
	// 次に追加するインデックス
	uint32_t m_NextIndex = 0;

public:
	TemplateManager() = default;
	~TemplateManager() = default;

	// ==============================
	// データ追加
	// ==============================
	uint32_t AddData(const std::string& name, const T& data)
	{
		// 名前が既に存在する場合は追加しない
		if (m_NameToIndexMap.find(name) != m_NameToIndexMap.end()) {
			return m_NameToIndexMap[name];
		}

		// データを配列に追加
		m_DataArray.push_back(data);

		// 名前とインデックスをマップに追加
		m_NameToIndexMap[name] = m_NextIndex;

		// インデックスを更新
		m_NextIndex++;

		// 使用した添え字のインデックスを返す
		return m_NextIndex - 1;
	}


	// ================================
	// データ取得
	// ================================
	T* GetData(const uint32_t& ID)
	{
#if defined(DEBUG) || defined(_DEBUG)
		// デバッグチェック
		if (ID >= m_DataArray.size()) {
			ErrorLog::OutputToConsole("TemplateManager: 指定されたIDのデータが存在しません");
			return nullptr;
		}
#endif

		return &m_DataArray[ID];
	}
	

	// ================================
	// 存在チェック
	// ================================
	bool Exists(const std::string& name) const
	{
		return m_NameToIndexMap.find(name) != m_NameToIndexMap.end();
	}


	// ================================
	// データ削除
	// ================================
	void ALLClear()
	{
		m_DataArray.clear();
		m_NameToIndexMap.clear();
		m_NextIndex = 0;
	}

};

