#pragma once


// ===================================
// 【クラス概要】
// テンプレートを使用したマネージャー
// vectorで実際のデータを保存し、
// unordered_mapで名前からハンドルを返す
// resizeで配列を確保すると効率が良くなる
// 
// 【注意点】
// 動的確保を行う場合はかならずスマートポインターを使用してください。
// RAIIがないと必ずメモリーリークになります。
// ===================================


// ===================================
// ヘッダー
// ===================================
// 配列用ヘッダー
#include <vector>
#include <unordered_map>
// 文字列ヘッダー
#include "Hashed_String.h"
// ハンドル構造体
#include "Handle.h"

// ログ出力
#include "ReportMessage.h"


// ===================================
// クラス
// ===================================
// テンプレートマネージャークラス
template<
	typename T,
	typename Key = Hashed_String,
	typename Hash = std::hash<Key>,
	typename Equal = std::equal_to<Key>>
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
	std::unordered_map<Key, Handle, Hash, Equal> m_KeyToHandleMap;
	// インデックスから名前を取得する配列 (複数制作しない用)
	std::vector<Key> m_IndexToKeys;

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

		m_KeyToHandleMap.reserve(size);
		m_IndexToKeys.reserve(size);
	}


	// ==============================
	// データ追加
	// ==============================
	Handle AddData(const Key& key, const T& data)
	{
		// 既に存在する場合はハンドルを返す
		auto it = m_KeyToHandleMap.find(key);

		if (it != m_KeyToHandleMap.end()) 
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
			m_IndexToKeys[handle.index] = key;
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
			m_IndexToKeys.push_back(key);
			// 参照を初期化
			m_RefHandleCounts.push_back(1);

			// 世代を初期化
			m_Generations.push_back(0);
			handle.generation = 0;
		}

		// キーからハンドルを保存
		m_KeyToHandleMap[key] = handle;
		return handle;
	}


	// ================================
	// データ取得
	// ================================
	T* GetData(const Handle& handle)
	{
		// 添え字をチェック
		if (handle.index >= (uint32_t)m_Datas.size()) { 
			ErrorLog::OutputToConsole(u8"TemplateManager::GetData - 無効な添え字です");
			return nullptr; }

		// 世代をチェック
		if (handle.generation != m_Generations[handle.index]) { 
			ErrorLog::OutputToConsole(u8"TemplateManager::GetData - 無効な世代です");
			return nullptr; }

		return &m_Datas[handle.index];
	}


	// ================================
	// 名前からハンドルを取得
	// ================================
	const Handle GetHandle(const Key& key)
	{
		// キーからハンドルを取得
		auto it = m_KeyToHandleMap.find(key);

		// 見つかったらハンドルを返す
		if (it != m_KeyToHandleMap.end())
		{
			// ハンドル取得
			Handle handle = it->second;
			// 参照カウントを増やす
			m_RefHandleCounts[handle.index]++;

			return handle;
		}

		return Handle();
	}


	// ================================
	// 存在チェック
	// ================================
	bool Exists(const Key& key) const
	{
		return m_KeyToHandleMap.find(key) != m_KeyToHandleMap.end();
	}


	// ================================
	// 削除
	// ================================
	void Remove(Handle handle)
	{
		// 存在チェック
		if (m_IndexToKeys.size() <= handle.index) { return; }
		// 世代チェック
		if (handle.generation != m_Generations[handle.index]) { return; }

		// ハンドルから名前を取得
		const Key& key = m_IndexToKeys[handle.index];

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
			m_KeyToHandleMap.erase(key);
			// ハンドルから削除
			m_IndexToKeys[handle.index] = Key();

			// データの初期化
			// コンパイル時にどちらかか確定させる
			if constexpr (requires(T a) { a.reset(); }) 
			{
				// .reset 関数がある場合
				m_Datas[handle.index].reset();
				DebugLog::OutputToConsole(u8".restを実行");
			}
			if constexpr (requires(T a) { a.Clear(); })
			{
				// .Clear 関数がある場合
				m_Datas[handle.index].Clear();
				DebugLog::OutputToConsole(u8"Clear関数を実行");
			}
			else {
				// デフォルト初期化
				m_Datas[handle.index] = T{};
				DebugLog::OutputToConsole(u8"デフォルトコンストラクタを実行");
			}
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

		m_KeyToHandleMap.clear();
		m_IndexToKeys.clear();
	}

};

