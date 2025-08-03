#pragma once

// ===============================================
// 経過時間を取得・渡す関数群
// ===============================================

namespace Timer
{
	// =======================================================
    // 関数
    // =======================================================
	void Init();    // 初期化
	void Start();   // 計測開始時間を記録

#if defined(DEBUG) || defined(_DEBUG)
	// デバック時にのみ有効　ゲームループの開始で呼び出してください
	// 毎フレームLastUpdate()が呼ばれているかのチェックをしています
	void Debug_CheckUpdate();
#else
	inline void Debug_CheckUpdate() {} // コンパイル時に展開されるように
#endif

	// ゲームループの一番最後で呼び出す処理
	void LastUpdate();
	
	// 1フレームの経過時間を返す
	float GetDeltaTime();
	// 計測開始からの経過時間を戻り値で返す
	float GetElapsedTime(); 


	// ---------------------------------------------------
	// 自分で追加できるタイム関数群
	// ---------------------------------------------------
	namespace Label {
		// タイマー開始
		void StartTimer(const char* name);
		// 経過時間取得
		float GetElapsedTimer(const char* name);
		// タイマー削除
		bool RemoveTimer(const char* name);
		// 全削除（リセット）
		void ClearTimers();
	}


};