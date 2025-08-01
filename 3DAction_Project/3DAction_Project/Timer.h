﻿#pragma once

// ===============================================
// 経過時間を取得・渡す関数群
// ===============================================

namespace Timer
{
	// =======================================================
	// 前方宣言
	// =======================================================
	struct TimePoint; // LARGE_INTEGERを隠す


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

	void LastUpdate(); // ゲームループの一番最後で呼び出す処理
	
	float GetDeltaTime(); // 1フレームの経過時間を返す
	float GetElapsedTime(const TimePoint& startTime); // 引き数で渡した時間からの経過時間を戻り値で返す
	float GetElapsedTime(); // 計測開始からの経過時間を戻り値で返す


};