#pragma once


// ===================================
// 【クラス概要】
// テンプレートを使用したマネージャー
// vectorで実際のデータを保存し、
// unordered_mapで名前からハンドルを返す
// resizeで配列を確保すると効率が良くなる
// ===================================


// ===================================
// ヘッダー
// ===================================
// 配列用ヘッダー
#include <vector>
#include <unordered_map>
// 文字列ヘッダー
#include <string>
// ハンドル構造体
#include "Handle.h"


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
	std::vector<T> m_Datas;
	// 世代を入れる配列
	std::vector<uint32_t> m_Generations;
	// オブジェクトがいくつハンドルを参照しているかを保存する配列
	std::vector<uint32_t> m_RefHandleCounts;
	// 何も入っていない空の添え字を保存する配列
	std::vector<uint32_t> m_FreeIndexs;

	// 名前からハンドルを取得する配列 (複数制作しない用)
	std::unordered_map<std::string, Handle> m_NameToHandleMap;
	// インデックスから名前を取得する配列 (複数制作しない用)
	std::vector<std::string> m_IndexToNames;

public:
	TemplateManager() = default;
	~TemplateManager() = default;

	// ==============================
	// 配列をリザーブする
	// ==============================
	void Reserve(const uint32_t& size)
	{
		// 各配列をリザーブ
		m_Datas.reserve(size);
		m_Generations.reserve(size);
		m_RefHandleCounts.reserve(size);
		m_FreeIndexs.reserve(size);

		m_NameToHandleMap.reserve(size);
		m_IndexToNames.reserve(size);
	}


	// ==============================
	// データ追加
	// ==============================
	Handle AddData(const std::string& name, const T& data)
	{
		// 名前が既に存在する場合はハンドルを返す
		auto it = m_NameToHandleMap.find(name);

		if (it != m_NameToHandleMap.end()) 
		{
			// 参照カウントを増やす
			m_RefHandleCounts[it->second.index]++;
			return it->second;
		}

		// ハンドル
		Handle handle;

		// 空いている添え字がある場合
		if (!m_FreeIndexs.empty())
		{
			// 空いている添え字を取得して、配列から削除
			handle.index = m_FreeIndexs.back();
			m_FreeIndexs.pop_back();

			// 世代を取得 (削除時に更新しているため、そのまま使用)
			handle.generation = m_Generations[handle.index];
			// データを更新
			m_Datas[handle.index] = data;
			// 名前を保存
			m_IndexToNames[handle.index] = name;
			// 参照を更新
			m_RefHandleCounts[handle.index] = 1;
		}
		else
		{
			// 添え字を作成
			handle.index = (uint32_t)m_Datas.size();

			// データを更新
			m_Datas.push_back(data);
			// 名前を更新
			m_IndexToNames.push_back(name);
			// 参照を初期化
			m_RefHandleCounts.push_back(1);

			// 世代を初期化
			m_Generations.push_back(0);
			handle.generation = 0;
		}

		// 名前からハンドルを保存
		m_NameToHandleMap[name] = handle;
		return handle;
	}


	// ================================
	// データ取得
	// ================================
	T* GetData(const Handle& handle)
	{
		// 添え字をチェック
		if (handle.index >= m_Datas.size()) { return nullptr; }

		// 世代をチェック
		if (handle.generation != m_Generations[handle.index]) { return nullptr; }

		return &m_Datas[handle.index];
	}


	// ================================
	// 名前からハンドルを取得
	// ================================
	const Handle GetHandle(const std::string& name) const
	{
		// 名前からハンドルを取得
		auto it = m_NameToHandleMap.find(name);

		// 見つかったらハンドルを返す
		if (it != m_NameToHandleMap.end())
		{
			return it->second;
		}

		return Handle();
	}


	// ================================
	// 存在チェック
	// ================================
	bool Exists(const std::string& name) const
	{
		return m_NameToHandleMap.find(name) != m_NameToHandleMap.end();
	}


	// ================================
	// 削除
	// ================================
	void Remove(Handle handle)
	{
		// 存在チェック
		if (m_IndexToNames.size() <= handle.index) { return; }
		// 世代チェック
		if (handle.generation != m_Generations[handle.index]) { return; }

		// ハンドルから名前を取得
		std::string& name = m_IndexToNames[handle.index];

		// 名前が空かチェック
		if (name.empty()) { return; }

		// 参照カウントをデクリメント
		m_RefHandleCounts[handle.index]--;

		// 参照カウントが0なら削除
		if (m_RefHandleCounts[handle.index] == 0)
		{
			// 世代を更新
			m_Generations[handle.index]++;

			// 空き添え字に追加
			m_FreeIndexs.push_back(handle.index);

			// 配列から削除
			m_NameToHandleMap.erase(name);
			// ハンドルから名前を削除
			m_IndexToNames[handle.index].clear();
		}
	}


	// ================================
	// データ削除
	// ================================
	void ALLClear()
	{
		m_Datas.clear();
		m_Generations.clear();
		m_RefHandleCounts.clear();
		m_FreeIndexs.clear();

		m_NameToHandleMap.clear();
		m_IndexToNames.clear();
	}

};

